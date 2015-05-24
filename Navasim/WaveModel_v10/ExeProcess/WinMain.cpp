#include "stdafx.h"
#include "resource.h"
#include <Simulation/Simulation.h>
#include <Windowing/IWindowEventSource.h>
#include <Windowing/WindowIntermediary.h>
#include <Windowing/IWindowEventHandler.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#define MAX_LOADSTRING 100
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

IWindowEventSource* p_win = 0;
IWindowEventHandler* p_wehSimulation = 0;

// Forward declarations of functions included in this code module:

ATOM MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WindowMessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	int intRetVal = FALSE;

	// Initialize global strings

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GL_1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	MSG msg;
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GL_1);

    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0,0, 616, 636, NULL, NULL, hInstance, NULL);

	
/*
	//Set this thread to run on a single processor, for experiment

	DWORD_PTR dwResult;
	DWORD dwMask = 0x01;
	HANDLE hThread = GetCurrentThread();
	dwResult = SetThreadAffinityMask(hThread, dwMask);
	if (dwResult == 0)
	{
		LPCWSTR strError = L"Error";
		LPCWSTR strMsg = L"Thread affinity could not be set";
		::MessageBox(NULL, strMsg, strError, 0);
	}
*/

	if (hWnd)
	{
		p_win = (IWindowEventSource*)(new WindowIntermediary(hWnd));
		p_win->EstablishWindow();

		p_wehSimulation = (IWindowEventHandler*)(new Simulation(p_win));

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		//SetFocus(hWnd);

		p_wehSimulation->CommenceInputLoop();

		delete p_wehSimulation;
		delete p_win;

		intRetVal = TRUE;
	}

	return intRetVal;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WindowMessageHandler;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GL_1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject (NULL_BRUSH);
	wcex.lpszMenuName	= NULL;//(LPCTSTR)IDC_GL_1;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK WindowMessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	UInt32 ui32Param = static_cast<UInt32>(wParam);

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
//		DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			p_wehSimulation->TerminateLoop();
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		
	case WM_KEYDOWN:
		p_wehSimulation->OnKeyDown(ui32Param);
		break;
	case WM_KEYUP:
		p_wehSimulation->OnKeyUp(ui32Param);
		break;
	case WM_CHAR:
		p_wehSimulation->OnChar(ui32Param);
		break;
	case WM_PAINT:
//		drawit();
		break;
	case WM_DESTROY:
		p_win->DestroyWindow();
		PostQuitMessage(0);
		break;
	case WM_RBUTTONDOWN:
		p_wehSimulation->OnRightButtonDown();
		break;
	case WM_LBUTTONDOWN:
		p_wehSimulation->OnLeftButtonDown();
		break;
	case WM_LBUTTONDBLCLK:
		p_wehSimulation->OnLeftButtonDoubleClick();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}