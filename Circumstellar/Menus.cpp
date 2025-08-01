#pragma once
#include "pch.h"
#include "Menus.h"
#include "Game.h"
#include "InputController.h"
#include "GraphicsSettingsIO.h"

bool MenuButton::IsPointInButton(int posX, int posY) {
	if (posX >= m_buttonRectangle.left && posX <= m_buttonRectangle.right &&
		posY >= m_buttonRectangle.top && posY <= m_buttonRectangle.bottom) {
		return true;
	}
	return false;
}

bool Slider::IsPointInSlider(int posX, int posY) {
	if (posX >= m_sliderRectangle.left && posX <= m_sliderRectangle.right &&
		posY >= m_sliderRectangle.top && posY <= m_sliderRectangle.bottom) {
		return true;
	}
	return false;
}

D2D1_RECT_F MenuElementCalculator::CalculateRectangle(int placementH, int placementV, float paddingH, float paddingV, float width, float height, D2D1_RECT_F menuFrame, float menuScale) {
	float posX1, posX2, posY1, posY2;
	if (placementH == MENU_TOP) {
		posX1 = paddingH + menuFrame.left;
		posX2 = paddingH + width + menuFrame.left;
	}
	else if (placementH == MENU_CENTER) {
		posX1 = (menuFrame.right - menuFrame.left) / 2.0f - width / 2.0f + paddingH + menuFrame.left;
		posX2 = (menuFrame.right - menuFrame.left) / 2.0f + width / 2.0f + paddingH + menuFrame.left;
	}
	else {
		posX1 = menuFrame.right - paddingH - width + paddingH;
		posX2 = menuFrame.right - paddingH + paddingH;
	}

	if (placementV == MENU_TOP) {
		posY1 = paddingV + menuFrame.top;
		posY2 = paddingV + height + menuFrame.top;
	}
	else if (placementV == MENU_CENTER) {
		posY1 = (menuFrame.bottom - menuFrame.top) / 2.0f - height / 2.0f + paddingV + menuFrame.top;
		posY2 = (menuFrame.bottom - menuFrame.top) / 2.0f + height / 2.0f + paddingV + menuFrame.top;
	}
	else {
		posY1 = menuFrame.bottom - paddingV - height;
		posY2 = menuFrame.bottom - paddingV;
	}
	return D2D1::RectF(posX1, posY1, posX2,	posY2);
}

Slider::Slider(int placementH, int placementV, float paddingH, float paddingV, float width, float height, D2D1_RECT_F menuFrame, float menuScale, Game* game)
	: m_alignmentH(placementH), m_alignmentV(placementV), m_paddingH(paddingH), m_paddingV(paddingV), m_width(width), m_height(height), m_game(game)
{
	std::string setting;
	if (m_game->GetSettingIO()->GetSetting("GraphicsSettings.txt", "FieldOfView", setting)) {
		m_sliderValue = std::stof(setting) / 3.14f;
	}
	else {
		m_sliderValue = 0.27f;
	}
	std::wstring msg = L"Constructed slider value is: " + std::to_wstring(m_sliderValue) + L"\n";
	OutputDebugString(msg.c_str());

	MenuElementCalculator calc;
	m_sliderRectangle = calc.CalculateRectangle(m_alignmentH, m_alignmentV, paddingH, paddingV, width, height, menuFrame, menuScale);
	RecalculateSliderSize(menuScale, menuFrame);
}

Slider::Slider(int placementH, int placementV, float paddingH, float paddingV, float width, float height, D2D1_RECT_F menuFrame, float menuScale, std::function<void()> onClickFunction, Game* game)
	: Slider(placementH, placementV, paddingH, paddingV, width, height, menuFrame, menuScale, game)
{
	m_onSliderFunctionality = onClickFunction;
}

void Slider::RecalculateSliderSize(float menuScale, D2D1_RECT_F menuFrame) {
	float paddingH = static_cast<float>(m_paddingH) * menuScale, paddingV = static_cast<float>(m_paddingV) * menuScale;
	float width = static_cast<float>(m_width) * menuScale, height = static_cast<float>(m_height) * menuScale;
	MenuElementCalculator calc;
	m_sliderRectangle = calc.CalculateRectangle(m_alignmentH, m_alignmentV, paddingH, paddingV, width, height, menuFrame, menuScale);

	//T=top, B=bottom, L=left, R=right, M=middle
	D2D1_POINT_2F pointTL = D2D1::Point2F(m_sliderRectangle.left, m_sliderRectangle.top);
	D2D1_POINT_2F pointBL = D2D1::Point2F(m_sliderRectangle.left, m_sliderRectangle.bottom);
	D2D1_POINT_2F pointTR = D2D1::Point2F(m_sliderRectangle.right, m_sliderRectangle.top);
	D2D1_POINT_2F pointBR = D2D1::Point2F(m_sliderRectangle.right, m_sliderRectangle.bottom);
	D2D1_POINT_2F pointML = D2D1::Point2F(m_sliderRectangle.left, (m_sliderRectangle.bottom - m_sliderRectangle.top) / 2.0f + m_sliderRectangle.top);
	D2D1_POINT_2F pointMR = D2D1::Point2F(m_sliderRectangle.right, (m_sliderRectangle.bottom - m_sliderRectangle.top) / 2.0f + m_sliderRectangle.top);

	m_points.clear();
	m_points.push_back(pointTL);
	m_points.push_back(pointBL);
	m_points.push_back(pointTR);
	m_points.push_back(pointBR);
	m_points.push_back(pointML);
	m_points.push_back(pointMR);
}

