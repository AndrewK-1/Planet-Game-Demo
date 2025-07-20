#pragma once
#include "pch.h"
#include <functional>

class Game;

class MenuButton {
public:
	MenuButton(std::wstring text, int buttonPlacementH, int buttonPlacementV, int paddingH, int paddingV, int width, int height, int screenSizeH, int screenSizeV);
	void SetText(std::wstring text);
	D2D1_RECT_F GetButtonRectangle();
	std::wstring GetButtonText();
	bool IsPointInButton(int posX, int posY);
	void SetOnClickFunction(std::function<void()> callbackFunction); //Function to set the function that a button will call when clicked
	void OnClick();
	enum ButtonHorizontalPlacement {
		BUTTON_H_LEFT = 0,
		BUTTON_H_CENTER = 1,
		BUTTON_H_RIGHT = 2
	};
	enum ButtonVerticalPlacement {
		BUTTON_V_TOP = 0,
		BUTTON_V_CENTER = 1,
		BUTTON_V_BOTTOM = 2
	};
private:
	D2D1_RECT_F m_buttonRectangle;
	std::wstring m_buttonText;
	std::function<void()> m_onClickFunction; //Function to call when the button is clicked
};

class Menus {
public:
	Menus(int screenSizeH, int screenSizeV, Game* game); //Initialize with the proper InputController format

	void AddButton(MenuButton button);
	MenuButton GetButton(int index) {
		return p_buttons.at(index);
	}

	bool ClickButton(int posX, int posY) {
		for (auto button : p_buttons) {
			if (button.IsPointInButton(posX, posY)) {
				std::wstring msg = L"Button clicked: " + button.GetButtonText() + L"\n";
				OutputDebugString(msg.c_str());
				button.OnClick();
				return true;
			}
		}
		return false;
	}
	int GetButtonCount() {
		return static_cast<int>(p_buttons.size());
	}
	void SetMenuFrame(int posX1, int posY1, int posX2, int poxY2);
	D2D1_RECT_F GetMenuFrame() {
		return m_menuFrame;
	};
	std::vector<MenuButton> p_buttons;
	Game* m_game;
private:
	unsigned int m_inputContext; //Determine which input formatting to use from the InputController class.
	int m_screenSizeH;
	int m_screenSizeV;
	D2D1_RECT_F m_menuFrame;
};

class MainMenu : public Menus {
public:
	MainMenu(int screenSizeH, int screenSizeV, Game* game);
private:
};

class InGameMenu : public Menus {
public:
	InGameMenu(int screenSizeH, int screeSizeV, Game* game);
};

class SettingsMenu : public Menus {
public:
	SettingsMenu(int screenSizeH, int screeSizeV, Game* game);
};