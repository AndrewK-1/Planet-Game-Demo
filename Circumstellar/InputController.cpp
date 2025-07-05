#pragma once
#include "pch.h"
#include "InputController.h"
#include <Debugapi.h>
#include "Camera.h"
#include <string>
#include "Game.h"


//Later, create a way to load input form an XML file into the class constructor

//bindingMap is a map containing a UINT key corresponding to a windows message, and an Action which is std::function<void()>

InputController::InputController() : cameraSpeed(0.1f), rollSpeed(0.05f), m_changePower(0.1f) {
	//Note: VK_ is a prefix for some key codes.  All letters and numbers can be listed as '1' or 'A' instead of hexadecimal
	BindKey(0x20, BindHelper(&InputController::MoveUp)); //VK_SPACE
	BindKey(0x43, BindHelper(&InputController::MoveDown));	//C key
	BindKey(0x57, BindHelper(&InputController::MoveForward));	//W key
	BindKey(0x53, BindHelper(&InputController::MoveBackward));	//S key
	BindKey(0x44, BindHelper(&InputController::MoveRight));	//D key
	BindKey(0x41, BindHelper(&InputController::MoveLeft));	//A key
	BindKey(0x0001, BindHelper(&InputController::UseTool)); //Left Mouse Button 0x0001
	BindKey(0x0002, BindHelper(&InputController::UseToolAlt)); //Right Mouse Button 0x010
	BindKey('Q', BindHelper(&InputController::RollCounterClockwise));
	BindKey('E', BindHelper(&InputController::RollClockwise));
}

void InputController::UseTool(Game* game) {
	OutputDebugString(L"UseTool activated.\n");
	if (m_gameTool.ChangeTerrain(game->m_planet1.get(), game->camera.get(), m_changePower)) {
		game->updatePlanetGeometryFlag = 1;
	}
}
void InputController::UseToolAlt(Game* game) {
	OutputDebugString(L"UseTool activated.\n");
	if (m_gameTool.ChangeTerrain(game->m_planet1.get(), game->camera.get(), -m_changePower)) {
		game->updatePlanetGeometryFlag = 1;
	}
}

void InputController::BindKey(UINT key, Action action) {
	bindingMap[key] = action;
}

void InputController::PressedKeysExecute(Game* game) {
	for (UINT key : m_pressedKeys) {
		//If bindingMap.find doesn't find the key, it will return bindingMap.end()
		if (bindingMap.find(key) != bindingMap.end()) {
			bindingMap[key](game); //Call the method whose name is listed in the map at this key
		}
	}
}

void InputController::HandleKeyDown(UINT key, Game* game) {
	//If bindingMap.find doesn't find the key, it will return bindingMap.end()
	if (m_pressedKeys.find(key) == m_pressedKeys.end()) {
		m_pressedKeys.insert(key);
	}
}

void InputController::HandleKeyUp(UINT key, Game* game) {
	m_pressedKeys.erase(key);
}

void InputController::HandleRawInput(long x, long y, Game* game) {
	float speed = 0.001f;
	
	if (x == 0) {
		game->camera->Pitch(static_cast<float>(y) * speed);
	}
	else if (y == 0) {
		game->camera->Yaw(static_cast<float>(x) * speed);
	}
	else {
		game->camera->YawAndPitch(static_cast<float>(x) * speed, static_cast<float>(y) * speed);
	}
}

void InputController::RollClockwise(Game* game) {
	game->camera->Roll(-rollSpeed);
}
void InputController::RollCounterClockwise(Game* game) {
	game->camera->Roll(rollSpeed);
}
void InputController::MoveUp(Game* game) {
	game->camera->Up(cameraSpeed);
}
void InputController::MoveDown(Game* game) {
	game->camera->Down(cameraSpeed);
}
void InputController::MoveForward(Game* game) {
	game->camera->Forward(cameraSpeed);
}
void InputController::MoveBackward(Game* game) {
	game->camera->Backward(cameraSpeed);
}
void InputController::MoveRight(Game* game) {
	game->camera->Right(cameraSpeed);
}
void InputController::MoveLeft(Game* game) {
	game->camera->Left(cameraSpeed);
}