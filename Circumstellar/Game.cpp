#pragma once
#include "pch.h"
#include "Game.h"

using Microsoft::WRL::ComPtr;

Game::Game() noexcept :
	m_windowHandle(nullptr),
	m_screenWidth(800),
	m_screenHeight(600),
	m_feature_level(D3D_FEATURE_LEVEL_9_1)
{
}

void Game::Tick() {
	Update();
	Render();
}

void Game::Update() {
	//Game logic.
	//The goal is to let game logic to operate at its own pace,
	//while rendering operates when it needs to.
}

void Game::Render() {
	Clear();
	Present();
}

//For clearing back buffers
void Game::Clear() {

	//Clear views
	using RGBA = float[4];
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), /*DirectX::Colors::CornflowerBlue */  RGBA{0.1f, 0.1f, 0.4f, 1.0f});
	//Below statement is for depth stencils
	////m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//Set render targets
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	//Set viewport
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(m_screenWidth), static_cast<float>(m_screenHeight), 0.f, 1.f };
	m_deviceContext->RSSetViewports(1, &viewport);
}

void Game::Present() {
	//Set the first variable to 1 to wait for VSYNC, preventing any frames from being rendered between displays to the monitor.
	HRESULT presentResult = m_swapChain->Present(0, 0);

	//In case the device was reset (could be due to undocking a device or some other action)
	if (presentResult == DXGI_ERROR_DEVICE_REMOVED || presentResult == DXGI_ERROR_DEVICE_RESET) {
		OnDeviceLost();
	}
	else {
		DX::ThrowIfFailed(presentResult);
	}
}

void Game::Initialize(HWND windowHandle) {
	m_windowHandle = windowHandle;
	CreateDevice();
	CreateResources();
}

void Game::CreateDevice() {
	UINT createFlags = 0;

#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		//Supported Direct3D feature levels.  These are the ones present in the VS DirectX template.  Technically it could be null.
		//I'll adjust this later as I familiarize myself with what each feature level does/is needed.
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	//Create DX11 API device object.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	DX::ThrowIfFailed(D3D11CreateDevice(
		nullptr,										//Adapter
		D3D_DRIVER_TYPE_HARDWARE,						//Specify to use the GPU or other graphics card.
		nullptr,										//used with D3D_DRIVER_TYPE_SOFTWARE to set software code for rendering by software instead of hardware.
		createFlags,									//Flags
		featureLevels,									//Supported feature levels
		static_cast<UINT>(std::size(featureLevels)),	//Number of feature levels present
		D3D11_SDK_VERSION,								//Tells programs which version of DirectX is being used.  For compatibility
		device.ReleaseAndGetAddressOf(),				//Releases the device, and gets its address to pass an ID3D11Device
		&m_feature_level,								//Pointer to feature level variable.
		deviceContext.ReleaseAndGetAddressOf()			//Release device context, and gets its address.
		//If COM objects are not released, they can stay in memory even when the program closes, so COM objects must be released at some point.
	));

	DX::ThrowIfFailed(device.As(&m_device));
	DX::ThrowIfFailed(deviceContext.As(&m_deviceContext));
}

void Game::CreateResources() 
{
	//Clear previous window size-specific contexts
	m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_deviceContext->Flush();

	const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM; //32-bit color format RGBA
	constexpr UINT backBufferCount = 2;
	//If swap chain exists, do nothing, otherwise create it
	if (m_swapChain) {

	}
	else {
		//More DirectX template stuff
		//Retrieve dxgi device from D3D device
		ComPtr<IDXGIDevice>	dxgiDevice;
		DX::ThrowIfFailed(m_device.As(&dxgiDevice));
		//Identify physical adapter device is running on
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
		//Obtain the factory object that created the device
		ComPtr<IDXGIFactory> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = m_screenWidth;					//UINT
		swapChainDesc.BufferDesc.Height = m_screenHeight;				//UINT
		swapChainDesc.BufferDesc.Format = backBufferFormat;				//DXGI_FORMAT
		swapChainDesc.BufferDesc.RefreshRate;							//DXGI_RATIONAL
		swapChainDesc.SampleDesc.Count = 1;								//Part of DXGI_SAMPLE_DESC, for anti-aliasing sampling count
		swapChainDesc.SampleDesc.Quality = 0;							//UINT
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//DXGI_USAGE. Describes surface usage and CPU access options.
		swapChainDesc.BufferCount = backBufferCount;					//UINT
		swapChainDesc.OutputWindow = m_windowHandle;					//HWND, put window handle here
		swapChainDesc.Windowed = true;									//BOOL
		//swapChainDesc.SwapEffect;										//DXGI_SWAP_EFFECT
		//swapChainDesc.Flags;											//UINT
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//Allow switch to full screen
		
		//Create Swapchain
		DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(
			m_device.Get(),
			&swapChainDesc,
			m_swapChain.ReleaseAndGetAddressOf()
		));

		//COM pointers should be released, but WRL makes it so that this is not always necessary.
		ComPtr<ID3D11Texture2D> backBuffer; //Stores a flat image
		//IID_PPV_ARGS retrieves the Interface ID of a COM, and retrieves a pointer to the interface.
		//Alternative line // DX::ThrowIfFailed(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf()));
		DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

		//Use back buffer address to create a render target
		DX::ThrowIfFailed(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

		//Set render target for device context to use in pipeline
		m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), NULL);
	}
}

//If a device is somehow lost or disconnected, cleanup and creation of a new device and resources occurs
void Game::OnDeviceLost() {

	//Further Direct3d resource cleanup can go here.

	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_deviceContext.Reset();
	m_device.Reset();


	CreateDevice();
	CreateResources();
}

void Game::GetDefaultSize(int& width, int& height) 
{
	width = 800;
	height = 600;
}

void Game::OnWindowSizeChanged(int width, int height) {
	if (!m_windowHandle) {
		return;
	}

	m_screenWidth = std::max(width, 1); //Window width cannot be less than 1
	m_screenHeight = std::max(height, 1); //Window length cannot be less than 1

	CreateResources();
}



//Suggested message handlers by DirectX template
void Game::OnActivated() {
	//When game becomes active window (Opportunity to ignore the first message received, likely due to clicking into the window)
}

void Game::OnDeactivated() {
	//Game becomes background window (Opportunity to auto-pause the game if necessary)
}

void Game::OnSuspending() {
	//Game is being power-suspended or minimized (Opportunity to auto-pause the game)
}

void Game::OnResuming() {
	//Game is power-resumed or returned from minimization
}

void Game::OnClosing() {
	m_swapChain->SetFullscreenState(FALSE, NULL);
}