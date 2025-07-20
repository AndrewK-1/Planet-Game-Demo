#pragma once
#include "pch.h"
#include "Menus.h"
#include "Game.h"

Menus::Menus(int screenSizeH, int screenSizeV, Game* game) {
	m_screenSizeH = screenSizeH;
	m_screenSizeV = screenSizeV;
	m_game = game;
}

void Menus::AddButton(MenuButton button) {
	p_buttons.push_back(button);
}

void Menus::SetMenuFrame(int posX1, int posY1, int posX2, int posY2) {
	m_menuFrame = D2D1::RectF(posX1, posY1, posX2, posY2);
}

bool MenuButton::IsPointInButton(int posX, int posY) {
	if(posX >= m_buttonRectangle.left && posX <= m_buttonRectangle.right &&
		posY >= m_buttonRectangle.top && posY <= m_buttonRectangle.bottom) {
		return true;
	}
	return false;
}

MenuButton::MenuButton(std::wstring text, int buttonPlacementH, int buttonPlacementV, int paddingH, int paddingV, int width, int height, int screenSizeH, int screenSizeV) {
	int posX1, posX2, posY1, posY2;
	if (buttonPlacementH == BUTTON_H_LEFT) {
		posX1 = paddingH;
		posX2 = paddingH + width;
	}
	else if (buttonPlacementH == BUTTON_H_CENTER){
		posX1 = screenSizeH / 2 - width / 2 + paddingH;
		posX2 = screenSizeH / 2 + width / 2 + paddingH;
	}
	else {
		posX1 = screenSizeH - paddingH - width + paddingH;
		posX2 = screenSizeH - paddingH + paddingH;
	}

	if (buttonPlacementV == BUTTON_V_TOP) {
		posY1 = paddingV;
		posY2 = paddingV + height;
	}else if (buttonPlacementV == BUTTON_V_CENTER) {
		posY1 = screenSizeV / 2 - height / 2 + paddingV;
		posY2 = screenSizeV / 2 + height / 2 + paddingV;
	}
	else {
		posY1 = screenSizeV - paddingV - height;
		posY2 = screenSizeV - paddingV;
	}

	m_buttonRectangle = D2D1::RectF(posX1, posY1, posX2, posY2);
	m_buttonText = text;
}

void MenuButton::SetText(std::wstring text) {
	m_buttonText = text;
}

D2D1_RECT_F MenuButton::GetButtonRectangle() {
	return m_buttonRectangle;
}

std::wstring MenuButton::GetButtonText(){
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

MainMenu::MainMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, game) {
	SetMenuFrame(0, 0, screenSizeH, screenSizeV);

	MenuButton exitButton(L"Exit", 0, 2, 100, 100, 200, 50, screenSizeH, screenSizeV);
	exitButton.SetOnClickFunction([this]() {m_game->Exit(); });

	MenuButton settingsMenuButton(L"Settings", 0, 2, 100, 200, 200, 50, screenSizeH, screenSizeV);
	settingsMenuButton.SetOnClickFunction([this]() {m_game->OpenSettingsMenu(); });
	
	MenuButton loadButton(L"Load Game", 0, 2, 100, 300, 200, 50, screenSizeH, screenSizeV);
	loadButton.SetOnClickFunction([this]() {m_game->LoadWorld(); });

	MenuButton newGameButton(L"New Game", 0, 2, 100, 400, 200, 50, screenSizeH, screenSizeV);
	newGameButton.SetOnClickFunction([this]() {m_game->NewWorld(); });

	p_buttons.push_back(exitButton);
	p_buttons.push_back(settingsMenuButton);
	p_buttons.push_back(loadButton);
	p_buttons.push_back(newGameButton);
}

InGameMenu::InGameMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, game) {
	int x1, y1, x2, y2;
	x1 = screenSizeH / 2 - 200; y1 = screenSizeV / 2 - 300; x2 = screenSizeH / 2 + 200; y2 = screenSizeV / 2 + 300;
	SetMenuFrame(x1, y1, x2, y2);

	MenuButton exitButton(L"Exit Game", 1, 1, 0, 100, 150, 100, screenSizeH, screenSizeV);
	exitButton.SetOnClickFunction([this]() {m_game->ExitWorld(); });

	MenuButton settingsButton(L"Settings", 1, 1, 0, -100, 150, 100, screenSizeH, screenSizeV);
	settingsButton.SetOnClickFunction([this]() {m_game->OpenSettingsMenu(); });
	
	p_buttons.push_back(exitButton);
	p_buttons.push_back(settingsButton);
}

SettingsMenu::SettingsMenu(int screenSizeH, int screenSizeV, Game* game) : Menus(screenSizeH, screenSizeV, game) {
	SetMenuFrame(screenSizeH / 2 - 250, screenSizeV / 2 - 250, screenSizeH / 2 + 250, screenSizeV / 2 + 250);
}