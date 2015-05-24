#include "stdafx.h"
#include <GenericConstants.h>
#include <LookupTables.h>
#include <Geometry/Primitives.h>
#include <VirtualSpace/ViewFrustum.h>
#include <VirtualSpace/ViewSpace.h>
#include <Geodetic/Spheroid.h>
#include <Geodetic/SpheroidSurfaceMesh.h>
#include <Threading/ThreadPool.h>
#include <Threading/ThreadPoolRequest.h>

void SpheroidSurfaceMesh::OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	Vector3d v3dNorthPole(0.0f, 1.0f, 0.0f);
	mp_sphSubject = new Spheroid(6400000, 6400000, v3dNorthPole);
	
	defInput.mp_vf = ap_vf;
	defInput.mp_vs = ap_vs;
	defInput.mp_sph = mp_sphSubject;

	PreAllocateRadialVertexStorage();
	PreGenerateRangeRingDistances(0, 0, 0, nullptr);

}



void SpheroidSurfaceMesh::Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	defInput.p3dPOE = ap_vs->Eye();

	this->CalculateBoundaryDefinitionData(defInput,  m_defMesh);
	DetermineAngularDeltasFromHOE(m_defMesh.fltHOE,m_defMesh.fltRadialDelta, m_defMesh.fltRangeRingDelta);
	CalculateWedgeSectors();

}

void SpheroidSurfaceMesh::Render()
{
}

void SpheroidSurfaceMesh::Deallocate()
{
	delete mp_sphSubject;
}



void SpheroidSurfaceMesh::GenerateMesh(SurfaceMeshBoundaryDefinitionInput* ap_defIn)
{

}


void SpheroidSurfaceMesh::DetermineAngularDeltasFromHOE(float a_fltHOE, float& a_fltRadialDelta, float& a_fltRangeRingDelta)
{
	//Given the HOE, determine the appropriate angular separation bewteen range rings.

	a_fltRadialDelta = 1.0f;
	a_fltRangeRingDelta = 1.0f;
}


void SpheroidSurfaceMesh::PreGenerateRangeRingDistances(float a_fltHOE,
													  float a_fltHorizonLineOfSightDistance,
													  float a_fltHorizonAngleFromVertical,
													  float** app_fltIndexedDistances)
{
	//Given the HOE and line-of-sight distance to the horizon, pre-generate the range distances from the SPP
	//based upon an angular separation appropriate for the HOE

}


void SpheroidSurfaceMesh::CalculateBoundaryDefinitionData(SurfaceMeshBoundaryDefinitionInput& a_defIn,
		                                                  SurfaceMeshBoundaryDefinitionOutput& a_defOut)
{
	//Based upon the input geodetic information and view frustum, determine the points HC, HR, NR, and NC

	a_defOut.fltHOE = a_defIn.p3dPOE.Y; //clearly not going to do for long. This should be taken from the POE and spheroid.

	CalculateHorizonBoundaryPoints(a_defIn, a_defOut);
	CalculateUnderfootBoundaryPoints(a_defIn, a_defOut);

	//For now, we'll just make the radials and distance rings based on 1 degree

	DetermineAngularDeltasFromHOE(a_defOut.fltHOE, a_defOut.fltRadialDelta, a_defOut.fltRangeRingDelta);

	a_defOut.l3fLateral.Init(a_defOut.p3fHR, a_defOut.p3fNR);
	a_defOut.l3fRear.Init(a_defOut.p3fNR, a_defOut.p3fNC);
}

