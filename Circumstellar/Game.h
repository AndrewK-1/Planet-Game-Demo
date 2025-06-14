#pragma once
#include "pch.h"

class Game {
public:
	//Constructor that is declared as not expecting exceptions
	Game() noexcept;
	//Defining the default destructor without having to write it out explicitly.
	~Game() = default;
	//Prohibits copying
	Game(Game const&) = delete;
	Game& operator= (const Game&) = delete;
	//Default copy behavior, using rvalue reference.
	//&& is a move semantic in which large objects can be "moved" to a new reference.
	//This prevents copying huge amounts of data to initialize a new object with an old, soon-to-be-deleted object's contents.
	//A Game pointer would take this Game's contents and move it to the new Game pointer without copying it.
	Game(Game&&) = default;
	Game& operator=(Game&&) = default;

private:
	void Initialize(HWND windowHandle);
	void CreateResources();

	//Resources
	HWND										m_windowHandle;
	int											m_width;
	int											m_height;
	Microsoft::WRL::ComPtr<ID3D11Device>		m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>		m_swapChain;
};