float Slider::GetHeight() {
	return m_height;
}

float Slider::GetWidth() {
	return m_width;
}

void Slider::OnClick(int posX, int posY) {
	if (IsPointInSlider(posX, posY)) {
		m_sliderValue = std::clamp((posX - m_sliderRectangle.left) / (m_sliderRectangle.right - m_sliderRectangle.left), 0.0f, 1.0f);
		std::wstring msg = L"New slider value: " + std::to_wstring(m_sliderValue); msg += L"\n";
		OutputDebugString(msg.c_str());

		if (m_onSliderFunctionality) {
			m_onSliderFunctionality();
		}
	}
}

float Slider::GetValue() {
	return m_sliderValue;
}

void Slider::SetSliderFunctionality(std::function<void()> sliderFunctionality) {
	m_onSliderFunctionality = sliderFunctionality;
}

std::vector<D2D1_POINT_2F> Slider::GetPointsList() {
	return m_points;
}

D2D1_RECT_F Slider::GetSliderRectangle() {
	return m_sliderRectangle;
}

MenuButton::MenuButton(
	std::wstring text, int buttonAlignmentH, int buttonAlignmentV, int paddingH, int paddingV, int width, int height, D2D1_RECT_F menuFrame, float menuScale)
	: m_buttonAlignmentH(buttonAlignmentH), m_buttonAlignmentV(buttonAlignmentV), m_paddingH(paddingH), m_paddingV(paddingV), m_width(width), m_height(height) 
{
	MenuElementCalculator calc;
	m_buttonRectangle = calc.CalculateRectangle(buttonAlignmentH, buttonAlignmentV, paddingH, paddingV, width, height, menuFrame, menuScale);
	m_buttonText = text;
	RecalculateButtonSize(menuScale, menuFrame);
}

MenuButton::MenuButton(std::wstring text, int buttonPlacementH, int buttonPlacementV, int paddingH, int paddingV, int width, int height, D2D1_RECT_F menuFrame, std::function<void()> onClickFunction, float menuScale)
	: MenuButton(text, buttonPlacementH, buttonPlacementV, paddingH, paddingV, width, height, menuFrame, menuScale)
{
	SetOnClickFunction(onClickFunction);
}

void MenuButton::RecalculateButtonSize(float menuScale, D2D1_RECT_F menuFrame) {
	float paddingH = static_cast<float>(m_paddingH) * menuScale, paddingV = static_cast<float>(m_paddingV) * menuScale;
	float width = static_cast<float>(m_width) * menuScale, height = static_cast<float>(m_height) * menuScale;
	MenuElementCalculator calc;
	m_buttonRectangle = calc.CalculateRectangle(m_buttonAlignmentH, m_buttonAlignmentV, paddingH, paddingV, width, height, menuFrame, menuScale);
}

void MenuButton::SetText(std::wstring text) {
	OutputDebugString(L"Setting text\n");
	m_buttonText = text;
}

D2D1_RECT_F MenuButton::GetButtonRectangle() {
	return m_buttonRectangle;
}

std::wstring MenuButton::GetButtonText() {
	return m_buttonText;
}

void MenuButton::SetOnClickFunction(std::function<void()> callbackFunction) {
	m_onClickFunction = callbackFunction;
}

void MenuButton::OnClick() {
	OutputDebugString(L"MenuButton: Button clicked.\n");
	if (m_onClickFunction) {
		m_onClickFunction();
	}
}

/*
MenuDropDownMenu::MenuDropDownMenu(std::wstring text, int buttonPlacementH, int buttonPlacementV, int paddingH, int paddingV, int width, int height, D2D1_RECT_F menuFrame)
	: MenuButton(text, buttonPlacementH, buttonPlacementV, paddingH, paddingV, width, height, menuFrame) {}

void MenuDropDownMenu::AddButton(std::wstring text, int width, int height, std::function<void()> onClickFunction) {
	m_buttonList.push_back()
}
*/

Menus::Menus(int screenSizeH, int screenSizeV, int width, int height, Game* game) : 
	m_screenSizeH(screenSizeH),
	m_screenSizeV(screenSizeV),
	m_menuWidth(width),
	m_menuHeight(height),
	m_game(game),
	m_menuScale(CalculateMenuScale(screenSizeH, screenSizeV))
{
	SetMenuFrame(
		screenSizeH / 2.0f - (width * m_menuScale) / 2.0f,
		screenSizeV / 2.0f - (height * m_menuScale) / 2.0f,
		screenSizeH / 2.0f + (width * m_menuScale) / 2.0f,
		screenSizeV / 2.0f + (height * m_menuScale) / 2.0f
	);
}

float Menus::CalculateMenuScale(int screenWidth, int screenHeight) {
	float widthDifference = static_cast<float>(screenWidth) / 1920.0f;
	float heightDifference = static_cast<float>(screenHeight) / 1080.0f;
	if (widthDifference < heightDifference) {
		return widthDifference;
	}
	else {
		return heightDifference;
	}
}

