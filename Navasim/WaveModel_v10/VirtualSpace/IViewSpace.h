#ifndef iviewspace_h
#define iviewspace_h

#include <Geometry/Primitives.h>

class IViewSpace
{
public:
	virtual void Init(Point3d& a_p3dEye, float a_fltHeadingAngle, float a_fltVerticalViewAngleView, float a_fltTiltAngle) = 0;
	virtual Point3d& Eye() = 0;
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
	virtual void Look() = 0;
	virtual void SetGlIdentLeftHand() = 0;
	virtual void SetGlIdentRightHand() = 0;

};

enum TranslationDirectiveEnum
{
	TranslateNegative=-1, TranslateNone=0, TranslatePositive=+1
};

enum HeadingDirectiveEnum
{
	HeadAnticlockwise=-1, HeadingNoDelta=0, HeadClockwise=+1
};

enum UpDownDirectiveEnum
{
	Up=+1, Level=0, Down=-1
};

enum TiltDirectiveEnum
{
	TiltAnticlockwise=-1, TiltLevel=0, TiltClockwise=+1
};

#endif