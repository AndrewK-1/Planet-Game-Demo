#pragma once
#include "pch.h"
#include <map>
#include <functional>


//Function alias Action that takes no parameters and returns void.
//When Action is used from here on, it means std::function<void()>.
using Action = std::function<void()>;

//Binding structure for mapping
struct InputBinding {
	UINT key;
	Action action;
};

class InputController {
public:
	InputController();
	void BindKey(UINT key, Action action);

	void HandleInput(UINT key);
private:
	std::map<UINT, Action> bindingMap;
};

void Jump();