void Menus::RecalculateMenuFrame(int screenWidth, int screenHeight) {
	m_menuScale = CalculateMenuScale(screenWidth, screenHeight);
	
	float width = m_menuWidth * m_menuScale;
	float height = m_menuHeight * m_menuScale;

	SetMenuFrame(
		(static_cast<float>(screenWidth) / 2.0f - width / 2.0f),
		(static_cast<float>(screenHeight) / 2.0f - height / 2.0f),
		(static_cast<float>(screenWidth) / 2.0f + width / 2.0f),
		(static_cast<float>(screenHeight) / 2.0f + height / 2.0f)
	);

	std::wstring msg = L"Menu sizes:\n  ";
	msg += std::to_wstring(static_cast<float>(screenWidth) / 2.0f - width / 2.0f) + L"\n  ";
	msg += std::to_wstring(static_cast<float>(screenHeight) / 2.0f - height / 2.0f) + L"\n  ";
	msg += std::to_wstring(static_cast<float>(screenWidth) / 2.0f + width / 2.0f) + L"\n  ";
	msg += std::to_wstring(static_cast<float>(screenHeight) / 2.0f + height / 2.0f) + L"\n\n  ";

	msg += std::to_wstring(m_menuFrame.left) + L"\n  ";
	msg += std::to_wstring(m_menuFrame.top) + L"\n  ";
	msg += std::to_wstring(m_menuFrame.right) + L"\n  ";
	msg += std::to_wstring(m_menuFrame.bottom) + L"\n";
	OutputDebugString(msg.c_str());
	msg = L"Menu scale: ";
	msg += std::to_wstring(m_menuScale) + L"\n";
	OutputDebugString(msg.c_str());

	for (auto& button : p_buttons) {
		button.RecalculateButtonSize(m_menuScale, m_menuFrame);
	}
	for (auto& slider : p_sliders) {
		slider.RecalculateSliderSize(m_menuScale, m_menuFrame);
	}
}

void Menus::AddButton(MenuButton button) {
	p_buttons.push_back(button);
}

void Menus::SetMenuFrame(float posX1, float posY1, float posX2, float posY2) {
	m_menuFrame = D2D1::RectF(posX1, posY1, posX2, posY2);
}

int Menus::GetButtonCount() {
	return static_cast<int>(p_buttons.size());
}

int Menus::GetSliderCount() {
	return p_sliders.size();
}

MenuButton Menus::GetButton(int index) {
	return p_buttons.at(index);
}

Slider Menus::GetSlider(int index) {
	return p_sliders.at(index);
}

MenuButton& Menus::GetButtonReference(int index) {
	return p_buttons.at(index);
}

D2D1_RECT_F Menus::GetMenuFrame() {
	return m_menuFrame;
};

float Menus::GetMenuScale() {
	return m_menuScale;
}

bool Menus::ClickButton(int posX, int posY) {
	for (auto& button : p_buttons) {
		if (button.IsPointInButton(posX, posY)) {
			std::wstring msg = L"Button clicked: " + button.GetButtonText() + L"\n";
			OutputDebugString(msg.c_str());
			button.OnClick();
			return true;
		}
	}
	return false;
}

bool Menus::ClickSlider(int posX, int posY) {
	for (auto& slider : p_sliders) {
		if (slider.IsPointInSlider(posX, posY)) {
			std::wstring msg = L"Slider clicked.\n";
			OutputDebugString(msg.c_str());
			slider.OnClick(posX, posY);
			return true;
		}
	}
	return false;
}

MainMenu::MainMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 1800, 1000, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();
	float menuScale = GetMenuScale();

	MenuButton exitButton(L"Exit", 0, 2, 100, 100, 200, 50, menuFrame, [this]() {m_game->Exit(); }, menuScale);
	MenuButton settingsMenuButton(L"Settings", 0, 2, 100, 200, 200, 50, menuFrame, [this]() {m_game->OpenSettingsMenu(); }, menuScale);
	MenuButton loadButton(L"Load Game", 0, 2, 100, 300, 200, 50, menuFrame, [this]() {m_game->OpenLoadMenu(); }, menuScale);
	MenuButton newGameButton(L"New Game", 0, 2, 100, 400, 200, 50, menuFrame, [this]() {m_game->NewWorld(); }, menuScale);

	p_buttons.push_back(exitButton);
	p_buttons.push_back(settingsMenuButton);
	p_buttons.push_back(loadButton);
	p_buttons.push_back(newGameButton);
}

InGameMenu::InGameMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 400, 450, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();
	float menuScale = GetMenuScale();

	MenuButton saveGameButton(L"Save Game", 1, 1, 0, -150, 300, 50, menuFrame, [this]() {m_game->OpenSaveMenu(); }, menuScale);
	MenuButton settingsButton(L"Settings", 1, 1, 0, -50, 300, 50, menuFrame, [this]() {m_game->OpenSettingsMenu(); }, menuScale);
	MenuButton closeButton(L"Close Menu", 1, 1, 0, 50, 300, 50, menuFrame, [this]() {m_game->CloseTopmostMenu(); }, menuScale);
	MenuButton exitButton(L"Exit Game", 1, 1, 0, 150, 300, 50, menuFrame, [this]() {m_game->ExitWorld(); }, menuScale);

	p_buttons.push_back(saveGameButton);
	p_buttons.push_back(settingsButton);
	p_buttons.push_back(closeButton);
	p_buttons.push_back(exitButton);
}

