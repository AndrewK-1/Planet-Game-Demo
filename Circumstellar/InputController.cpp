#pragma once
#include "pch.h"
#include "InputController.h"
#include <Debugapi.h>
#include "Camera.h"
#include <string>
#include "Game.h"

//Later, create a way to load input form an XML file into the class constructor

//bindingMap is a map containing a UINT key corresponding to a windows message, and an Action which is std::function<void()>

InputController::InputController() : m_cameraSpeed(0.1f), m_rollSpeed(0.05f), m_changePower(0.1f), m_wireframeOn(0), m_menuContextEnabled(1),
	m_lastMouseClickPosH(0), m_lastMouseClickPosV(0), m_keyChangeInputContextEnabled(0)
{
	//Note: VK_ is a prefix for some key codes.  All letters and numbers can be listed as '1' or 'A' as a char instead of hexadecimal	
	OutputDebugString(L"InputController: Receiving Keybind settings.\n");
	//InitializeSetting will use the file setting if available, otherwise it will add the setting to the file with the default value.
	InitializeGameBindSetting(&InputController::PlayerForward, ID_PlayerForward, 'W');
	InitializeGameBindSetting(&InputController::PlayerBackward, ID_PlayerBackward, 'S');
	InitializeGameBindSetting(&InputController::PlayerLeft, ID_PlayerLeft, 'A');
	InitializeGameBindSetting(&InputController::PlayerRight, ID_PlayerRight, 'D');
	InitializeGameBindSetting(&InputController::PlayerUp, ID_PlayerUp, VK_SPACE);
	InitializeGameBindSetting(&InputController::PlayerDown, ID_PlayerDown, 'C');
	InitializeGameBindSetting(&InputController::UseTool, ID_UseTool, 0x001);
	InitializeGameBindSetting(&InputController::UseToolAlt, ID_UseToolAlt, 0x002);
	InitializeGameBindSetting(&InputController::ChangeToToolOne, ID_ChangeToToolOne, '1');
	InitializeGameBindSetting(&InputController::ChangeToToolTwo, ID_ChangeToToolTwo, '2');
	InitializeGameBindSetting(&InputController::ChangeToToolThree, ID_ChangeToToolThree, '3');
	InitializeGameBindSetting(&InputController::ChangeToToolOff, ID_ChangeToToolOff, VK_OEM_3);
	InitializeGameBindSetting(&InputController::DebugWireframe, ID_DebugWireframe, VK_OEM_PERIOD);
	InitializeGameBindSetting(&InputController::Sprint, ID_Sprint, VK_SHIFT);
	InitializeGameBindSetting(&InputController::PlayerRollClockwise, ID_PlayerRollClockwise, 'E');
	InitializeGameBindSetting(&InputController::PlayerRollCounterClockwise, ID_PlayerRollCounterClockwise, 'Q');
	InitializeGameBindSetting(&InputController::PlayerMount, ID_PlayerMount, VK_RETURN);
	InitializeGameBindSetting(&InputController::InGameMenu, ID_GameMenu, VK_ESCAPE);

	//Binding permanent menu actions.  Not adjustable by the user.
	BindMenuKey(0x01, BindHelper(&InputController::MenuLeftClick));
	BindMenuKey(VK_ESCAPE, BindHelper(&InputController::MenuClosing));
}



void InputController::BindGameKey(UINT key, Action action) {
	bindingMap[key] = action, key;
}

void InputController::BindMenuKey(UINT key, Action action) {
	menuBindingMap[key] = action;
}

void InputController::SetGameBindSetting(UINT oldKey, UINT newKey, UINT settingID) {
	Action action = bindingMap[oldKey];
	bindingMap.erase(oldKey);
	BindGameKey(newKey, action);
	m_controlSettingsIO.SetSetting(m_controlSettingsFileName, settingID, newKey);
}

template<typename T>
void InputController::InitializeGameBindSetting(T methodName, UINT settingID, UINT settingValue) {
	UINT setting;
	if (m_controlSettingsIO.GetSetting(m_controlSettingsFileName, settingID, setting)) {
		OutputDebugString(L"InputController: Setting found, using file value.\n");
		BindGameKey(setting, BindHelper(methodName));
	}
	else {
		OutputDebugString(L"InputController: Setting not found, setting new value.\n");
		m_controlSettingsIO.SetSetting(m_controlSettingsFileName, settingID, settingValue);
		BindGameKey(settingValue, BindHelper(methodName));
	}
}

