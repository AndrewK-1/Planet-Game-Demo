#pragma once
#include "pch.h"

class MouseInput {
public:
	void HandleInput(WPARAM wParam, LPARAM lParam);
private:
	int		xAbsolutePos;
	int		yAbsolutePos;
};