SettingsMenu::SettingsMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 500, 500, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();
	float menuScale = GetMenuScale();

	MenuButton graphicsButton(L"Graphics Settings", 1, 1, 0, -100, 300, 50, menuFrame, [this]() {m_game->OpenGraphicsSettingsMenu(); }, menuScale);
	MenuButton keybindButton(L"Key Bindings", 1, 1, 0, 0, 300, 50, menuFrame, [this]() {m_game->OpenKeybindMenu(); }, menuScale);
	MenuButton closeButton(L"Close Menu", 1, 1, 0, 100, 300, 50, menuFrame, [this]() {m_game->CloseTopmostMenu(); }, menuScale);

	p_buttons.push_back(graphicsButton);
	p_buttons.push_back(keybindButton);
	p_buttons.push_back(closeButton);
}

GraphicsSettingsMenu::GraphicsSettingsMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 1000, 800, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();
	float menuScale = GetMenuScale();

	p_buttons.push_back(MenuButton(L"3840 x 2160", 1, 0, -150, 50, 300, 50, menuFrame, [this]() {m_game->SetResolution(3840, 2160); }, menuScale));
	p_buttons.push_back(MenuButton(L"2715 x 1527", 1, 0, -150, 100, 300, 50, menuFrame, [this]() {m_game->SetResolution(2715, 1527); }, menuScale));
	p_buttons.push_back(MenuButton(L"1920 x 1080", 1, 0, -150, 150, 300, 50, menuFrame, [this]() {m_game->SetResolution(1920, 1080); }, menuScale));
	p_buttons.push_back(MenuButton(L"1768 x 992", 1, 0, -150, 200, 300, 50, menuFrame, [this]() {m_game->SetResolution(1768, 992); }, menuScale));
	p_buttons.push_back(MenuButton(L"1680 x 1050", 1, 0, -150, 250, 300, 50, menuFrame, [this]() {m_game->SetResolution(1680, 1050); }, menuScale));
	p_buttons.push_back(MenuButton(L"1600 x 1024", 1, 0, -150, 300, 300, 50, menuFrame, [this]() {m_game->SetResolution(1600, 1024); }, menuScale));
	p_buttons.push_back(MenuButton(L"1600 x 900", 1, 0, -150, 350, 300, 50, menuFrame, [this]() {m_game->SetResolution(1600, 900); }, menuScale));
	p_buttons.push_back(MenuButton(L"1440 x 1080", 1, 0, -150, 400, 300, 50, menuFrame, [this]() {m_game->SetResolution(1440, 1080); }, menuScale));
	p_buttons.push_back(MenuButton(L"1440 x 900", 1, 0, -150, 450, 300, 50, menuFrame, [this]() {m_game->SetResolution(1440, 900); }, menuScale));
	p_buttons.push_back(MenuButton(L"1360 x 768", 1, 0, -150, 500, 300, 50, menuFrame, [this]() {m_game->SetResolution(1360, 768); }, menuScale));
	p_buttons.push_back(MenuButton(L"1280 x 1024", 1, 0, -150, 550, 300, 50, menuFrame, [this]() {m_game->SetResolution(1280, 1024); }, menuScale));
	p_buttons.push_back(MenuButton(L"1280 x 960", 1, 0, 150, 50, 300, 50, menuFrame, [this]() {m_game->SetResolution(1280, 960); }, menuScale));
	p_buttons.push_back(MenuButton(L"1280 x 800", 1, 0, 150, 100, 300, 50, menuFrame, [this]() {m_game->SetResolution(1280, 800); }, menuScale));
	p_buttons.push_back(MenuButton(L"1280 x 768", 1, 0, 150, 150, 300, 50, menuFrame, [this]() {m_game->SetResolution(1280, 768); }, menuScale));
	p_buttons.push_back(MenuButton(L"1280 x 720", 1, 0, 150, 200, 300, 50, menuFrame, [this]() {m_game->SetResolution(1280, 720); }, menuScale));
	p_buttons.push_back(MenuButton(L"1176 x 664", 1, 0, 150, 250, 300, 50, menuFrame, [this]() {m_game->SetResolution(1176, 664); }, menuScale));
	p_buttons.push_back(MenuButton(L"1152 x 864", 1, 0, 150, 300, 300, 50, menuFrame, [this]() {m_game->SetResolution(1152, 864); }, menuScale));
	p_buttons.push_back(MenuButton(L"1024 x 768", 1, 0, 150, 350, 300, 50, menuFrame, [this]() {m_game->SetResolution(1024, 768); }, menuScale));
	p_buttons.push_back(MenuButton(L"800 x 600", 1, 0, 150, 400, 300, 50, menuFrame, [this]() {m_game->SetResolution(800, 600); }, menuScale));
	p_buttons.push_back(MenuButton(L"720 x 480", 1, 0, 150, 450, 300, 50, menuFrame, [this]() {m_game->SetResolution(720, 480); }, menuScale));
	p_buttons.push_back(MenuButton(L"640 x 480", 1, 0, 150, 500, 300, 50, menuFrame, [this]() {m_game->SetResolution(640, 480); }, menuScale));

	Slider fovSlider(1, 0, 0, 625, 300, 50, menuFrame, menuScale, [game]() { game->SetFOV(1.0f); }, game);
	p_sliders.push_back(fovSlider);

	MenuButton closeButton(L"Close Menu", 1, 0, 0, 700, 300, 50, menuFrame, [this, game]() {m_game->CloseTopmostMenu(); }, menuScale);
	p_buttons.push_back(closeButton);
}

