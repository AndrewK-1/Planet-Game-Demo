#pragma once
#include "pch.h"
#include "InputController.h"
#include <Debugapi.h>
#include "Camera.h"
#include <string>
#include "Game.h"

//Later, create a way to load input form an XML file into the class constructor

//bindingMap is a map containing a UINT key corresponding to a windows message, and an Action which is std::function<void()>

Game::Game() : cameraSpeed(0.01f) {
	//Note: VK_ is a prefix for some key codes.  All letters and numbers can be listed as '1' or 'A' instead of hexadecimal
	BindKey(0x20, BindHelper(&Game::MoveUp)); //VK_SPACE
	BindKey(0x43, BindHelper(&Game::MoveDown));	//C key
	BindKey(0x57, BindHelper(&Game::MoveForward));	//W key
	BindKey(0x53, BindHelper(&Game::MoveBackward));	//S key
	BindKey(0x44, BindHelper(&Game::MoveRight));	//D key
	BindKey(0x41, BindHelper(&Game::MoveLeft));	//A key
}

void Game::BindKey(UINT key, Action action) {
	bindingMap[key] = action;
}

void Game::HandleInput(UINT key, Game* game) {
	//If bindingMap.find doesn't find the key, it will return bindingMap.end()
	if (bindingMap.find(key) != bindingMap.end()) {
		bindingMap[key](game); //Call the method whose name is listed in the map at this key
	}
}

void Game::HandleRawInput(long x, long y, Game* game) {
	float speed = 0.01f;

	std::wstring outMessage = L"x: ";
	outMessage += std::to_wstring(static_cast<float>(x) * speed);
	outMessage += L" y: ";
	outMessage += std::to_wstring(static_cast<float>(y) * speed);
	OutputDebugString(outMessage.c_str());
	
	game->camera->Yaw(static_cast<float>(x) * speed);
	game->camera->Pitch(static_cast<float>(y) * speed);
}

void Game::MoveUp(Game* game) {
	game->camera->Up(cameraSpeed);
}
void Game::MoveDown(Game* game) {
	game->camera->Down(cameraSpeed);
}
void Game::MoveForward(Game* game) {
	game->camera->Forward(cameraSpeed);
}
void Game::MoveBackward(Game* game) {
	game->camera->Backward(cameraSpeed);
}
void Game::MoveRight(Game* game) {
	game->camera->Right(cameraSpeed);
}
void Game::MoveLeft(Game* game) {
	game->camera->Left(cameraSpeed);
}
