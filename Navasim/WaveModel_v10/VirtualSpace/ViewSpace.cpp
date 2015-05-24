#include "stdafx.h"
#include "GenericConstants.h"
//#include "math.h"
#include <GL/gl.h>
#include <Geometry/Primitives.h>
#include <Matrix/Matrix.h>
#include <VirtualSpace/ViewSpace.h>

void ViewSpace::Init(Point3d& a_p3dEye, float a_fltHeadingAngle, float a_fltVerticalViewAngle, float a_fltTiltAngle)
{
	m_fltHeadingAngle = a_fltHeadingAngle;
	m_fltVerticalViewAngle = a_fltVerticalViewAngle;
	m_fltTiltAngle = a_fltTiltAngle;
	m_p3dEye = a_p3dEye;

	ApplyRotations();
}

void ViewSpace::ApplyRotations()
{
	m_m44Axes.SetIdentity();
	m_m44Axes.RotateInWorldY(m_fltHeadingAngle);
	m_m44Axes.RotateInWorldX(m_fltVerticalViewAngle);
	m_m44Axes.RotateInLocalZ(m_fltTiltAngle);	
}

void ViewSpace::Look()
{
	glMultMatrixf(this->m_m44Axes.m_fpElements);
	glTranslatef(-m_p3dEye.X, -m_p3dEye.Y, -m_p3dEye.Z);	
}

void ViewSpace::SetGlIdentLeftHand()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(1,1,-1);
}

void ViewSpace::SetGlIdentRightHand()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool ViewSpace::IsTiltApplied()
{
	return (m_fltTiltAngle != 0.0);
}

void ViewSpace::TransformDefaultSpaceToViewSpace(Point3f* ap_p3fDefault, Point3f* ap_p3fOut, Int64 a_i64NumPoints)
{
	m_m44Axes.Transform(ap_p3fDefault, ap_p3fOut, a_i64NumPoints);
}

void ViewSpace::TranslateInX(float fltDistance)
{
	m_p3dEye.X += fltDistance;
}

void ViewSpace::TranslateInY(float fltDistance)
{
	m_p3dEye.Y += fltDistance;
}

void ViewSpace::TranslateInZ(float fltDistance)
{
	m_p3dEye.Z += fltDistance;
}

void ViewSpace::RotateInX(float fltDeltaUpDownAngle)
{
	m_fltVerticalViewAngle = LookupTables::NormaliseHeading(m_fltVerticalViewAngle + fltDeltaUpDownAngle);
	//ApplyRotations();
}

void ViewSpace::RotateInY(float fltHeadingDelta)
{
	m_fltHeadingAngle = LookupTables::NormaliseHeading(m_fltHeadingAngle + fltHeadingDelta);
	//ApplyRotations();
}

void ViewSpace::RotateInZ(float fltDeltaTiltAngle)
{
	m_fltTiltAngle = LookupTables::NormaliseHeading(m_fltTiltAngle + fltDeltaTiltAngle);
	//ApplyRotations();
}

void ViewSpace::Look(float fltHeading)
{
	m_fltHeadingAngle = LookupTables::NormaliseHeading(fltHeading);
	//ApplyRotations();
}

void ViewSpace::LevelHead()
{
	m_fltVerticalViewAngle = 0;
	//ApplyRotations();
}

void ViewSpace::LevelTilt()
{
	m_fltTiltAngle = 0;
	//ApplyRotations();
}

void ViewSpace::ToOrigin()
{
	m_p3dEye.Init(0,0,0);
}

void ViewSpace::TranslateAlongViewVector(float fltDistance)
{
	Matrix4x4f m44Step;
	//vsViewSpace.m_m44Axes.Multiply(fltViewVectorDelta, m44Step);

	_Align16_ Vector3f v3fViewDefault(0.0f,0.0f,1.0f);
	_Align16_ Vector3f v3fViewActual;

	Matrix4x4f::Multiply(m_m44Axes, v3fViewDefault, v3fViewActual);

	v3fViewActual *= fltDistance;
	Vector3d v3dViewActual;
	Geometry::Vector3::ConvertF2D(v3fViewActual, v3dViewActual);
	m_p3dEye += v3dViewActual;
}