#pragma once
#include "pch.h"
#include "Menus.h"
#include "Game.h"

bool MenuButton::IsPointInButton(int posX, int posY) {
	if (posX >= m_buttonRectangle.left && posX <= m_buttonRectangle.right &&
		posY >= m_buttonRectangle.top && posY <= m_buttonRectangle.bottom) {
		return true;
	}
	return false;
}

MenuButton::MenuButton(
	std::wstring text, int buttonPlacementH, int buttonPlacementV, int paddingH, int paddingV, int width, int height, D2D1_RECT_F menuFrame, float menuScale) {
	int posX1, posX2, posY1, posY2;
	m_buttonAlignentH = buttonPlacementH;
	m_buttonAlignentV = buttonPlacementV;
	m_paddingH = paddingH;
	m_paddingV = paddingV;
	m_width = width;
	m_height = height;

	if (buttonPlacementH == BUTTON_H_LEFT) {
		posX1 = paddingH + menuFrame.left;
		posX2 = paddingH + width + menuFrame.left;
	}
	else if (buttonPlacementH == BUTTON_H_CENTER) {
		posX1 = (menuFrame.right - menuFrame.left) / 2 - width / 2 + paddingH + menuFrame.left;
		posX2 = (menuFrame.right - menuFrame.left) / 2 + width / 2 + paddingH + menuFrame.left;
	}
	else {
		posX1 = menuFrame.right - paddingH - width + paddingH;
		posX2 = menuFrame.right - paddingH + paddingH;
	}

	if (buttonPlacementV == BUTTON_V_TOP) {
		posY1 = paddingV + menuFrame.top;
		posY2 = paddingV + height + menuFrame.top;
	}
	else if (buttonPlacementV == BUTTON_V_CENTER) {
		posY1 = (menuFrame.bottom - menuFrame.top) / 2 - height / 2 + paddingV + menuFrame.top;
		posY2 = (menuFrame.bottom - menuFrame.top) / 2 + height / 2 + paddingV + menuFrame.top;
	}
	else {
		posY1 = menuFrame.bottom - paddingV - height;
		posY2 = menuFrame.bottom - paddingV;
	}

	m_buttonRectangle = D2D1::RectF(posX1, posY1, posX2, posY2);
	m_buttonText = text;
	RecalculateButtonSize(menuScale, menuFrame);
}

void MenuButton::RecalculateButtonSize(float menuScale, D2D1_RECT_F menuFrame) {
	int posX1, posX2, posY1, posY2;
	int paddingH = static_cast<float>(m_paddingH) * menuScale, paddingV = static_cast<float>(m_paddingV) * menuScale;
	int width = static_cast<float>(m_width) * menuScale, height = static_cast<float>(m_height) * menuScale;

	if (m_buttonAlignentH == BUTTON_H_LEFT) {
		posX1 = paddingH + menuFrame.left;
		posX2 = paddingH + width + menuFrame.left;
	}
	else if (m_buttonAlignentH == BUTTON_H_CENTER) {
		posX1 = (menuFrame.right - menuFrame.left) / 2 - width / 2 + paddingH + menuFrame.left;
		posX2 = (menuFrame.right - menuFrame.left) / 2 + width / 2 + paddingH + menuFrame.left;
	}
	else {
		posX1 = menuFrame.right - paddingH - width + paddingH;
		posX2 = menuFrame.right - paddingH + paddingH;
	}

	if (m_buttonAlignentV == BUTTON_V_TOP) {
		posY1 = paddingV + menuFrame.top;
		posY2 = paddingV + height + menuFrame.top;
	}
	else if (m_buttonAlignentV == BUTTON_V_CENTER) {
		posY1 = (menuFrame.bottom - menuFrame.top) / 2 - height / 2 + paddingV + menuFrame.top;
		posY2 = (menuFrame.bottom - menuFrame.top) / 2 + height / 2 + paddingV + menuFrame.top;
	}
	else {
		posY1 = menuFrame.bottom - paddingV - height;
		posY2 = menuFrame.bottom - paddingV;
	}

	m_buttonRectangle = D2D1::RectF(posX1, posY1, posX2, posY2);
}

MenuButton::MenuButton(std::wstring text, int buttonPlacementH, int buttonPlacementV, int paddingH, int paddingV, int width, int height, D2D1_RECT_F menuFrame, std::function<void()> onClickFunction, float menuScale) 
	: MenuButton(text, buttonPlacementH, buttonPlacementV, paddingH, paddingV, width, height, menuFrame, menuScale) 
{
	SetOnClickFunction(onClickFunction);
}

