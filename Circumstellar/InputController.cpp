#pragma once
#include "pch.h"
#include "InputController.h"
#include <Debugapi.h>
#include "Camera.h"
#include <string>
#include "Game.h"


//Later, create a way to load input form an XML file into the class constructor

//bindingMap is a map containing a UINT key corresponding to a windows message, and an Action which is std::function<void()>

InputController::InputController() : m_cameraSpeed(0.1f), m_rollSpeed(0.05f), m_changePower(0.1f), m_wireframeOn(1)
{
	//Note: VK_ is a prefix for some key codes.  All letters and numbers can be listed as '1' or 'A' instead of hexadecimal
	BindKey(0x20, BindHelper(&InputController::MoveUp)); //VK_SPACE
	BindKey(0x43, BindHelper(&InputController::MoveDown));	//C key
	BindKey(0x57, BindHelper(&InputController::MoveForward));	//W key
	BindKey(0x53, BindHelper(&InputController::MoveBackward));	//S key
	BindKey(0x44, BindHelper(&InputController::MoveRight));	//D key
	BindKey(0x41, BindHelper(&InputController::MoveLeft));	//A key
	BindKey(0x0001, BindHelper(&InputController::UseTool)); //Left Mouse Button 0x0001
	BindKey(0x0002, BindHelper(&InputController::UseToolAlt)); //Right Mouse Button 0x010
	BindKey('1', BindHelper(&InputController::ChangeToToolOne)); //One
	BindKey('2', BindHelper(&InputController::ChangeToToolTwo)); //Two
	BindKey('3', BindHelper(&InputController::ChangeToToolThree)); //Two
	BindKey('Q', BindHelper(&InputController::RollCounterClockwise));
	BindKey('E', BindHelper(&InputController::RollClockwise));
	BindKey(VK_OEM_PERIOD, BindHelper(&InputController::DebugWireframe));
	BindKey(VK_SHIFT, BindHelper(&InputController::Sprint));

	BindKey('W', BindHelper(&InputController::PlayerForward));
	BindKey('S', BindHelper(&InputController::PlayerBackward));
	BindKey('A', BindHelper(&InputController::PlayerLeft));
	BindKey('D', BindHelper(&InputController::PlayerRight));
	BindKey(VK_SPACE, BindHelper(&InputController::PlayerUp));
	BindKey('C', BindHelper(&InputController::PlayerDown));
	BindKey('E', BindHelper(&InputController::PlayerRollClockwise));
	BindKey('Q', BindHelper(&InputController::PlayerRollCounterClockwise));
	BindKey(VK_RETURN, BindHelper(&InputController::PlayerMount));
}


void InputController::BindKey(UINT key, Action action) {
	bindingMap[key] = action, key;
}

void InputController::RemoveAllPressedKeys() {
	m_pressedKeys.clear();
}

void InputController::PressedKeysExecute(Game* game) {
	for (UINT key : m_pressedKeys) {
		//If bindingMap.find doesn't find the key, it will return bindingMap.end()
		if (bindingMap.find(key) != bindingMap.end()) {
			//Call the method whose name is listed in the map at this key. Return 1 if the method should only trigger on click.  Return 0 for holding the button.
			if (bindingMap[key](game) == 1) {
				m_pressedKeys.erase(key);
			}
		}
	}
}

void InputController::HandleKeyDown(UINT key, long long lParam, Game* game) {
	//If bindingMap.find doesn't find the key, it will return bindingMap.end()
	std::wstring msg;

	//Key flag becomes last 16 bits of lParam.  Useful since KF_REPEAT is 16-bit.
	int keyFlag = HIWORD(lParam);
	OutputDebugString(L"Key Pressed: ");  OutputDebugString(std::to_wstring(key).c_str());  OutputDebugString(L"\n");
	msg = L"Was Key already pressed? "; msg += std::to_wstring((keyFlag & KF_REPEAT) == KF_REPEAT); msg += L"\n"; OutputDebugString(msg.c_str());
	msg = L"lParam value: " + std::to_wstring(lParam); msg += L"\n"; OutputDebugString(msg.c_str());
	
	//If key was already pressed.  Prevent the array from being filled again until the button is released.
	//In subsequent action methods, return 1 if the action should only be executed on the first press, not while held down.
	//Otherwise, keys will remain in the key map and will continue to execute until the button is lifted up.
	if ((keyFlag & KF_REPEAT) != KF_REPEAT) { //KF_REPEAT is used for bit masking
		if (m_pressedKeys.find(key) == m_pressedKeys.end()) {
			m_pressedKeys.insert(key);
		}
	}
}

