#pragma once
#include "pch.h"
#include <windowsx.h>
#include "MessageHandler.h"
#include "Game.h"
#include "InputController.h"

InputController inputController;

	LRESULT CustomWinMessageHandler::processMessage(HWND hwnd, UINT ProcMSG, WPARAM wParam, LPARAM lParam) {
	switch (ProcMSG) {
	case WM_LBUTTONDOWN: {
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		return 0;
	}
	case WM_KEYDOWN: {
		inputController.HandleInput(ProcMSG);
		return 0;
	}

	case WM_CLOSE: {
		DestroyWindow(hwnd);
		return 0;
	}
	
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}

	default: {
		return DefWindowProc(hwnd, ProcMSG, wParam, lParam);
	}

	return 0;
	}
}