#pragma once
#include <Windows.h>
class CustomWinMessageHandler {
public:
	LRESULT processMessage(HWND hwnd, UINT ProcMSG, WPARAM wParam, LPARAM lParam);
};