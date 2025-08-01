#pragma once
#include "pch.h"
#include <functional>

class Game;
class InputController;
class GraphicsSettingsIO;

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
enum MenuElementAlignment {
	MENU_LEFT = 0,
	MENU_TOP = 0,
	MENU_CENTER = 1,
	MENU_RIGHT = 2,
	MENU_BOTTOM = 2
};

class MenuElementCalculator {
public:
	D2D1_RECT_F CalculateRectangle(int placementH, int placementV, float paddingH, float paddingV, float width, float height, D2D1_RECT_F menuFrame, float menuScale);
};

class MenuButton {
public:
	MenuButton(std::wstring text, int buttonAlignmentH, int buttonAlignmentV, int paddingH, int paddingV, int width, int height, D2D1_RECT_F menuFrame, float menuScale);
	MenuButton(std::wstring text, int buttonAlignmentH, int buttonAlignmentV, int paddingH, int paddingV, int width, int height, D2D1_RECT_F menuFrame, std::function<void()> onClickFunction, float menuScale);
	void SetText(std::wstring text);
	D2D1_RECT_F GetButtonRectangle();
	std::wstring GetButtonText();
	bool IsPointInButton(int posX, int posY);
	void SetOnClickFunction(std::function<void()> callbackFunction); //Function to set the function that a button will call when clicked
	void OnClick();
	void RecalculateButtonSize(float menuScale, D2D1_RECT_F menuFrame); //Recalculate button size and position when resolution changes
private:
	D2D1_RECT_F m_buttonRectangle;
	std::wstring m_buttonText;
	std::function<void()> m_onClickFunction; //Function to call when the button is clicked
	int m_buttonAlignmentH, m_buttonAlignmentV;
	int m_paddingH, m_paddingV;
	int m_width, m_height;
};

class Slider {
public:
	Slider(int alignmentH, int alignmentV, float paddingH, float paddingV, float width, float height, D2D1_RECT_F menuFrame, float menuScale, Game* game);
	Slider(int placementH, int placementV, float paddingH, float paddingV, float width, float height, D2D1_RECT_F menuFrame, float menuScale, std::function<void()> onClickFunction, Game* game);
	void RecalculateSliderSize(float menuScale, D2D1_RECT_F menuFrame);
	float GetWidth();
	float GetHeight();
	float GetValue();
	bool IsPointInSlider(int posX, int posY);
	void OnClick(int posX, int posY);
	void SetSliderFunctionality(std::function<void()> sliderFunctionality);
	std::vector<D2D1_POINT_2F> GetPointsList();
	D2D1_RECT_F GetSliderRectangle();
private:
	D2D1_RECT_F m_sliderRectangle;
	std::vector<D2D1_POINT_2F> m_points;
	int m_alignmentH;
	int m_alignmentV;
	float m_paddingH;
	float m_paddingV;
	float m_width;
	float m_height;
	float m_sliderValue; //From 0.0 to 1.0
	std::function<void()> m_onSliderFunctionality;
	Game* m_game;
};

/*
class TextBox {
public:
	TextBox(std::wstring text, int textPlacementH, int textPlacementV, int paddingH, int paddingV, int width, int height, D2D1_RECT_F menuFrame, float menuScale);
	void SetText(std::wstring text);
	std::wstring GetText();
	D2D1_RECT_F GetTextBoxRectangle();
	void RecalculateTextBoxSize(float menuScale, D2D1_RECT_F menuFrame); //Recalculate button size and position when resolution changes
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
	D2D1_RECT_F m_rectangle;
	std::wstring m_text;
	int m_alignentH, m_alignentV;
	int m_paddingH, m_paddingV;
	int m_width, m_height;
};
*/

/*
class MenuDropDownMenu : public MenuButton {
public:
	MenuDropDownMenu(std::wstring text, int buttonPlacementH, int buttonPlacementV, int paddingH, int paddingV, int width, int height, D2D1_RECT_F menuFrame);
	void AddButton(std::wstring text, int width, int height, std::function<void()> onClickFunction);
private:
	std::vector<MenuButton> m_buttonList;
};
*/


class Menus {
public:
	Menus(int screenSizeH, int screenSizeV, int width, int height, Game* game); //Initialize with the proper InputController format
	void AddButton(MenuButton button);
	MenuButton GetButton(int index);
	Slider GetSlider(int index);
	MenuButton& GetButtonReference(int index);
	bool ClickButton(int posX, int posY);
	bool ClickSlider(int posX, int posY);
	int GetButtonCount();
	int GetSliderCount();
	void SetMenuFrame(float posX1, float posY1, float posX2, float poxY2);
	float CalculateMenuScale(int screenWidth, int screenHeight);
	void RecalculateMenuFrame(int screenWidth, int screenHeight); //Recalculate menu frame size based on screen when resolution changes.
	D2D1_RECT_F GetMenuFrame();
	float GetMenuScale();
	void UpdateButtonText(int index, UINT settingID) {}
	std::vector<MenuButton> p_buttons;
	std::vector<Slider> p_sliders;
	Game* m_game;
	InputController* m_inputController;
private:
	int m_screenSizeH;
	int m_screenSizeV;
	int m_menuWidth;
	int m_menuHeight;
	D2D1_RECT_F m_menuFrame;
	float m_menuScale;
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

class GraphicsSettingsMenu : public Menus {
public:
	GraphicsSettingsMenu(int screenSizeH, int screeSizeV, Game* game);
};

class KeybindMenu : public Menus {
public:
	KeybindMenu(int screenSizeH, int screeSizeV, Game* game, InputController* inputController);
	void ButtonFunction(int buttonIndex, UINT settingID, Game* game);
	std::wstring SettingText(UINT settingID);
	void UpdateButtonText(int index, UINT settingID);
private:
	InputController* m_inputController;
};

class KeybindPromptMenu : public Menus {
public:
	KeybindPromptMenu(int screenSizeH, int screeSizeV, Game* game);
};

class LoadGameMenu : public Menus {
public:
	LoadGameMenu(int screenSizeH, int screeSizeV, Game* game);
};

class SaveGameMenu : public Menus {
public:
	SaveGameMenu(int screenSizeH, int screenSizeV, Game* game);
};

class GameplayUIMenu : public Menus {
public:
	GameplayUIMenu(int screenSizeH, int screenSizeV, Game* game);
	void ChangeToolKeyDisplay(int toolIndex, UINT key);
};

std::wstring ConvertVirtualKeyToString(UINT key);