void MenuButton::SetText(std::wstring text) {
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

MenuButton Menus::GetButton(int index) {
	return p_buttons.at(index);
}

D2D1_RECT_F Menus::GetMenuFrame() {
	return m_menuFrame;
};

float Menus::GetMenuScale() {
	return m_menuScale;
}

bool Menus::ClickButton(int posX, int posY) {
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

MainMenu::MainMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 1920, 1080, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();

	MenuButton exitButton(L"Exit", 0, 2, 100, 100, 200, 50, menuFrame, [this]() {m_game->Exit(); }, GetMenuScale());
	MenuButton settingsMenuButton(L"Settings", 0, 2, 100, 200, 200, 50, menuFrame, [this]() {m_game->OpenSettingsMenu(); }, GetMenuScale());
	MenuButton loadButton(L"Load Game", 0, 2, 100, 300, 200, 50, menuFrame, [this]() {m_game->LoadWorld(); }, GetMenuScale());
	MenuButton newGameButton(L"New Game", 0, 2, 100, 400, 200, 50, menuFrame, [this]() {m_game->NewWorld(); }, GetMenuScale());

	p_buttons.push_back(exitButton);
	p_buttons.push_back(settingsMenuButton);
	p_buttons.push_back(loadButton);
	p_buttons.push_back(newGameButton);
}

InGameMenu::InGameMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 400, 450, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();

	MenuButton saveGameButton(L"Save Game", 1, 1, 0, -150, 300, 50, menuFrame, GetMenuScale());
	saveGameButton.SetOnClickFunction([this]() {m_game->SaveWorld(); });

	MenuButton settingsButton(L"Settings", 1, 1, 0, -50, 300, 50, menuFrame, GetMenuScale());
	settingsButton.SetOnClickFunction([this]() {m_game->OpenSettingsMenu(); });

	MenuButton closeButton(L"Close Menu", 1, 1, 0, 50, 300, 50, menuFrame, GetMenuScale());
	closeButton.SetOnClickFunction([this]() {m_game->CloseTopmostMenu(); });

	MenuButton exitButton(L"Exit Game", 1, 1, 0, 150, 300, 50, menuFrame, GetMenuScale());
	exitButton.SetOnClickFunction([this]() {m_game->ExitWorld(); });

	p_buttons.push_back(saveGameButton);
	p_buttons.push_back(settingsButton);
	p_buttons.push_back(closeButton);
	p_buttons.push_back(exitButton);
}

SettingsMenu::SettingsMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 500, 500, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();

	MenuButton graphicsButton(L"Graphics Settings", 1, 1, 0, -100, 300, 50, menuFrame, GetMenuScale());
	graphicsButton.SetOnClickFunction([this]() {m_game->OpenGraphicsSettingsMenu(); });

	MenuButton keybindButton(L"Key Bindings", 1, 1, 0, 0, 300, 50, menuFrame, GetMenuScale());
	keybindButton.SetOnClickFunction([this]() {m_game->OpenKeybindMenu(); });

	MenuButton closeButton(L"Close Menu", 1, 1, 0, 100, 300, 50, menuFrame, GetMenuScale());
	closeButton.SetOnClickFunction([this]() {m_game->CloseTopmostMenu(); });

	p_buttons.push_back(graphicsButton);
	p_buttons.push_back(keybindButton);
	p_buttons.push_back(closeButton);
}

GraphicsSettingsMenu::GraphicsSettingsMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 800, 800, game) {
	D2D1_RECT_F menuFrame = GetMenuFrame();

	MenuButton res1920x1080Button(L"1920 x 1080", 0, 0, 50, 100, 300, 50, menuFrame, [this]() {m_game->SetResolution(1920, 1080); }, GetMenuScale());
	MenuButton res1440x900Button(L"1440 x 900", 0, 0, 50, 150, 300, 50, menuFrame, [this]() {m_game->SetResolution(1440, 900); }, GetMenuScale());
	MenuButton res800x600Button(L"800 x 600", 0, 0, 50, 200, 300, 50, menuFrame, [this]() {m_game->SetResolution(800, 600); }, GetMenuScale());

	p_buttons.push_back(res1920x1080Button);
	p_buttons.push_back(res1440x900Button);
	p_buttons.push_back(res800x600Button);
}
KeybindMenu::KeybindMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, 600, 800, game) {

}