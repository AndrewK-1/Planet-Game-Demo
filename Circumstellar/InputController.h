#pragma once
#include "pch.h"
#include <map>
#include <functional>
#include "Camera.h"
#include "Game.h"
#include <unordered_set>

//Function alias Action that takes no parameters and returns void.
//When Action is used from here on, it means std::function<void()>.
using Action = std::function<void(Game*)>;

//Binding structure for mapping
struct InputBinding {
	UINT key;
	Action action;
};

class InputController {
public:
	InputController();
	void BindKey(UINT key, Action action);
	void HandleKeyDown(UINT key, Game* game);
	void HandleKeyUp(UINT key, Game* game);
	void PressedKeysExecute(Game* game);
	void HandleRawInput(long x, long y, Game* game);
	
	//Helper to improve readability of source binds while moving callable methods to the class
	template<typename T>
	Action BindHelper(T methodName) {
		return [this, methodName](Game* game) {(this->*methodName)(game); };
	}
private:
	std::map<UINT, Action> bindingMap;	//Handles binding virtual keys to actions
	void MoveUp(Game* camera);			//space
	void MoveDown(Game* camera);		//shift
	void MoveForward(Game* camera);		//w
	void MoveBackward(Game* camera);	//s
	void MoveRight(Game* camera);		//d
	void MoveLeft(Game* camera);		//a
	float cameraSpeed;
	std::unordered_set<UINT> m_pressedKeys;
};