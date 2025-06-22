#include "pch.h"
#include "MouseInput.h"

void MouseInput::HandleInput(WPARAM wParam, LPARAM lParam) {
	xAbsolutePos = GET_X_LPARAM(lParam);
	yAbsolutePos = GET_Y_LPARAM(lParam);
}