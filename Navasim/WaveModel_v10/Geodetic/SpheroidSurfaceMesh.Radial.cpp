#include "stdafx.h"
#include <GenericConstants.h>
#include <LookupTables.h>
#include <Geometry/Primitives.h>
#include <Geodetic/SpheroidSurfaceMesh.h>
#include <VirtualSpace/IViewFrustum.h>
#include <VirtualSpace/IViewSpace.h>
#include <Geodetic/Spheroid.h>


void SpheroidSurfaceMesh::PreAllocateRadialVertexStorage()
{
	//Allocate memory for the vertex positions from 0-359, out to the maximum number of range rings, and
	//also allocate an array to store the number of actual range rings per radial (0-180 only)

	//We have two numbers to work out:
	//Firstly, the maximum possible number of vertices per radial, given the range ring delta
	//Secondly, the number of radials given the radial delta
	//Given those results, we can allocate the memory we need.

	//We're going to be over-generous here, and just use the range ring delta all the way from
	//0 degrees to 90 degrees, even though we'll never use the higher angles.

	float fltMaxRangeRingCount = 90.0f/m_defMesh.fltRangeRingDelta;
	UInt16 ui16MaxRangeRingCount = static_cast<UInt16>(floor(fltMaxRangeRingCount));

	//Now with radials, the divisor must be exactly divisible into 180.  If it is not, we
	//need to assert an issue.  The number should be exactly divisible by 2 so that we don't
	//get rounding errors.

	float fltRadialCount = (360.0f/m_defMesh.fltRadialDelta)+1;
	UInt16 ui16RadialCount = static_cast<UInt16>(floor(fltRadialCount));

	mp_p3fMeshVertices = new Point3f[ui16RadialCount * ui16MaxRangeRingCount];
}

void SpheroidSurfaceMesh::SpawnRadialGeneration(SurfaceMeshSector& smsHorizon, SurfaceMeshSector& smsLateral, SurfaceMeshSector& smsRear)
{
	//Given pre-generated range-ring distance information and wedge sector boundaries, spawn the generation
	//of radial vertex data, preferably using a thread-pool

	for (short i16Index=smsHorizon.i16IndexLeft; i16Index<=smsHorizon.i16IndexRight; i16Index++)
		GenerateRadialVertices(i16Index, m_smsHorizon.p_l3fBoundary, &mp_p3fMeshVertices);

	for (short i16Index=smsLateral.i16IndexLeft; i16Index<=smsLateral.i16IndexRight; i16Index++)
		GenerateRadialVertices(i16Index, m_smsLateral.p_l3fBoundary, &mp_p3fMeshVertices);

	for (short i16Index=smsRear.i16IndexLeft; i16Index<=smsRear.i16IndexRight; i16Index++)
		GenerateRadialVertices(i16Index, m_smsRear.p_l3fBoundary, &mp_p3fMeshVertices);
}


float SpheroidSurfaceMesh::GetRadialAngle(short a_i16RadialIndex)
{
	//Use this index to look up into the table of values, or calculate it

	return ((float)a_i16RadialIndex) * m_defMesh.fltRadialDelta;
}


//this should be a pooled operation and ultimately we want to make the input to this a void* of SurfaceMeshSector