void InputController::HandleKeyUp(UINT key, Game* game) {
	m_pressedKeys.erase(key);
	if (key == VK_SHIFT) {
		StopSprinting(game);
	}
}

void InputController::HandleRawInput(long x, long y, Game* game) {
	float speed = 0.001f;
	
	if (x == 0) {
		//game->camera->Pitch(static_cast<float>(y) * speed);
		//game->GetWorld()->GetPlayer()->Pitch(static_cast<float>(y) * speed);
		Player* player = game->GetWorld()->GetPlayer();
		Spaceship* ship = player->GetMounted();
		if (player->IsMounted() == 0) {
			player->Pitch(static_cast<float>(y) * speed);
		}
		else {
			ship->Pitch(static_cast<float>(y) * speed);
		}
	}
	else if (y == 0) {
		//game->camera->Yaw(static_cast<float>(x) * speed);
		//game->GetWorld()->GetPlayer()->Yaw(static_cast<float>(x) * speed);
		Player* player = game->GetWorld()->GetPlayer();
		Spaceship* ship = player->GetMounted();
		if (player->IsMounted() == 0) {
			player->Yaw(static_cast<float>(x) * speed);
		}
		else {
			ship->Yaw(static_cast<float>(x) * speed);
		}
	}
	else {
		//game->camera->YawAndPitch(static_cast<float>(x) * speed, static_cast<float>(y) * speed);
		//game->GetWorld()->GetPlayer()->YawAndPitch(static_cast<float>(x) * speed, static_cast<float>(y) * speed);
		Player* player = game->GetWorld()->GetPlayer();
		Spaceship* ship = player->GetMounted();
		if (player->IsMounted() == 0) {
			player->YawAndPitch(static_cast<float>(x) * speed, static_cast<float>(y) * speed);
		}
		else {
			ship->YawAndPitch(static_cast<float>(x) * speed, static_cast<float>(y) * speed);
		}
	}
}

bool InputController::ChangeToToolOne(Game* game) {
	m_gameTool.SetCurrentTool(1);
	game->SetCurrentTool(1);
	return 1;
}
bool InputController::ChangeToToolTwo(Game* game) {
	m_gameTool.SetCurrentTool(2);
	game->SetCurrentTool(2);
	return 1;
}
bool InputController::ChangeToToolThree(Game* game) {
	m_gameTool.SetCurrentTool(3);
	game->SetCurrentTool(3);
	return 1;
}

bool InputController::UseTool(Game* game) {
	OutputDebugString(L"UseTool activated.\n");

	switch (m_gameTool.GetCurrentTool()) {
	case 1: {

		if (m_gameTool.ChangeTerrain(game->GetWorld()->GetPlanet(0), game->camera.get(), m_changePower)) {
			game->updatePlanetGeometryFlag = 1;
		}
		return 0;
	}
	case 2: {
		game->AddBlock();
		return 1;
	}
	case 3: {
		game->AddShip();
		return 1;
	}
	}
}

bool InputController::UseToolAlt(Game* game) {
	OutputDebugString(L"UseTool activated.\n");
	
	switch (m_gameTool.GetCurrentTool()) {
	case 1: {
		if (m_gameTool.ChangeTerrain(game->GetWorld()->GetPlanet(0), game->camera.get(), -m_changePower)) {
			game->updatePlanetGeometryFlag = 1;
		}
		return 0;
	}
	case 2: {
		game->RemoveBlock();
		return 1;
	}
	case 3: {
		game->RemoveShip();
		return 1;
	}
	}
}

