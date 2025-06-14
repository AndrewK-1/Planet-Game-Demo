#pragma once
#include "pch.h"
#include "MessageHandler.h"
	LRESULT CustomWinMessageHandler::processMessage(HWND hwnd, UINT ProcMSG, WPARAM wParam, LPARAM lParam) {
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

	default: {
		return DefWindowProc(hwnd, ProcMSG, wParam, lParam);
	}

	return 0;
	}
}