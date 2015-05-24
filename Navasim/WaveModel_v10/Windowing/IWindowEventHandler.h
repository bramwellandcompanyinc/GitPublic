#ifndef iwindoweventhandler_h
#define iwindoweventhandler_h

#include "GenericConstants.h"
#include <Windowing/IWindowEventSource.h>

class IWindowEventHandler
{
public:
	virtual void CommenceInputLoop() = 0;
	virtual void TerminateLoop() = 0;
	virtual void SetWindowCallback(IWindowEventSource* ap_wes) = 0;
	virtual void OnExitCommand(UInt32 ui32Param) = 0;
	virtual void OnKeyDown(UInt32 ui32Param) = 0;
	virtual void OnKeyUp(UInt32 ui32Param) = 0;
	virtual void OnChar(UInt32 ui32Param) = 0;
	virtual void OnRightButtonDown() = 0;
	virtual void OnLeftButtonDown() = 0;
	virtual void OnLeftButtonDoubleClick() = 0;
	virtual void OnWindowResize() = 0;
};

#endif