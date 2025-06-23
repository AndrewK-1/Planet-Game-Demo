#pragma once
#include "pch.h"
#include "InputController.h"
#include "Game.h"

class CustomWinMessageHandler {
public:
	CustomWinMessageHandler();
	LRESULT processMessage(HWND hwnd, UINT ProcMSG, WPARAM wParam, LPARAM lParam, Game* game);
	Game* getInputController();
private:
	POINT point;
};