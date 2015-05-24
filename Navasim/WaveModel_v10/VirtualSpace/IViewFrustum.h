#pragma once
#include <VirtualSpace/IViewSpace.h>
class IViewFrustum
{
public:
	virtual void Init(float a_fltNearPlaneZ, float a_fltFarPlaneZ, float a_fltFieldOfViewInY, float a_fltAspectRatio) = 0;
	virtual IViewFrustum& GenerateFrustum() = 0;
	virtual IViewFrustum& PositionInWorldSpace(IViewSpace* ap_vs) = 0;
	virtual void Render() = 0;
	virtual float GetDiagonalFieldOfView() = 0;
};