KeybindMenu::KeybindMenu(int screenSizeH, int screenSizeV, Game* game, InputController* inputCon) : Menus(screenSizeH, screenSizeV, 1000, 1050, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();
	float menuScale = GetMenuScale();
	UINT oldKey;
	m_inputController = inputCon;
	
	MenuButton setUp(SettingText(ID_PlayerUp), 1, 0, 225, 50, 450, 50, menuFrame, [this, game]() { ButtonFunction(0, ID_PlayerUp, game); }, menuScale);
	MenuButton setDown(SettingText(ID_PlayerDown), 1, 0, 225, 100, 450, 50, menuFrame, [this, game]() { ButtonFunction(1, ID_PlayerDown, game); }, menuScale);
	MenuButton setLeft(SettingText(ID_PlayerLeft), 1, 0, 225, 150, 450, 50, menuFrame, [this, game]() { ButtonFunction(2, ID_PlayerLeft, game); }, menuScale);
	MenuButton setRight(SettingText(ID_PlayerRight), 1, 0, 225, 200, 450, 50, menuFrame, [this, game]() { ButtonFunction(3, ID_PlayerRight, game); }, menuScale);
	MenuButton setForward(SettingText(ID_PlayerForward), 1, 0, 225, 250, 450, 50, menuFrame, [this, game]() { ButtonFunction(4, ID_PlayerForward, game); }, menuScale);
	MenuButton setBackward(SettingText(ID_PlayerBackward), 1, 0, 225, 300, 450, 50, menuFrame, [this, game]() { ButtonFunction(5, ID_PlayerBackward, game); }, menuScale);
	MenuButton setRollClockwise(SettingText(ID_PlayerRollClockwise), 1, 0, 225, 350, 450, 50, menuFrame, [this, game]() { ButtonFunction(6, ID_PlayerRollClockwise, game); }, menuScale);
	MenuButton setRollCounterClockwise(SettingText(ID_PlayerRollCounterClockwise), 1, 0, 225, 400, 450, 50, menuFrame, [this, game]() { ButtonFunction(7, ID_PlayerRollCounterClockwise, game); }, menuScale);
	MenuButton setMount(SettingText(ID_PlayerMount), 1, 0, 225, 450, 450, 50, menuFrame, [this, game]() { ButtonFunction(8, ID_PlayerMount, game); }, menuScale);
	MenuButton setUseTool(SettingText(ID_UseTool), 1, 0, 225, 500, 450, 50, menuFrame, [this, game]() { ButtonFunction(9, ID_UseTool, game); }, menuScale);
	MenuButton setUseToolAlt(SettingText(ID_UseToolAlt), 1, 0, 225, 550, 450, 50, menuFrame, [this, game]() { ButtonFunction(10, ID_UseToolAlt, game); }, menuScale);
	MenuButton setSpringt(SettingText(ID_Sprint), 1, 0, 225, 600, 450, 50, menuFrame, [this, game]() { ButtonFunction(11, ID_Sprint, game); }, menuScale);
	MenuButton setChangeToToolOne(SettingText(ID_ChangeToToolOne), 1, 0, 225, 650, 450, 50, menuFrame, [this, game]() { ButtonFunction(12, ID_ChangeToToolOne, game); }, menuScale);
	MenuButton setChangeTotoolTwo(SettingText(ID_ChangeToToolTwo), 1, 0, 225, 700, 450, 50, menuFrame, [this, game]() { ButtonFunction(13, ID_ChangeToToolTwo, game); }, menuScale);
	MenuButton setChangeToToolThree(SettingText(ID_ChangeToToolThree), 1, 0, 225, 750, 450, 50, menuFrame, [this, game]() { ButtonFunction(14, ID_ChangeToToolThree, game); }, menuScale);
	MenuButton setChangeToToolOff(SettingText(ID_ChangeToToolOff), 1, 0, 225, 800, 450, 50, menuFrame, [this, game]() { ButtonFunction(15, ID_ChangeToToolOff, game); }, menuScale);
	MenuButton setDebugWireframe(SettingText(ID_DebugWireframe), 1, 0, 225, 850, 450, 50, menuFrame, [this, game]() { ButtonFunction(16, ID_DebugWireframe, game); }, menuScale);
	MenuButton setGameMenu(SettingText(ID_GameMenu), 1, 0, 225, 900, 450, 50, menuFrame, [this, game]() { ButtonFunction(17, ID_GameMenu, game); }, menuScale);

	MenuButton titleUp(L"Up: ", 1, 0, -225, 50, 450, 50, menuFrame, menuScale);
	MenuButton titleDown(L"Down: ", 1, 0, -225, 100, 450, 50, menuFrame, menuScale);
	MenuButton titleLeft(L"Left: ", 1, 0, -225, 150, 450, 50, menuFrame, menuScale);
	MenuButton titleRight(L"Right: ", 1, 0, -225, 200, 450, 50, menuFrame, menuScale);
	MenuButton titleForward(L"Forward: ", 1, 0, -225, 250, 450, 50, menuFrame, menuScale);
	MenuButton titleBackward(L"Backward: ", 1, 0, -225, 300, 450, 50, menuFrame, menuScale);
	MenuButton titleRollClockwise(L"Roll Clockwise: ", 1, 0, -225, 350, 450, 50, menuFrame, menuScale);
	MenuButton titleRollCounterClockwise(L"Roll Counter Clockwise: ", 1, 0, -225, 400, 450, 50, menuFrame, menuScale);
	MenuButton titleMount(L"Mount Vehicle: ", 1, 0, -225, 450, 450, 50, menuFrame, menuScale);
	MenuButton titleUseTool(L"Use Tool: ", 1, 0, -225, 500, 450, 50, menuFrame, menuScale);
	MenuButton titleUseToolAlt(L"Use Tool Alt: ", 1, 0, -225, 550, 450, 50, menuFrame, menuScale);
	MenuButton titleStopSprinting(L"Sprint: ", 1, 0, -225, 600, 450, 50, menuFrame, menuScale);
	MenuButton titleChangeToToolOne(L"Change to Tool One: ", 1, 0, -225, 650, 450, 50, menuFrame, menuScale);
	MenuButton titleChangeToToolTwo(L"Change to Tool Two: ", 1, 0, -225, 700, 450, 50, menuFrame, menuScale);
	MenuButton titleChangeToToolThree(L"Change to Tool Three: ", 1, 0, -225, 750, 450, 50, menuFrame, menuScale);
	MenuButton titleChangeToToolOff(L"Change to Tool Off: ", 1, 0, -225, 800, 450, 50, menuFrame, menuScale);
	MenuButton titleDebugWireframe(L"Debug View: ", 1, 0, -225, 850, 450, 50, menuFrame, menuScale);
	MenuButton titleGameMenu(L"Open Menu: ", 1, 0, -225, 900, 450, 50, menuFrame, menuScale);

	p_buttons.push_back(setUp);
	p_buttons.push_back(setDown);
	p_buttons.push_back(setLeft);
	p_buttons.push_back(setRight);
	p_buttons.push_back(setForward);
	p_buttons.push_back(setBackward);
	p_buttons.push_back(setRollClockwise);
	p_buttons.push_back(setRollCounterClockwise);
	p_buttons.push_back(setMount);
	p_buttons.push_back(setUseTool);
	p_buttons.push_back(setUseToolAlt);
	p_buttons.push_back(setSpringt);
	p_buttons.push_back(setChangeToToolOne);
	p_buttons.push_back(setChangeTotoolTwo);
	p_buttons.push_back(setChangeToToolThree);
	p_buttons.push_back(setChangeToToolOff);
	p_buttons.push_back(setDebugWireframe);
	p_buttons.push_back(setGameMenu);

	p_buttons.push_back(titleUp);
	p_buttons.push_back(titleDown);
	p_buttons.push_back(titleLeft);
	p_buttons.push_back(titleRight);
	p_buttons.push_back(titleForward);
	p_buttons.push_back(titleBackward);
	p_buttons.push_back(titleRollClockwise);
	p_buttons.push_back(titleRollCounterClockwise);
	p_buttons.push_back(titleMount);
	p_buttons.push_back(titleUseTool);
	p_buttons.push_back(titleUseToolAlt);
	p_buttons.push_back(titleStopSprinting);
	p_buttons.push_back(titleChangeToToolOne);
	p_buttons.push_back(titleChangeToToolTwo);
	p_buttons.push_back(titleChangeToToolThree);
	p_buttons.push_back(titleChangeToToolOff);
	p_buttons.push_back(titleDebugWireframe);
	p_buttons.push_back(titleGameMenu);

	MenuButton closeButton(L"Close Menu", 1, 0, 0, 950, 300, 50, menuFrame, [this, game]() {m_game->CloseTopmostMenu(); }, menuScale);
	p_buttons.push_back(closeButton);
}