bool InputController::RollClockwise(Game* game) {
	game->camera->Roll(-m_rollSpeed); return 0;
}
bool InputController::RollCounterClockwise(Game* game) {
	game->camera->Roll(m_rollSpeed); return 0;
}
bool InputController::MoveUp(Game* game) {
	game->camera->Up(m_cameraSpeed); return 0;
}
bool InputController::MoveDown(Game* game) {
	game->camera->Down(m_cameraSpeed); return 0;
}
bool InputController::MoveForward(Game* game) {
	game->camera->Forward(m_cameraSpeed); return 0;
}
bool InputController::MoveBackward(Game* game) {
	game->camera->Backward(m_cameraSpeed); return 0;
}
bool InputController::MoveRight(Game* game) {
	game->camera->Right(m_cameraSpeed); return 0;
}
bool InputController::MoveLeft(Game* game) {
	game->camera->Left(m_cameraSpeed); return 0;
}
bool InputController::Sprint(Game* game) {
	//m_cameraSpeed = 0.3f; return 1;
	game->GetWorld()->GetPlayer()->SetForcePower(1.0f); return 1;
}
bool InputController::StopSprinting(Game* game) {
	//m_cameraSpeed = 0.1f; return 1;
	game->GetWorld()->GetPlayer()->SetForcePower(0.5f); return 1;
}

bool InputController::DebugWireframe(Game* game) {
	if (m_wireframeOn == 0) {
		m_wireframeOn = 1;
		game->SetWireframe(1);

	}
	else {
		m_wireframeOn = 0;
		game->SetWireframe(0);
	}
	return 1;
}

bool InputController::PlayerUp(Game* game) {
	Player* player = game->GetWorld()->GetPlayer();
	Spaceship* ship = player->GetMounted();
	if (player->IsMounted() == 0) {
		player->AccelerateUp();
	}
	else {
		ship->AccelerateUp();
	}
	return 0;
}
bool InputController::PlayerDown(Game* game) {
	Player* player = game->GetWorld()->GetPlayer();
	Spaceship* ship = player->GetMounted();
	if (player->IsMounted() == 0) {
		player->AccelerateDown();
	}
	else {
		ship->AccelerateDown();
	}
	return 0;
}
bool InputController::PlayerLeft(Game* game) {
	Player* player = game->GetWorld()->GetPlayer();
	Spaceship* ship = player->GetMounted();
	if (player->IsMounted() == 0) {
		player->AccelerateLeft();
	}
	else {
		ship->AccelerateLeft();
	}
	return 0;
}
bool InputController::PlayerRight(Game* game) {
	Player* player = game->GetWorld()->GetPlayer();
	Spaceship* ship = player->GetMounted();
	if (player->IsMounted() == 0) {
		player->AccelerateRight();
	}
	else {
		ship->AccelerateRight();
	}
	return 0;
}
bool InputController::PlayerForward(Game* game) {
	Player* player = game->GetWorld()->GetPlayer();
	Spaceship* ship = player->GetMounted();
	if (player->IsMounted() == 0) {
		player->AccelerateForward();
	}
	else {
		ship->AccelerateForward();
	}
	return 0;
}
bool InputController::PlayerBackward(Game* game) {
	Player* player = game->GetWorld()->GetPlayer();
	Spaceship* ship = player->GetMounted();
	if (player->IsMounted() == 0) {
		player->AccelerateBackward();
	}
	else {
		ship->AccelerateBackward();
	}
	return 0;
}
bool InputController::PlayerRollClockwise(Game* game) {
	Player* player = game->GetWorld()->GetPlayer();
	Spaceship* ship = player->GetMounted();
	if (player->IsMounted() == 0) {
		player->Roll(-m_rollSpeed);
	}
	else {
		ship->Roll(-m_rollSpeed);
	}
	return 0;
}
bool InputController::PlayerRollCounterClockwise(Game* game) {
	Player* player = game->GetWorld()->GetPlayer();
	Spaceship* ship = player->GetMounted();
	if (player->IsMounted() == 0) {
		player->Roll(m_rollSpeed);
	}
	else {
		ship->Roll(m_rollSpeed);
	}
	return 0;
}
bool InputController::PlayerMount(Game* game) {
	if (game->GetWorld()->GetPlayer()->IsMounted() == 1) {
		game->GetWorld()->GetPlayer()->Unmount();
	}
	else {
		game->GetWorld()->BindPlayerPositionToNearestShip(game->GetWorld()->GetPlayer());
	}
	return 1;
}