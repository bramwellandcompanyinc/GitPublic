#include "StdAfx.h"
#include <Windowing/IWindowEventSource.h>
#include <Windowing/WindowIntermediary.h>

WindowIntermediary::WindowIntermediary(HWND a_hWnd)
{
	hWnd = a_hWnd;
}

void WindowIntermediary::DeferHandler()
{
}

void WindowIntermediary::YieldForEvents()
{
	MSG msg;
	if (PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void WindowIntermediary::GetWindowSize(UInt16& ui16Width, UInt16& ui16Height)
{
	RECT rc;
	::GetClientRect(hWnd, &rc);
	ui16Width = static_cast<UInt16>(rc.right - rc.left);
	ui16Height = static_cast<UInt16>(rc.bottom - rc.top);
}

void WindowIntermediary::EstablishWindow()
{

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		8,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	hdc = ::GetDC(hWnd);

	int pixelformat = ChoosePixelFormat(hdc, &pfd);
	BOOL boolRes = SetPixelFormat(hdc, pixelformat, &pfd);

	m_hrc = wglCreateContext(hdc);
	boolRes = wglMakeCurrent(hdc, m_hrc);

}

void WindowIntermediary::DestroyWindow()
{
	//::KillTimer(hWnd, 1);
	::wglMakeCurrent(NULL,  NULL);
	::wglDeleteContext(m_hrc);
	m_hrc = NULL;

}

void WindowIntermediary::StartRenderCycle()
{
	hdc = BeginPaint(hWnd, &ps);
}

void WindowIntermediary::CompleteRenderCycle()
{
	SwapBuffers(hdc);
	EndPaint(hWnd, &ps);
}