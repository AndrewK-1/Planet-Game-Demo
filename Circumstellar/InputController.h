#pragma once
#include "pch.h"
#include <map>
#include <functional>
#include "Camera.h"
#include "Game.h"
#include <unordered_set>
#include "GameTool.h"
#include "ControlSettingsIO.h"

//Function alias Action that takes no parameters and returns void.
//When Action is used from here on, it means std::function<void()>.
using Action = std::function<bool(Game*)>;

//Binding structure for mapping
struct InputBinding {
	UINT key;
	Action action;
};

class InputController {
public:
	InputController();
	void RemoveAllPressedKeys();
	void BindGameKey(UINT key, Action action);
	void BindMenuKey(UINT key, Action action);
	void HandleKeyDown(UINT key, long long lParam, Game* game);
	void HandleKeyUp(UINT key, Game* game);
	void PressedKeysExecute(Game* game);
	void HandleRawInput(long x, long y, Game* game);
	
	//Helper to improve readability of source binds while moving callable methods to the class
	template<typename T>
	Action BindHelper(T methodName) {
		return [this, methodName](Game* game) -> bool{
			return (this->*methodName)(game); };
	}
	//Helper to make generic the retrieval or setting of control settings in regards to a file.
	template<typename T>
	void InitializeGameBindSetting(T methodName, UINT settingID, UINT settingValue);

	template<typename T>
	void InitializeMenuBindSetting(T methodName, UINT settingID, UINT settingValue);

private:
	std::map<UINT, Action> bindingMap;	//Handles binding virtual keys to in-game actions
	std::map<UINT, Action> menuBindingMap;	//A binding map for menu actions.  These don't need to be rebound.
	bool m_wireframeButtonStillPressed;
	bool m_wireframeOn;
	bool m_menuContextEnabled;
	float m_cameraSpeed;
	float m_rollSpeed;
	std::unordered_set<UINT> m_pressedKeys;
	float m_changePower;
	GameTool m_gameTool;
	ControlSettingsIO m_controlSettingsIO;
	std::string m_controlSettingsFileName = "ControlSettings.txt";

	bool DebugWireframe(Game* game);
	bool UseTool(Game* game);
	bool UseToolAlt(Game* game);
	bool Sprint(Game* game);
	bool StopSprinting(Game* game);
	bool ChangeToToolOne(Game* game);
	bool ChangeToToolTwo(Game* game);
	bool ChangeToToolThree(Game* game);
	bool PlayerUp(Game* game);
	bool PlayerDown(Game* game);
	bool PlayerLeft(Game* game);
	bool PlayerRight(Game* game);
	bool PlayerForward(Game* game);
	bool PlayerBackward(Game* game);
	bool PlayerRollClockwise(Game* game);
	bool PlayerRollCounterClockwise(Game* game);
	bool PlayerMount(Game* game);
	bool InGameMenu(Game* game);

	bool MenuLeftClick(Game* game);
	bool MenuClosing(Game* game);

	//Enumerator for the setting IDs used in the binary settings file.
	enum SettingIDs {
		ID_PlayerUp = 0x01,
		ID_PlayerDown = 0x02,
		ID_PlayerLeft = 0x03,
		ID_PlayerRight = 0x04,
		ID_PlayerForward = 0x05,
		ID_PlayerBackward = 0x06,
		ID_PlayerRollClockwise = 0x07,
		ID_PlayerRollCounterClockwise = 0x08,
		ID_PlayerMount = 0x09,
		ID_UseTool = 0x0A,
		ID_UseToolAlt = 0x0B,
		ID_Sprint = 0x0C,
		ID_StopSprinting = 0x0D,
		ID_ChangeToToolOne = 0x0E,
		ID_ChangeToToolTwo = 0x0F,
		ID_ChangeToToolThree = 0x10,
		ID_ChangeToToolOff = 0x11,
		ID_DebugWireframe = 0x12,
		ID_GameMenu = 0x13
	};

	int m_lastMouseClickPosH;
	int m_lastMouseClickPosV;
};