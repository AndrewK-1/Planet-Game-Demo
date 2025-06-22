#pragma once
#include "pch.h"
#include "MessageHandler.h"
#include <Windows.h>

LRESULT CALLBACK MainWndProc(
	_In_ HWND,
	_In_ UINT,
	_In_ WPARAM,
	_In_ LPARAM);
WNDCLASSEX WindowDefine(HINSTANCE);
extern CustomWinMessageHandler mHandler;