void KeybindMenu::ButtonFunction(int buttonIndex, UINT settingID, Game* game) {
	m_inputController->PrepareKeybindChange(buttonIndex, settingID);
	game->OpenKeybindPromptMenu();
}

std::wstring KeybindMenu::SettingText(UINT settingID) {
	return ConvertVirtualKeyToString(m_inputController->GetGameBindSetting(settingID));
}

void KeybindMenu::UpdateButtonText(int index, UINT settingID) {
	p_buttons.at(index).SetText(SettingText(settingID));
}

KeybindPromptMenu::KeybindPromptMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 1000, 300, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();
	float menuScale = GetMenuScale();

	MenuButton promptButton(L"Press a key to bind new control.", 1, 1, 0, 0, 600, 50, menuFrame, menuScale);

	p_buttons.push_back(promptButton);
}

LoadGameMenu::LoadGameMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 800, 800, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();
	float menuScale = GetMenuScale();

	MenuButton loadGameOneButton(L"Load World One", 1, 1, 0, -200, 300, 100, menuFrame, [this]() {m_game->LoadWorld(L"World One.cwd"); }, menuScale);
	MenuButton loadGameTwoButton(L"Load World Two", 1, 1, 0, 0, 300, 100, menuFrame, [this]() {m_game->LoadWorld(L"World Two.cwd"); }, menuScale);
	MenuButton loadGameThreeButton(L"Load World Three", 1, 1, 0, 200, 300, 100, menuFrame, [this]() {m_game->LoadWorld(L"World Three.cwd"); }, menuScale);

	p_buttons.push_back(loadGameOneButton);
	p_buttons.push_back(loadGameTwoButton);
	p_buttons.push_back(loadGameThreeButton);
}

