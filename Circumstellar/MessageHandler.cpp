#pragma once
#include "pch.h"
#include <windowsx.h>
#include "MessageHandler.h"
#include "InputController.h"
#include "ErrorHandling.h"
namespace {
	std::unique_ptr<InputController> inputController;
}

CustomWinMessageHandler::CustomWinMessageHandler() :
	point({ 0, 0 })
{
	inputController = std::make_unique<InputController>();

	RAWINPUTDEVICE RID[1];
	RID[0] = {};
	RID[0].usUsagePage = 0x01;
	RID[0].usUsage = 0x02;
	RID[0].hwndTarget = NULL;
	RID[0].dwFlags = 0;
	if (RegisterRawInputDevices(RID, 1, sizeof(RAWINPUTDEVICE)) == FALSE) 
	{
		ErrorHandler error;
		error.DisplayError();
	}
}

LRESULT CustomWinMessageHandler::processMessage(HWND hwnd, UINT ProcMSG, WPARAM wParam, LPARAM lParam, Game* game) {
	switch (ProcMSG) {
	case WM_INPUT: {
		UINT miDataSize;	//Size of data from: (implicitely due to raw input flags set) mouse input
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &miDataSize, sizeof(RAWINPUTHEADER));

		if (miDataSize > 0) {
			std::unique_ptr<BYTE[]> pByteSize = std::make_unique<BYTE[]>(miDataSize);
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, pByteSize.get(), &miDataSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* rawInput = reinterpret_cast<RAWINPUT*>(pByteSize.get());

			if (rawInput->header.dwType == RIM_TYPEMOUSE) {
				rawInput->data.mouse.usFlags = MOUSE_MOVE_RELATIVE;
				inputController->HandleRawInput(rawInput->data.mouse.lLastX, rawInput->data.mouse.lLastY, game);
			}
			else {
				OutputDebugString(L"Not a mouse");
			}
			
		}
		return 0;
	}

	case WM_LBUTTONDOWN: {
		inputController->HandleKeyDown(static_cast<UINT>(0x0001), static_cast<long long>(lParam), game);
		return 0;
	}
	case WM_LBUTTONUP: {
		inputController->HandleKeyUp(static_cast<UINT>(0x0001), game);
		return 0;
	}
	case WM_RBUTTONDOWN: {
		inputController->HandleKeyDown(static_cast<UINT>(0x0002), static_cast<long long>(lParam), game);
		return 0;
	}
	case WM_RBUTTONUP: {
		inputController->HandleKeyUp(static_cast<UINT>(0x0002), game);
		return 0;
	}

	case WM_KEYDOWN: {
		inputController->HandleKeyDown(static_cast<UINT>(wParam), static_cast<long long>(lParam), game);
		return 0;
	}
	case WM_KEYUP: {
		inputController->HandleKeyUp(static_cast<UINT>(wParam), game);
		return 0;
	}

	case WM_ACTIVATE: {
		if (wParam == 0) {
			game->OnDeactivated();
			inputController->RemoveAllPressedKeys();
		}
		else {
			game->OnActivated();
		}
		break;
	}

	case WM_CLOSE: {
		game->SaveWorld();
		ClipCursor(NULL);
		ShowCursor(true);
		DestroyWindow(hwnd);
		return 0;
	}
	
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}

	case WM_SIZE: {
		OutputDebugString(L"Window size changed.\n");
		std::wstring msg = L"Window message wParam" + std::to_wstring(wParam); msg += L"\n"; OutputDebugString(msg.c_str());
		if (wParam == SIZE_MAXIMIZED) {
			int width, height;
			game->GetDefaultSize(width, height);
			game->OnWindowSizeChanged(width, height);
		}
		else {
			game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
		}
		break;
	}

	default: {
		return DefWindowProc(hwnd, ProcMSG, wParam, lParam);
	}

	return 0;
	}
}

InputController* CustomWinMessageHandler::getInputController() {
	return inputController.get();
}