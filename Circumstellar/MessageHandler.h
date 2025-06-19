#pragma once
#include "pch.h"

class CustomWinMessageHandler {
public:
	POINT point;
	LRESULT processMessage(HWND hwnd, UINT ProcMSG, WPARAM wParam, LPARAM lParam);
};