SaveGameMenu::SaveGameMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 800, 800, game) {
	MenuButton saveGameOneButton(L"Save World One", 1, 1, 0, -200, 300, 100, GetMenuFrame(), [this]() {m_game->SaveWorld(L"World One.cwd"); m_game->CloseTopmostMenu(); }, GetMenuScale());
	MenuButton saveGameTwoButton(L"Save World Two", 1, 1, 0, 0, 300, 100, GetMenuFrame(), [this]() {m_game->SaveWorld(L"World Two.cwd"); m_game->CloseTopmostMenu(); }, GetMenuScale());
	MenuButton saveGameThreeButton(L"Save World Three", 1, 1, 0, 200, 300, 100, GetMenuFrame(), [this]() {m_game->SaveWorld(L"World Three.cwd"); m_game->CloseTopmostMenu(); }, GetMenuScale());

	p_buttons.push_back(saveGameOneButton);
	p_buttons.push_back(saveGameTwoButton);
	p_buttons.push_back(saveGameThreeButton);
}

GameplayUIMenu::GameplayUIMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, screenSizeH, screenSizeV, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();
	float menuScale = GetMenuScale();
	MenuButton terrainToolDisplay = { L"\nTerrain", 1, 2, -250, 10, 200, 100, menuFrame, menuScale };
	MenuButton blockToolDisplay = { L"\nBlocks", 1, 2, 0, 10, 200, 100, menuFrame, menuScale };
	MenuButton shipToolDisplay = { L"\nShips", 1, 2, 250, 10, 200, 100, menuFrame, menuScale };

	p_buttons.push_back(terrainToolDisplay);
	p_buttons.push_back(blockToolDisplay);
	p_buttons.push_back(shipToolDisplay);
}

void GameplayUIMenu::ChangeToolKeyDisplay(int toolIndex, UINT key) {
	MenuButton& button = p_buttons.at(toolIndex);
	switch (toolIndex) {
	case 0: {
		button.SetText(ConvertVirtualKeyToString(key) + L"\nTerrain");
		break;
	}
	case 1: {
		button.SetText(ConvertVirtualKeyToString(key) + L"\nBlocks");
		break;
	}
	case 2: {
		button.SetText(ConvertVirtualKeyToString(key) + L"\nShips");
		break;
	}
	}
}