UINT InputController::GetGameBindSetting(UINT settingID) {
	UINT setting;
	m_controlSettingsIO.GetSetting(m_controlSettingsFileName, settingID, setting);
	return setting;
}

void InputController::RemoveAllPressedKeys() {
	m_pressedKeys.clear();
}

void InputController::PrepareKeybindChange(int buttonIndex, UINT settingID) {
	std::wstring msg = L"InputController: Preparing to change keybind for SettingID: " + std::to_wstring(settingID) + L"\n";
	OutputDebugString(msg.c_str());
	m_keyChangeInputContextEnabled = true;
	m_nextKeyToChangeSettingID = settingID;
	m_buttonChangeIndex = buttonIndex;
}

void InputController::PressedKeysExecute(Game* game) {
	std::vector<UINT> keysToErase;
	for (UINT key : m_pressedKeys) {
		if (m_menuContextEnabled == 1) {
			if (menuBindingMap.find(key) != menuBindingMap.end()) {
				if (menuBindingMap[key](game) == 1) {
					keysToErase.push_back(key);
				}
			}
		}
		else {
			//If bindingMap.find doesn't find the key, it will return bindingMap.end()
			if (bindingMap.find(key) != bindingMap.end()) {
				//Call the method whose name is listed in the map at this key. Return 1 if the method should only trigger on click.  Return 0 for holding the button.
				if (bindingMap[key](game) == 1) {
					keysToErase.push_back(key);
				}
			}
		}
	}
	for(int i = 0; i < keysToErase.size(); i++) {
		m_pressedKeys.erase(keysToErase[i]);
	}
}

void InputController::HandleKeyDown(UINT key, long long lParam, Game* game) {
	//If bindingMap.find doesn't find the key, it will return bindingMap.end()
	std::wstring msg;

	if (m_keyChangeInputContextEnabled == true) {
		UINT oldKey = GetGameBindSetting(m_nextKeyToChangeSettingID);
		std::wstring msg = L"InputController: Changing keybind for SettingID: " + std::to_wstring(m_nextKeyToChangeSettingID) + L" from " + std::to_wstring(oldKey) + L" to " + std::to_wstring(key) + L"\n";
		OutputDebugString(msg.c_str());
		SetGameBindSetting(oldKey, key, m_nextKeyToChangeSettingID);
		m_keyChangeInputContextEnabled = false;
		game->CloseTopmostMenu();
	}

	//Key flag becomes last 16 bits of lParam.  Useful since KF_REPEAT is 16-bit.
	int keyFlag = HIWORD(lParam);
	OutputDebugString(L"Key Pressed: ");  OutputDebugString(std::to_wstring(key).c_str());  OutputDebugString(L"\n");
	msg = L"Was Key already pressed? "; msg += std::to_wstring((keyFlag & KF_REPEAT) == KF_REPEAT); msg += L"\n"; OutputDebugString(msg.c_str());
	msg = L"lParam value: " + std::to_wstring(lParam); msg += L"\n"; OutputDebugString(msg.c_str());
	
	if (key == 1) {
		m_lastMouseClickPosH = LOWORD(lParam);
		m_lastMouseClickPosV = HIWORD(lParam);
	}

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
	if (m_menuContextEnabled == 0) {
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
bool InputController::ChangeToToolOff(Game* game) {
	m_gameTool.SetCurrentTool(0);
	game->SetCurrentTool(0);
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
	case 0: {
		return 1; //No tool, do nothing.
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
bool InputController::InGameMenu(Game* game) {
	if (m_menuContextEnabled == 0) {
		game->OpenInGameMenu();
		OutputDebugString(L"Opening game menu\n");
	}
	m_menuContextEnabled = game->CheckMenuState();
	return 1;
}

//Menu actions below this point
bool InputController::MenuLeftClick(Game* game) {
	game->CheckMenuClick(m_lastMouseClickPosH, m_lastMouseClickPosV);
	std::wstring msg = L"Menu clicked.  Mouse position: "; msg += std::to_wstring(m_lastMouseClickPosH); msg += L", "; msg += std::to_wstring(m_lastMouseClickPosV); msg += L"\n";
	OutputDebugString(msg.c_str());
	m_menuContextEnabled = game->CheckMenuState();
	if(m_menuContextEnabled == 0) {
		OutputDebugString(L"Switching to game controls.\n");
	}
	else {
		OutputDebugString(L"Switching to menu controls.\n");		
	}
	return 1;
}
bool InputController::MenuClosing(Game* game) {
	game->CloseMenus();
	m_menuContextEnabled = game->CheckMenuState();
	return 1;
}