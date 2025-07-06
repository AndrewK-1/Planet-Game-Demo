#pragma once
#include "pch.h"
#include <map>
#include <functional>
#include "Camera.h"
#include "Game.h"
#include <unordered_set>
#include "GameTool.h"

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
	void BindKey(UINT key, Action action);
	void HandleKeyDown(UINT key, long long lParam, Game* game);
	void HandleKeyUp(UINT key, Game* game);
	void PressedKeysExecute(Game* game);
	void HandleRawInput(long x, long y, Game* game);
	
	void ChangeCameraSpeed(float speed);
	
	//Helper to improve readability of source binds while moving callable methods to the class
	template<typename T>
	Action BindHelper(T methodName) {
		return [this, methodName](Game* game) -> bool{
			return (this->*methodName)(game); };
	}
private:
	std::map<UINT, Action> bindingMap;	//Handles binding virtual keys to actions
	bool MoveUp(Game* game);			//space
	bool MoveDown(Game* game);		//shift
	bool MoveForward(Game* game);		//w
	bool MoveBackward(Game* game);	//s
	bool MoveRight(Game* game);		//d
	bool MoveLeft(Game* game);		//a
	bool RollCounterClockwise(Game* game);
	bool RollClockwise(Game* game);
	bool DebugWireframe(Game* game);
	bool m_wireframeButtonStillPressed;
	bool m_wireframeOn;
	float m_cameraSpeed;
	float m_rollSpeed;
	std::unordered_set<UINT> m_pressedKeys;
	float m_changePower;
	GameTool m_gameTool;

	bool UseTool(Game* game);
	bool UseToolAlt(Game* game);

	//Change these later.  There needs to be a way to have different actions on press versus release.
	//This is different from taking an action on press, and stopping that action on release.
	bool Sprint(Game* game);
	bool StopSprinting(Game* game);

	bool ChangeToToolOne(Game* game);
	bool ChangeToToolTwo(Game* game);
	bool ChangeToToolThree(Game* game);
};