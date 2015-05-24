#include "stdafx.h"
#include "GenericConstants.h"
#include "math.h"
#include <Geometry/Primitives.h>
#include <Geometry/Converters.h>
#include <Matrix/Matrix.h>
#include <VirtualSpace/ViewSpace.h>
#include <VirtualSpace/ViewFrustum.h>
#include "LookupTables.h"
#include <GL/gl.h>
#include <GL/glu.h>

ViewFrustum::ViewFrustum(){}

ViewFrustum::ViewFrustum(float a_fltNearPlaneZ, float a_fltFarPlaneZ, float a_fltFieldOfViewInY, float a_fltAspectRatio)
{
	Init(a_fltNearPlaneZ, a_fltFarPlaneZ, a_fltFieldOfViewInY, a_fltAspectRatio);
}

void ViewFrustum::Init(float a_fltNearPlaneZ, float a_fltFarPlaneZ, float a_fltFieldOfViewInY, float a_fltAspectRatio)
{
	m_fltNearPlaneZ = a_fltNearPlaneZ;// + 1.0f;
	m_fltFarPlaneZ = a_fltFarPlaneZ;// - 2.0f;
	m_fltFieldOfViewInY = a_fltFieldOfViewInY/2.0f;
	m_fltAspectRatio = a_fltAspectRatio;

	//What was failing before was taking the field of view in Y and using the aspect ratio to
	//calculate the fov in X - that is geometrically incorrect.  The aspect ratio can only
	//correctly be used against the width of the screen, not its angle.

	float fltNL, fltNR, fltNB, fltNT = 0;

	fltNT = m_fltNearPlaneZ * tan(m_fltFieldOfViewInY * dblD2R);
	fltNB = -fltNT;
	fltNR = fltNT * m_fltAspectRatio;
	fltNL = -fltNR;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(fltNL, fltNR, fltNB, fltNT, m_fltNearPlaneZ, m_fltFarPlaneZ);
}

	//Takes the data provided in the constuctor and populates the default frustum points array, which we'll use as the source for rotation into world space

IViewFrustum& ViewFrustum::GenerateFrustum()
{

	//Calculate the points of the near plane

	float fltNL, fltNR, fltNB, fltNT = 0;

	fltNT = 1.05f * m_fltNearPlaneZ * tan(0.95f * m_fltFieldOfViewInY * dblD2R); //push the near plane out a bit
	fltNB = -fltNT;
	fltNR = fltNT * m_fltAspectRatio;
	fltNL = -fltNR;

	//Calculate the points of the far plane

	float fltFL, fltFR, fltFB, fltFT = 0;

	fltFT = 0.95f * m_fltFarPlaneZ * tan(m_fltFieldOfViewInY * dblD2R); //pull the far plane in a bit
	fltFB = -fltFT;
	fltFR = fltFT * m_fltAspectRatio;
	fltFL = -fltFR;

	//Far plane first

	m_p3fFrustumDefault[FTL].Init(fltFL, fltFT, m_fltFarPlaneZ);
	m_p3fFrustumDefault[FTR].Init(fltFR, fltFT, m_fltFarPlaneZ);
	m_p3fFrustumDefault[FBR].Init(fltFR, fltFB, m_fltFarPlaneZ);
	m_p3fFrustumDefault[FBL].Init(fltFL, fltFB, m_fltFarPlaneZ);

	//Near plane next

	m_p3fFrustumDefault[FTL].Init(fltNL, fltNT, m_fltNearPlaneZ);
	m_p3fFrustumDefault[FTR].Init(fltNR, fltNT, m_fltNearPlaneZ);
	m_p3fFrustumDefault[FBR].Init(fltNR, fltNB, m_fltNearPlaneZ);
	m_p3fFrustumDefault[FBL].Init(fltNL, fltNB, m_fltNearPlaneZ);

	//Work out the line equations for our 4 corners between the far and near planes

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//We need to populate our 4 corner line segments using our corresponding corner points

	//Similarly, we need to populate our 4 edge line segments using points on each plane, near and far

	return *this;
}

