//degrees either side of the view frustum's greatest angular extent between opposing corners - a little fat

#pragma once
#include <Geometry/Primitives.h>
#include <VirtualSpace/ISceneObject.h>
#include <Geodetic/Spheroid.h>
#include <VirtualSpace/IViewSpace.h>
#include <VirtualSpace/IViewFrustum.h>
#include <Threading/ThreadPool.h>

#define SPH_SURF_MESH_ANGLE_ADJUST 3.0

//????????????

//should we just make these data members of the spheroidsurfacemesh class?  probably, it seems a reasonable way to deal with the data

struct SurfaceMeshSector
{
	short i16IndexLeft;
	short i16IndexRight;
	Line3f* p_l3fBoundary;

	SurfaceMeshSector(){}

	SurfaceMeshSector(short a_i16IndexLeft, short a_i16IndexRight, Line3f* ap_l3fBoundary):
		i16IndexLeft(a_i16IndexLeft), i16IndexRight(a_i16IndexRight), p_l3fBoundary(ap_l3fBoundary){}

	void Init(short a_i16IndexLeft, short a_i16IndexRight, Line3f* ap_l3fBoundary)
	{
		i16IndexLeft = a_i16IndexLeft;
		i16IndexRight = a_i16IndexRight;
		p_l3fBoundary = ap_l3fBoundary;
	}
};

struct SurfaceMeshBoundaryDefinitionInput
{
	Spheroid* mp_sph;
	Point3d p3dSPP;
	Point3d p3dPOE;
	IViewSpace* mp_vs;
	IViewFrustum* mp_vf;
};

struct SurfaceMeshBoundaryDefinitionOutput
{
	//Defines the boundary points for the surface mesh pie-wedge

	float fltHorizonDistFromSPP; //The far extremity of the wedge - the outer edge of the pie
	Point3f p3fHC; //horizon-centre in 2-space (the x/z plane)
	Point3f p3fHR; //horizon-right
	Point3f p3fNR; //near-right
	Point3f p3fNC; //near-centre
	Line3f l3fLateral;
	Line3f l3fRear;

	float fltHOE; //The height of eye as determined from the subject spheroid
	float fltHorizonAngleFromVertical; //The angle between the horizon and the line between the HOE and SPP
	float fltAngleHR; //The angle from the SPP to the HR point on the plane
	float fltAngleNR; //The angle from the SPP to the NR point on the plane

	float fltRadialDelta;
	float fltRangeRingDelta;
};


class SpheroidSurfaceMesh: public ISceneObject
{

#pragma region Interface Implementations
public:
	virtual ~SpheroidSurfaceMesh(){}
	void OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Render();
	void Deallocate();
#pragma endregion

private:
	Spheroid* mp_sphSubject;
	SurfaceMeshBoundaryDefinitionInput defInput;
	SurfaceMeshBoundaryDefinitionOutput m_defMesh;
	SurfaceMeshSector m_smsHorizon;
	SurfaceMeshSector m_smsLateral;
	SurfaceMeshSector m_smsRear;

	Point3f* mp_p3fMeshVertices;

public:

						SpheroidSurfaceMesh(){}
						SpheroidSurfaceMesh(Spheroid* ap_sphSubject):mp_sphSubject(ap_sphSubject){};
	void				GenerateMesh(SurfaceMeshBoundaryDefinitionInput* ap_defIn);

private:

	//Given the HOE, determine the appropriate angular separation bewteen range rings.

	void				DetermineAngularDeltasFromHOE(float a_fltHOE, float& a_fltRadialDelta, float& a_fltRangeRingDelta);

	//Given the HOE and line-of-sight distance to the horizon, pre-generate the range distances from the SPP
	//based upon an angular separation appropriate for the HOE

	void				PreGenerateRangeRingDistances(float a_fltHOE,
													  float a_fltHorizonLineOfSightDistance,
													  float a_fltHorizonAngleFromVertical,
													  float** app_fltIndexedDistances);
	
	//Allocate memory for the vertex positions from 0-359, out to the maximum number if range rings, and
	//also allocate an array to store the number of actual range rings per radial (0-180 only)

	void				PreAllocateRadialVertexStorage();

	//Based upon the input geodetic information and view frustum, determine the points HC, HR, NR, and NC

	void				CalculateBoundaryDefinitionData(SurfaceMeshBoundaryDefinitionInput& a_defIn,
		                                                SurfaceMeshBoundaryDefinitionOutput& a_defOut);

	//Determine the information required to define the radial mesh using position and frustum data
	//for the horizon points HR and HC

	void				CalculateHorizonBoundaryPoints(SurfaceMeshBoundaryDefinitionInput& a_defIn,
		                                               SurfaceMeshBoundaryDefinitionOutput& a_defOut);

	//Determine the information required to define the radial mesh using position and frustum data
	//for the underfoot points NR and NC

	void				CalculateUnderfootBoundaryPoints(SurfaceMeshBoundaryDefinitionInput& a_defIn,
		                                                 SurfaceMeshBoundaryDefinitionOutput& a_defOut);


	//Based upon the angles of the corner points, determine the left and right angles of the three radial
	//generation loops that will populate the radial vertex array

	void				CalculateWedgeSectors();

	//Given pre-generated range-ring distance information and wedge sector boundaries, spawn the generation
	//of radial vertex data, preferably using a thread-pool

	void				SpawnRadialGeneration(SurfaceMeshSector& smsHorizon, SurfaceMeshSector& smsLateral, SurfaceMeshSector& smsRear);

	//Given radial information and pre-generated , populate a single
	//radial's vertex information

	void				GenerateRadialVertices(UInt16 a_i16RadialIndex, //the index of the radial to generate
		                                       Line3f* ap_l3dBoundary, //a line on the 
											   Point3f** app_p3fVertices);

	//Given a radial index, get the angle that it represents

	float				GetRadialAngle(short a_i16RadialIndex); 

	//Given a radial, create a line equation assuming a 2d line on the x-z plane

	void				GetRadialUnitVector(float fltRadialAngle, Vector3f& v3fOut);


	//Given a radial and the boundary line equation, determine the outermost range ring index to which we'll push.
	//In the case of the horizon boundary, we'll end up pushing right to the horizon

	UInt16				DetermineMaxRangeRingIndex(Vector3f& a_v3fRadial, Line3f* ap_l3fBoundary);

	//Given the range ring index, find out the distance from SPP either by calculation
	//or by lookup.  This is essentially an ATAN problem.

	float				GetRingRangeByIndex(UInt16 i16RangeIndex);

	//Given a radial vector and the distance in the direction of that vector, calculate the
	//point and store it in the mesh's radial storage array

	void				GenerateAndStoreVertex(UInt16 a_ui16RadialIndex, Vector3f& a_v3fRadial, float fltDistance);
};
