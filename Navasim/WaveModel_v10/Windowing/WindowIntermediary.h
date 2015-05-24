#include <Windowing/IWindowEventSource.h>

class WindowIntermediary : public IWindowEventSource
{
private:
	#define MAX_LOADSTRING 100

	HWND hWnd;
	HDC hdc;
	RECT rc;
	HGLRC m_hrc;
	PAINTSTRUCT ps;

public:

	WindowIntermediary(HWND hWnd);
	void DeferHandler();
	void YieldForEvents();
	void EstablishWindow();
	void DestroyWindow();
	void GetWindowSize(UInt16& ui16Width, UInt16& ui16Height);
	void StartRenderCycle();
	void CompleteRenderCycle();
};