IViewFrustum& ViewFrustum::PositionInWorldSpace(IViewSpace* ap_vsViewSpace)
{
	//We need to take the arrays of frustum points and rotate them all to agree with the view volume
	//Because we know we have 8 points here. this may be an ideal use of simd/sse instructions

	ap_vsViewSpace->TransformDefaultSpaceToViewSpace(m_p3fFrustumDefault, m_p3fFrustumWorld, NUM_PTS);

	//Place the view volume to make it relative to the point of eye.

	m_p3dEye = ap_vsViewSpace->Eye();
	mp_vwViewSpace = ap_vsViewSpace;
	Vector3d v3dEyePos;
	m_p3dEye.GetAsPositionVector(v3dEyePos);

	//WE REALLY REALLY REALLY REALLY NEED TO MAKE VIEW COORDINATES END UP BEING TRUE TO WORLD COORDINATES
/*
	for (int i=0; i<NUM_PTS; i++)
		m_p3fFrustumWorld[i] += v3dEyePos;
*/
	Vector3f v3fEyePos;
	Geometry::Vector3::ConvertD2F(v3dEyePos, v3fEyePos);
	for (int i=0; i<NUM_PTS; i++)
		m_p3fFrustumWorld[i] += v3fEyePos;
	CalculateFrustumLineEquations();

	return *this;
}

//Populates the lines that describe the frustum corners and frustum near/far edges

void ViewFrustum::CalculateFrustumLineEquations()
{
	for (Int64 i=0; i<NUM_CORNERS; i++)
		m_ls3fCorners[i].Init(m_p3fFrustumWorld[i], m_p3fFrustumWorld[i+4]);

	for (Int64 i=0; i<(NUM_FACE_EDGES/2)-1; i++)
	{
		//Far face
		
		m_l3fFaceEdges[i].Init(m_p3fFrustumWorld[i], m_p3fFrustumWorld[i+1]);

		//Near face

		m_l3fFaceEdges[i+4].Init(m_p3fFrustumWorld[i+4], m_p3fFrustumWorld[i+5]);
	}
}

void ViewFrustum::Render()
{
	Point3f* F = m_p3fFrustumWorld;

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(10, 0xAAAA);
	glBegin(GL_LINES);
	{
		//Draw the far plane first

		glVertex3f(F[FTL].X, F[FTL].Y, F[FTL].Z);
		glVertex3f(F[FTR].X, F[FTR].Y, F[FTR].Z);

		glVertex3f(F[FTR].X, F[FTR].Y, F[FTR].Z);
		glVertex3f(F[FBR].X, F[FBR].Y, F[FBR].Z);

		glVertex3f(F[FBR].X, F[FBR].Y, F[FBR].Z);
		glVertex3f(F[FBL].X, F[FBL].Y, F[FBL].Z);

		glVertex3f(F[FBL].X, F[FBL].Y, F[FBL].Z);
		glVertex3f(F[FTL].X, F[FTL].Y, F[FTL].Z);

		//Now draw the near plane

		glVertex3f(F[NTL].X, F[NTL].Y, F[NTL].Z);
		glVertex3f(F[NTR].X, F[NTR].Y, F[NTR].Z);

		glVertex3f(F[NTR].X, F[NTR].Y, F[NTR].Z);
		glVertex3f(F[NBR].X, F[NBR].Y, F[NBR].Z);

		glVertex3f(F[NBR].X, F[NBR].Y, F[NBR].Z);
		glVertex3f(F[NBL].X, F[NBL].Y, F[NBL].Z);

		glVertex3f(F[NBL].X, F[NBL].Y, F[NBL].Z);
		glVertex3f(F[NTL].X, F[NTL].Y, F[NTL].Z);

		//Now draw the connecting corner lines

		glVertex3f(F[FTL].X, F[FTL].Y, F[FTL].Z);
		glVertex3f(F[NTL].X, F[NTL].Y, F[NTL].Z);

		glVertex3f(F[FTR].X, F[FTR].Y, F[FTR].Z);
		glVertex3f(F[NTR].X, F[NTR].Y, F[NTR].Z);

		glVertex3f(F[FBR].X, F[FBR].Y, F[FBR].Z);
		glVertex3f(F[NBR].X, F[NBR].Y, F[NBR].Z);

		glVertex3f(F[FBL].X, F[FBL].Y, F[FBL].Z);
		glVertex3f(F[NBL].X, F[NBL].Y, F[NBL].Z);

	}
	glDisable(GL_LINE_STIPPLE);
}






float ViewFrustum::GetDiagonalFieldOfView()
{
	float fltNT = m_fltNearPlaneZ * tan(m_fltFieldOfViewInY * dblD2R);
	float fltNR = fltNT * m_fltAspectRatio;
	
	float fltNearPlaneDiagonalDistance = sqrt(fltNT*fltNT + fltNR*fltNR);
	float fltDiagFov = atan(fltNearPlaneDiagonalDistance/m_fltNearPlaneZ)/dblD2R;

	return fltDiagFov;
}