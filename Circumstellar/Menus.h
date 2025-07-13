#pragma once
#include "pch.h"

class Menus {
public:
	Menus(unsigned int inputContext); //Initialize with the proper InputController format
	unsigned int GetInputContext(); //Return input context
	void OnWindowResized(); //Adjust the position of UI elements when the screen is resized.
	void OnClicked(); //Handle what happens in the UI when a click occurs.
private:
	unsigned int m_inputContext; //Determine which input formatting to use from the InputController class
};

class MainMenu : public Menus {
public:
	MainMenu(unsigned int inputContext);
private:
};