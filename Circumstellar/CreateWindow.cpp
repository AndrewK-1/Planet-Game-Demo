#pragma once
#include "pch.h"
#include <Windows.h>
#include "CreateWindow.h"
#include "MessageHandler.h"
#include "Game.h"

namespace 
{
	std::unique_ptr<Game> game;
}

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
	//Defining game object
	game = std::make_unique<Game>();
	//Call to custom function to define the window class
	WNDCLASSEX wClassX = WindowDefine(handle);
	
	//Registering the Window Class
	ATOM RegisteredWindowClass;	
	if (!(RegisteredWindowClass = RegisterClassExW(&wClassX))) {
		return 1; //Error registering window class
	}

	//Creating the default window size
	int sWidth, sHeight;
	game->GetDefaultSize(sWidth, sHeight);
	//Rectangle object can be resized depending on window styles to get the right dimensions for that style
	RECT screenRect{ 0, 0, static_cast<LONG>(sWidth), static_cast<LONG>(sHeight)};
	//Adjusting for window style
	AdjustWindowRect(&screenRect, WS_OVERLAPPEDWINDOW, FALSE);

	//Creating the window
	HWND hWind = CreateWindowExW(
		WS_EX_CLIENTEDGE, 
		MAKEINTATOM(RegisteredWindowClass), 
		L"Circumstellar", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		screenRect.right - screenRect.left,
		screenRect.bottom - screenRect.top,
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	//Showing the window
	ShowWindow(hWind, nCmdShow);
	UpdateWindow(hWind);

	game->Initialize(hWind);

	/*In Get Message, parameter 1 is pointing to the MSG structure receiving messages.
	Parameter 2 is a handle to the window whose messages are to be retrieved, can be null to retrieve any messages on the current thread.
	Parameter 3 specifies keyboard and mouse messages, more specifically, the "lowest" message value to be retrieved.
		Can also use WM_INPUT for WM_INPUT messages.  Zero means no filtering.
	Parameter 4 is "highest" value to be retrieved.  similar to previous parameter.*/
	MSG message = {};
	while (WM_QUIT != message.message) 
	{
		//??PeekMessage is clearing this 'if' statement to the 'else' after a few passes, but the alternative, GetMessage(), is not
		if (PeekMessage(&message, (HWND)NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else 
		{
			game->Tick();
		}
		
	}
	game->OnClosing();
	return 0;
}

LRESULT CALLBACK MainWndProc(
	_In_ HWND hwnd, 
	_In_ UINT ProcMSG, 
	_In_ WPARAM wParam, 
	_In_ LPARAM lParam) {

	CustomWinMessageHandler mHandler;
	return mHandler.processMessage(hwnd, ProcMSG, wParam, lParam);
}

WNDCLASSEX WindowDefine(HINSTANCE hI) {
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_VREDRAW | CS_HREDRAW;
	wcx.lpfnWndProc = MainWndProc;
	wcx.hInstance = hI;
	wcx.lpszClassName = L"DXGameWindow";
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hIcon = NULL;
	wcx.hCursor = NULL;
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW+2);
	wcx.lpszMenuName = NULL;
	wcx.hIconSm = NULL;
	return wcx;

}

