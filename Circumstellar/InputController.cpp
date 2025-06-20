#pragma once
#include "pch.h"
#include "InputController.h"
#include <Debugapi.h>

//Later, create a way to load input form an XML file into the class constructor

//bindingMap is a map containing a UINT key corresponding to a windows message, and an Action which is std::function<void()>

InputController::InputController() {
	BindKey(0x100, Jump); //VK_SPACE
}

void InputController::BindKey(UINT key, Action action) {
	bindingMap[key] = action;
}

void InputController::HandleInput(UINT key) {
	//If bindingMap.find doesn't find the key, it will return bindingMap.end()
	if (bindingMap.find(key) != bindingMap.end()) {
		bindingMap[key](); //Call the method whose name is listed in the map at this key
	}
}

void Jump() {
	OutputDebugString(L"Jump");
}