std::wstring ConvertVirtualKeyToString(UINT key) {
	switch (key) {
	case VK_LBUTTON: return L"Left MButton";
	case VK_RBUTTON: return L"Right MButton";
	case VK_CANCEL: return L"Cancel";
	case VK_MBUTTON: return L"Middle MButton";
	case VK_XBUTTON1: return L"X1 Mouse Button";
	case VK_XBUTTON2: return L"X2 Mouse Button";
	case VK_BACK: return L"Backspace";
	case VK_TAB: return L"Tab";
	case VK_CLEAR: return L"Clear";
	case VK_RETURN: return L"Enter";
	case VK_SHIFT: return L"Shift";
	case VK_CONTROL: return L"Control";
	case VK_MENU: return L"Alt";
	case VK_PAUSE: return L"Pause";
	case VK_CAPITAL: return L"Caps Lock";
	case VK_ESCAPE: return L"Escape";
	case VK_SPACE: return L"Space";
	case VK_PRIOR: return L"Page Up";
	case VK_NEXT: return L"Page Down";
	case VK_END: return L"End";
	case VK_HOME: return L"Home";
	case VK_LEFT: return L"Left Arrow";
	case VK_UP: return L"Up Arrow";
	case VK_RIGHT: return L"Right Arrow";
	case VK_DOWN: return L"Down Arrow";
	case VK_SELECT: return L"Select";
	case VK_PRINT: return L"Print";
	case VK_EXECUTE: return L"Execute";
	case VK_SNAPSHOT: return L"Print screen";
	case VK_INSERT: return L"Insert";
	case VK_DELETE: return L"Delete";
	case VK_HELP: return L"Help";
	case '0': return L"0";
	case '1': return L"1";
	case '2': return L"2";
	case '3': return L"3";
	case '4': return L"4";
	case '5': return L"5";
	case '6': return L"6";
	case '7': return L"7";
	case '8': return L"8";
	case '9': return L"9";
	case 'A': return L"A";
	case 'B': return L"B";
	case 'C': return L"C";
	case 'D': return L"D";
	case 'E': return L"E";
	case 'F': return L"F";
	case 'G': return L"G";
	case 'H': return L"H";
	case 'I': return L"I";
	case 'J': return L"J";
	case 'K': return L"K";
	case 'L': return L"L";
	case 'M': return L"M";
	case 'N': return L"N";
	case 'O': return L"O";
	case 'P': return L"P";
	case 'Q': return L"Q";
	case 'R': return L"R";
	case 'S': return L"S";
	case 'T': return L"T";
	case 'U': return L"U";
	case 'V': return L"V";
	case 'W': return L"W";
	case 'X': return L"X";
	case 'Y': return L"Y";
	case 'Z': return L"Z";
	case VK_LWIN: return L"Left Windows";
	case VK_RWIN: return L"Right Windows";
	case VK_APPS: return L"Application";
	case VK_SLEEP: return L"Computer Sleep";
	case VK_NUMPAD0: return L"Numpad 0";
	case VK_NUMPAD1: return L"Numpad 1";
	case VK_NUMPAD2: return L"Numpad 2";
	case VK_NUMPAD3: return L"Numpad 3";
	case VK_NUMPAD4: return L"Numpad 4";
	case VK_NUMPAD5: return L"Numpad 5";
	case VK_NUMPAD6: return L"Numpad 6";
	case VK_NUMPAD7: return L"Numpad 7";
	case VK_NUMPAD8: return L"Numpad 8";
	case VK_NUMPAD9: return L"Numpad 9";
	case VK_MULTIPLY: return L"Numpad Multiply";
	case VK_ADD: return L"Numpad Add";
	case VK_SEPARATOR: return L"Numpad Separator";
	case VK_SUBTRACT: return L"Numpad Subtract";
	case VK_DECIMAL: return L"Numpad Decimal";
	case VK_DIVIDE: return L"Numpad Divide";
	case VK_F1: return L"F1";
	case VK_F2: return L"F2";
	case VK_F3: return L"F3";
	case VK_F4: return L"F4";
	case VK_F5: return L"F5";
	case VK_F6: return L"F6";
	case VK_F7: return L"F7";
	case VK_F8: return L"F8";
	case VK_F9: return L"F9";
	case VK_F10: return L"F10";
	case VK_F11: return L"F11";
	case VK_F12: return L"F12";
	case VK_F13: return L"F13";
	case VK_F14: return L"F14";
	case VK_F15: return L"F15";
	case VK_F16: return L"F16";
	case VK_F17: return L"F17";
	case VK_F18: return L"F18";
	case VK_F19: return L"F19";
	case VK_F20: return L"F20";
	case VK_F21: return L"F21";
	case VK_F22: return L"F22";
	case VK_F23: return L"F23";
	case VK_F24: return L"F24";
	case VK_NUMLOCK: return L"Num Lock";
	case VK_SCROLL: return L"Scroll Lock";
	case VK_LSHIFT: return L"Left Shift";
	case VK_RSHIFT: return L"Right Shift";
	case VK_LCONTROL: return L"Left Control";
	case VK_RCONTROL: return L"Right Control";
	case VK_LMENU: return L"Left Alt";
	case VK_RMENU: return L"Right Alt";
	case VK_BROWSER_BACK: return L"Browser Back";
	case VK_BROWSER_FORWARD: return L"Browser Forward";
	case VK_BROWSER_REFRESH: return L"Browser Refresh";
	case VK_BROWSER_STOP: return L"Browser Stop";
	case VK_BROWSER_SEARCH: return L"Browser Search";
	case VK_BROWSER_FAVORITES: return L"Browser Favorites";
	case VK_BROWSER_HOME: return L"Browser Start/Home";
	case VK_VOLUME_MUTE: return L"Volume Mute";
	case VK_VOLUME_DOWN: return L"Volume Down";
	case VK_VOLUME_UP: return L"Volume Up";
	case VK_MEDIA_NEXT_TRACK: return L"Media Next Track";
	case VK_MEDIA_PREV_TRACK: return L"Media Previous Track";
	case VK_MEDIA_STOP: return L"Media Stop";
	case VK_MEDIA_PLAY_PAUSE: return L"Media Play/Pause";
	case VK_LAUNCH_MAIL: return L"Start Mail";
	case VK_LAUNCH_MEDIA_SELECT: return L"Start Media";
	case VK_LAUNCH_APP1: return L"Launch Application 1";
	case VK_LAUNCH_APP2: return L"Launch Application 2";
	case VK_OEM_1: return L"Semicolon";
	case VK_OEM_PLUS: return L"Plus";
	case VK_OEM_COMMA: return L"Comma";
	case VK_OEM_MINUS: return L"Minus";
	case VK_OEM_PERIOD: return L"Period";
	case VK_OEM_2: return L"Forward Slash";
	case VK_OEM_3: return L"Grave/Tilde";
	case VK_OEM_4: return L"Left Brace";
	case VK_OEM_5: return L"Backslash";
	case VK_OEM_6: return L"Right Brace";
	case VK_OEM_7: return L"Apostrophe";
	case VK_OEM_8: return L"OEM 8";
	case VK_OEM_102: return L"OEM 102";
	case VK_PROCESSKEY: return L"Process Key";
	case VK_PACKET: return L"Unicode Keystroke Packet";
	case VK_ATTN: return L"Attn";
	case VK_CRSEL: return L"CrSel";
	case VK_EXSEL: return L"ExSel";
	case VK_EREOF: return L"Erase EOF";
	case VK_PLAY: return L"Play";
	case VK_ZOOM: return L"Zoom";
	case VK_PA1: return L"PA1";
	case VK_OEM_CLEAR: return L"Clear";
	default:
		return std::to_wstring(key);
	}
}