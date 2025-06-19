#pragma once
#include "pch.h"
#include <windowsx.h>
#include "MessageHandler.h"
#include "Game.h"


	LRESULT CustomWinMessageHandler::processMessage(HWND hwnd, UINT ProcMSG, WPARAM wParam, LPARAM lParam) {
	switch (ProcMSG) {
	case WM_LBUTTONDOWN: {
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		return 0;
	}
	break;

	//Test right mouse click to destroy window
	case WM_RBUTTONDOWN: {
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