void SpheroidSurfaceMesh::GenerateRadialVertices(UInt16 a_i16RadialIndex, //the angle of the radial to generate
		                                         Line3f* ap_l3fBoundary, //a line on the 
											     Point3f** app_p3fVertices)
{
	//Given radial parameters, generate a single radial's vertex data from centre to extremity

	UInt16 i16MaxRangeRingIndex = 0;
	bool blnPushToHorizon = false;

	//Get the radial's unit vector

	Vector3f v3fRadial;
	float fltRadialAngle = GetRadialAngle(a_i16RadialIndex);
	GetRadialUnitVector(fltRadialAngle, v3fRadial);

	if (ap_l3fBoundary != nullptr && a_i16RadialIndex != m_smsLateral.i16IndexLeft)
		i16MaxRangeRingIndex = DetermineMaxRangeRingIndex(v3fRadial, ap_l3fBoundary);
	else
	{
		//We're going to push all the way out to the horizon.  Get the last range ring before reaching the horizon.
		//We'll make sure we do the horizon ring without using a range index - just the range itself.

		i16MaxRangeRingIndex = static_cast<UInt16>(floor(m_defMesh.fltHorizonAngleFromVertical/m_defMesh.fltRangeRingDelta));
		blnPushToHorizon = true;
	}

	//Generate the vertices now

	for (UInt16 i16RangeIndex=0; i16RangeIndex<=i16MaxRangeRingIndex; i16RangeIndex++)
	{
		//Get the range from this location by index

		float fltSPPtoVertexDistance = GetRingRangeByIndex(i16RangeIndex);
		GenerateAndStoreVertex(a_i16RadialIndex, v3fRadial, fltSPPtoVertexDistance);
	}

	if (blnPushToHorizon)
		GenerateAndStoreVertex(a_i16RadialIndex, v3fRadial, m_defMesh.fltHorizonDistFromSPP);

	//That should be it
}

UInt16 SpheroidSurfaceMesh::DetermineMaxRangeRingIndex(Vector3f& a_v3fRadial, Line3f* ap_l3fBoundary)
{
	UInt16 i16MaxRangeRingIndex = 0;

	//Determine the intersection of the boundary line with this radial, and how far away it is

	Line3f l3fRadial(a_v3fRadial);
	Point3f p3fIntersection;
	l3fRadial.IntersectOnXZPlane(*ap_l3fBoundary, p3fIntersection); //Disregard the intersection constant retval
		
	//We now have a point on the surface plane.
	//Work out the angle of this intersection point relative to the line between the POE and SPP and the
	//surface plane.

	float fltDistanceToSPP = p3fIntersection.DistanceToOrigin();
	float fltTanAngleToPOE = fltDistanceToSPP/m_defMesh.fltHOE;
	float fltAngleToPOE = atan(fltTanAngleToPOE);

	//Assuming for now that the range rings are equidistant angular-wise (not actual distance-wise),
	//work out the index of the range ring next greater than this angle, unless it falls exactly on a ring.

	float fltRangeRings = fltAngleToPOE/m_defMesh.fltRangeRingDelta;
	float fltNextInnerRing = floor(fltRangeRings);
	i16MaxRangeRingIndex = static_cast<UInt16>(fltNextInnerRing);

	//If the division falls over a range ring, increment to the next ring

	if (fltNextInnerRing < fltRangeRings)
		 i16MaxRangeRingIndex++;

	return i16MaxRangeRingIndex;
}

float SpheroidSurfaceMesh::GetRingRangeByIndex(UInt16 i16RangeIndex)
{
	//This is the angle from POE/SPP to POE/VERTEX
	float fltRadialAngle = i16RangeIndex * m_defMesh.fltRangeRingDelta;
	float fltVertexDistance = tan(static_cast<float>(fltRadialAngle * dblD2R)) * m_defMesh.fltHOE;
	return fltVertexDistance;
}

void SpheroidSurfaceMesh::GenerateAndStoreVertex(UInt16 a_ui16RadialIndex, Vector3f& a_v3fRadial, float a_fltSPPtoVertex)
{

	Vector3f v3fPoint;
	a_v3fRadial.Multiply(a_fltSPPtoVertex, v3fPoint);

	//Store the vertex

	//store it
}

void SpheroidSurfaceMesh::GetRadialUnitVector(float fltRadialAngle, Vector3f& v3fOut)
{
	float fltSine, fltCosine = 0.0f;
	LookupTables::SineAndCosine(fltRadialAngle, fltSine, fltCosine);

	v3fOut.Init(fltCosine, 0, fltSine);
}


