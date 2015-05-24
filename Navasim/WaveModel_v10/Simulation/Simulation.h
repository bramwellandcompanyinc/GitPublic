#include "stdafx.h"
#include "GenericConstants.h"

#include <Windowing/IWindowEventHandler.h>
#include <Windowing/IWindowEventSource.h>
#include <VirtualSpace/IViewSpace.h>
#include <VirtualSpace/IViewFrustum.h>
#include <VirtualSpace/ISceneObject.h>

class Simulation : public IWindowEventHandler
{
private:
	IWindowEventSource* mp_wes;
	IViewSpace* mp_vw;
	IViewFrustum* mp_vfFrustum;

	ISceneObject** mpp_scobObjects;
	UInt16 m_i16SceneObjectCount;

	bool m_blnTerminateSimulation;
	bool m_blnRenderWireFrame;

	bool m_blnShiftKeyDown;

	float m_fltHeadingVelocity;
	float m_fltDeltaNodVelocity;
	float m_fltDeltaTiltVelocity;
	float m_fltTranslateVelocity;

	TranslationDirectiveEnum m_dirTranslateNorthSouth;
	TranslationDirectiveEnum m_dirTranslateEastWest;
	TranslationDirectiveEnum m_dirTranslateAlongView;
	TranslationDirectiveEnum m_dirTranslateVertical;

	HeadingDirectiveEnum m_dirHeading;
	UpDownDirectiveEnum m_dirUpDown;
	TiltDirectiveEnum m_dirTilt;

	double m_dblLastFrameCompleteSeconds;

	void AllocateResources();
	void ReleaseResources();
	void EstablishVirtualSpace();
	void SetInitialSceneState();
	void SetDefaultSceneState();
	void RenderScene();
	void OneTimeInitSceneObjects();
	void ApplyRotations();
	void DrawReferenceAxes();
	void DetermineFrameMotionDeltas(float& fltHeadingDelta, float& fltDeltaEyeAngle, float& fltDeltaTiltAngle, float& fltTranslateDelta);


public:
	Simulation(IWindowEventSource* ap_wes);
	~Simulation();
	void CommenceInputLoop();
	void TerminateLoop();
	void SetWindowCallback(IWindowEventSource* ap_wes);
	void OnExitCommand(UInt32 ui32Param);
	void OnKeyDown(UInt32 ui32Param);
	void OnKeyUp(UInt32 ui32Param);
	void OnChar(UInt32 ui32Param);
	void OnRightButtonDown();
	void OnLeftButtonDown();
	void OnLeftButtonDoubleClick();
	void OnWindowResize();
};