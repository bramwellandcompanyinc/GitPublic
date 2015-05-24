//This class is the aggregation of all the parameters that represent the transformation required to bring objects in world space into view space.  That is,
//it holds the current point of eye, view vector, rotation matrix required to make the vector (0,0,1) transform into world space z-axis, the angular rotations
//applied to generate the rotation matrix.

//This class should be independent of any considerations of spheroid geometry - it is really just a class (stuct) designed to encapsulate view-space related
//information.  This will mean we need to translate between world-space and spheroid coordinates, but that is ok; we don't want too much spheroid stuff
//permeating simple classes.

#pragma once
#include <VirtualSpace/IViewSpace.h>
#include <Matrix/Matrix.h>

class ViewSpace : public IViewSpace
{
public:
	Point3d m_p3dEye;
	Matrix4x4f m_m44Axes;
	float m_fltHeadingAngle;
	float m_fltVerticalViewAngle;
	float m_fltTiltAngle;

public:

	ViewSpace()
	{
	}

	void Init(Point3d& a_p3dEye, float a_fltHeadingAngle, float a_fltVerticalViewAngleView, float a_fltTiltAngle);

	//For this one, let's return a copy of the eye, as we don't want the view space being modified inadvertently

	Point3d& Eye()
	{
		return m_p3dEye;
	}

	float VerticalViewAngle()
	{
		return m_fltVerticalViewAngle;
	}

	bool IsTiltApplied();

	void TransformDefaultSpaceToViewSpace(Point3f* ap_p3fDefault, Point3f* ap_p3fOut, Int64 a_i64NumPoints);
	void TranslateAlongViewVector(float fltDistance);
	void TranslateInX(float fltDistance);
	void TranslateInY(float fltDistance);
	void TranslateInZ(float fltDistance);
	void RotateInX(float fltDeltaUpDownAngle);
	void RotateInY(float fltHeadingDelta);
	void RotateInZ(float fltDeltaTiltAngle);
	void Look(float fltHeading);
	void Look();
	void LevelHead();
	void LevelTilt();
	void ToOrigin();
	void ApplyRotations();
	void SetGlIdentLeftHand();
	void SetGlIdentRightHand();

	/*
	virtual void Init(Point3d& a_p3dEye, float& a_fltHeadingAngle, float& a_fltVerticalViewAngleView, float& a_fltTiltAngle) = 0;
	virtual Point3d Eye() = 0;
	virtual float VerticalViewAngle() = 0;
	virtual bool IsTiltApplied() = 0;
	virtual void TransformDefaultSpaceToViewSpace(Point3f* ap_p3fDefault, Point3f* ap_p3fOut, Int64 a_i64NumPoints) = 0;
	virtual void TranslateInX(float fltDistance) = 0;
	virtual void TranslateInY(float fltDistance) = 0;
	virtual void TranslateInZ(float fltDistance) = 0;
	virtual void TranslateAlongViewVector(float fltDistance) = 0;
	virtual void RotateInX(float fltDeltaUpDownAngle) = 0;
	virtual void RotateInY(float fltHeadingDelta) = 0;
	virtual void RotateInZ(float fltDeltaTiltAngle) = 0;
	virtual void Look(float fltHeading) = 0;
	virtual void LevelHead() = 0;
	virtual void LevelTilt() = 0;
	virtual void ToOrigin() = 0;
	virtual void ApplyRotations() = 0;
	*/
};


