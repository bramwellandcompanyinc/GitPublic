#ifndef iwindoweventsource_h
#define iwindoweventsource_h

#include "GenericConstants.h"

class IWindowEventSource
{
public:
	virtual void DeferHandler() = 0;
	virtual void YieldForEvents() = 0;
	virtual void EstablishWindow() = 0;
	virtual void DestroyWindow() = 0;
	virtual void GetWindowSize(UInt16& ui16Width, UInt16& ui16Height) = 0;
	virtual void StartRenderCycle() = 0;
	virtual void CompleteRenderCycle() = 0;
};

#endif