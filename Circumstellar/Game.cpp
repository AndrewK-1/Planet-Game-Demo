#include "pch.h"
#include "Game.h"

Game::Game() noexcept :
	m_width(800),
	m_height(600)
{
}

void Game::Initialize(HWND windowHandle) {
	m_windowHandle = windowHandle;

	CreateResources();
}

void Game::CreateResources() {
	const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM; //32-bit color format RGBA
	constexpr UINT backBufferCount = 2;
	//If swap chain exists, do nothing, otherwise create it
	if (m_swapChain) {

	}
	else {
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc.Width = m_width;			//UINT
		swapChainDesc.BufferDesc.Height = m_height;			//UINT
		swapChainDesc.BufferDesc.Format = backBufferFormat;	//DXGI_FORMAT
		swapChainDesc.BufferDesc.RefreshRate;				//DXGI_RATIONAL
		//swapChainDesc.SampleDesc;							//DXGI_SAMPLE_DESC
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//DXGI_USAGE. Describes surface usage and CPU access options.
		swapChainDesc.BufferCount = backBufferCount;		//UINT
		swapChainDesc.OutputWindow = m_windowHandle;		//HWND, put window handle here
		swapChainDesc.Windowed = true;						//BOOL
		//swapChainDesc.SwapEffect;							//DXGI_SWAP_EFFECT
		//swapChainDesc.Flags;								//UINT
		
	}
	
}

