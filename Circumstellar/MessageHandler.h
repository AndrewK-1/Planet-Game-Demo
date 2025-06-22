#pragma once
#include "pch.h"
#include "InputController.h"

class CustomWinMessageHandler {
public:
	POINT point;
	LRESULT processMessage(HWND hwnd, UINT ProcMSG, WPARAM wParam, LPARAM lParam);
private:
	InputController inputController;
};