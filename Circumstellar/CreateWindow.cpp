#include <Windows.h>
#include <windowsx.h>
#include <chrono>
#include <thread>
#include <iostream>


LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

//WINAPI is a kind of macro that includes a bunch of necessary stuff for wWinMain to work.
//wWinMain is the Unicode wchar_t version of WinMain, an dso the program enables usage of Unicode.
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPTSTR lpCmdLine, 
	_In_ int nCmdShow
	)
{
	//To create a window: define a window class, register it, and create the window with CreateWindow or CreateWindowEx functions

	//Defining the Window Class
	HINSTANCE handle = hInstance;
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_VREDRAW | CS_HREDRAW;
	wcx.lpfnWndProc = MainWndProc;
	wcx.hInstance = hInstance;
	wcx.lpszClassName = L"DXGameWindow";
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hIcon = NULL;
	wcx.hCursor = NULL;
	wcx.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcx.lpszMenuName = NULL;
	wcx.hIconSm = NULL;
	
	ATOM RegisteredWindowClass = RegisterClassExW(&wcx);

	HWND hWind = CreateWindowExW(
		WS_EX_CLIENTEDGE, 
		MAKEINTATOM(RegisteredWindowClass), 
		L"Circumstellar", 
		WS_CAPTION |  WS_OVERLAPPEDWINDOW | WS_HSCROLL, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	BOOL shown = ShowWindow(hWind, nCmdShow);
	UpdateWindow(hWind);

	
	/*In Get Message, parameter 1 is pointing to the MSG structure receiving messages.
	Parameter 2 is a handle to the window whose messages are to be retrieved, can be null to retrieve any messages on the current thread.
	Parameter 3 specifies keyboard and mouse messages, more specifically, the "lowest" message value to be retrieved.
		Can also use WM_INPUT for WM_INPUT messages.  Zero means no filtering.
	Parameter 4 is "highest" value to be retrieved.  similar to previous parameter.*/
	MSG message;
	while (((GetMessage(&message, (HWND)NULL, 0, 0))) != 0) 
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

LRESULT CALLBACK MainWndProc(
	_In_ HWND hwnd, 
	_In_ UINT ProcMSG, 
	_In_ WPARAM wParam, 
	_In_ LPARAM lParam) {
	POINT point;
	
	switch (ProcMSG) {
	case WM_LBUTTONDOWN: {
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
	}
	break;

		case WM_CLOSE: {
			DestroyWindow(hwnd);
		}

		case WM_DESTROY: {
			PostQuitMessage(0);
		}

		default:{
			return DefWindowProc(hwnd, ProcMSG, wParam, lParam);
		}

	return 0;
	}
}