void SpheroidSurfaceMesh::CalculateHorizonBoundaryPoints(SurfaceMeshBoundaryDefinitionInput& a_defIn,
		                                                 SurfaceMeshBoundaryDefinitionOutput& a_defOut)
{
	//Let firstly find how wide the view frustum is - in degrees - so we know how wide to make it, and add the fudge factor to make sure we make it a little bigger
	//It is actually the half angle of the diagonal plus the mesh adjustment

	float fltAdjFrustumDiagonalFov = a_defIn.mp_vf->GetDiagonalFieldOfView();// + SPH_SURF_MESH_ANGLE_ADJUST;
	
	//Get the horizon parameters

	float fltHorizonDistanceFromPOE, fltHorizonDistanceFromSPP, fltHorizonDepressionFromPOE, fltHorizonDepressionBelowSurface = 0;
	a_defIn.mp_sph->GetHorizonParameters(a_defIn.p3dPOE, fltHorizonDistanceFromPOE, fltHorizonDistanceFromSPP, fltHorizonDepressionFromPOE, fltHorizonDepressionBelowSurface);

	a_defOut.fltHorizonDistFromSPP = fltHorizonDistanceFromSPP;
	a_defOut.fltHorizonAngleFromVertical = 90-fltHorizonDepressionFromPOE;

	//Based on the distance to the horizon from the POE we need to work out the angle between the left point (L), S, and the right point (R).
	//This will give us the angle to calculate the vector from.  The angle on the surface LSR will be close to LPR but not exactly the same.
	//So, if we define a triangle from points L and R on the surface at the horizon and P, because we know PH we can work out the distance LR.
	//Once we know LR, we can work out the angle LSR.

	//Now lets get the distance LR

	float LR = 2*pow(fltHorizonDistanceFromPOE, 2)*(1-cos(fltAdjFrustumDiagonalFov * dblD2R));

	//Now we have the length LR, and we already know the other two sides SH, we have a triangle with sides of length LR, SH, SH.  If we define a point
	//D in the centre of LR, we can establish the distance DS, which will be the distance in Z that the horizon points will be at, and the distances
	//LD and DR (equivalent) will provide the X distance.  We have the info required to work out that distance using pythagorus:
	//SH^2 = LD^2 + DS^2

	float LD = LR/2; //so we have the X distance for the horizon points
	float DS = sqrt(pow(fltHorizonDistanceFromSPP, 2) - LD*LD); //we now have the Z distance for the horizon points

	a_defOut.p3fHC.Init(0, 0, fltHorizonDistanceFromSPP);
	a_defOut.p3fHR.Init(LD, 0, DS); //we only need to define one point - we're just going to mirror it later anyway

	//We can also work out the angle of the horizon right point for the radial calculations

	a_defOut.fltAngleHR = static_cast<float>(atan(LD/DS)*dblR2D);
}

void SpheroidSurfaceMesh::CalculateUnderfootBoundaryPoints(SurfaceMeshBoundaryDefinitionInput& a_defIn,
		                                                   SurfaceMeshBoundaryDefinitionOutput& a_defOut)
{

	//Now, lets find the two opposing points that will be nearly below the POE, directly on the surface.  This will be done by again using the adjusted
	//diagonal half-fov angle but we'll construct points that are the same distance left and right in x, and the same distance back in z, as this angle's
	//projection for the height of eye.  Let's re-use our temporaries from above for the same purpose.

	float fltAdjFrustumDiagonalFov = a_defIn.mp_vf->GetDiagonalFieldOfView();// + SPH_SURF_MESH_ANGLE_ADJUST;

	float floatTanFov = LookupTables::Tangent(fltAdjFrustumDiagonalFov);
	float fltDistanceOnSurface = floatTanFov * m_defMesh.fltHOE;
	
	//We now have our two points below the POE

	a_defOut.p3fNC.Init(0, 0, fltDistanceOnSurface);
	a_defOut.p3fNR.Init(fltDistanceOnSurface, 0, -fltDistanceOnSurface);
	a_defOut.fltAngleNR = 135.0f; //we know this already for now because we use the same distance back as to the right
}

void SpheroidSurfaceMesh::CalculateWedgeSectors()
{
	//Based upon the angles of the corner points, determine the left and right angles of the three radial
	//generation loops that will populate the radial vertex array

	float fltHorizonRightRadialIndex = floor(m_defMesh.fltAngleHR/m_defMesh.fltRadialDelta);
	m_smsHorizon.Init(0, fltHorizonRightRadialIndex, NULL);

	float fltLateralLeftRadialIndex = fltHorizonRightRadialIndex + 1;
	float fltLateralRightRadialIndex = floor(m_defMesh.fltAngleNR/m_defMesh.fltRadialDelta)-fltLateralLeftRadialIndex;
	m_smsLateral.Init(fltLateralLeftRadialIndex, fltLateralRightRadialIndex, &(m_defMesh.l3fLateral));

	float fltRearLeftRadialIndex = fltLateralRightRadialIndex + 1;
	m_smsRear.Init(fltRearLeftRadialIndex, 180, &(m_defMesh.l3fRear));
}
