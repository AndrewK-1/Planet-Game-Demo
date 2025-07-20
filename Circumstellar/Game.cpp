#pragma once
#include "pch.h"
#include "Game.h"
#include <d3dcompiler.h>
#include <WindowsNumerics.h>
#include <debugapi.h>
#include <chrono>
#include <thread>
#include "Camera.h"
#include "GraphicsObject.h"
#include "WorldObject.h"
#include "Block.h"
#include "Planet.h"


using Microsoft::WRL::ComPtr;
using namespace DirectX;

Game::Game() noexcept :
	m_windowHandle(nullptr),
	m_screenWidth(1440),
	m_screenHeight(900),
	m_feature_level(D3D_FEATURE_LEVEL_10_0),
	float4x4Data(
		{ {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f} }),
	updatePlanetGeometryFlag(0),
	m_planetVertexCount(0),
	m_planetIndexCount(0),
	m_currentTool(1),
	m_FOV(3.14159f / 4.0f),
	m_settingsFileName("GraphicsSettings.txt"),
	m_worldLoaded(false),
	m_menuActive(true)
{
	OpenMainMenu();
	m_world1 = std::make_unique<World>();
	camera = std::make_unique<Camera>();
	OutputDebugString(L"Creating settings object\n");

	//User Settings input
	m_settingsIO = std::make_unique<GraphicsSettingsIO>();
	OutputDebugString(L"Receiving resolution settings.\n");
	std::string setting;
	if (m_settingsIO->GetSetting(m_settingsFileName, "ResolutionWidth", setting)){
		m_screenWidth = std::stoi(setting);
	}
	else {
		m_settingsIO->SetSetting(m_settingsFileName, "ResolutionWidth", std::to_string(m_screenWidth));
	}

	if (m_settingsIO->GetSetting(m_settingsFileName, "ResolutionHeight", setting)) {
		m_screenHeight = std::stoi(setting);
	}
	else {
		m_settingsIO->SetSetting(m_settingsFileName, "ResolutionHeight", std::to_string(m_screenHeight));
	}

	OutputDebugString(L"FOV settings.\n");
	if (m_settingsIO->GetSetting(m_settingsFileName, "FieldOfView", setting)) {
		m_FOV = std::stoi(setting);
	}
	else {
		m_settingsIO->SetSetting(m_settingsFileName, "FieldOfView", std::to_string(m_FOV));
	}
}

void Game::Initialize(HWND windowHandle) {
	m_windowHandle = windowHandle;
	CreateDevice();
	CreateResources();

	//if (m_IOHandler.DoesPlanetExist() == 0) {
	//	m_world1->AddPlanet(XMQuaternionIdentity(), 10.0f);
	//}
	if (m_world1->CheckIfLoaded()) {
		InitializeShaders();
	}
}

void Game::Tick() {
	Render();
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
	Update();
}

void Game::Update() {
	//Game logic.
	//The goal is to let game logic to operate at its own pace,
	//while rendering operates when it needs to.
	
	if (m_worldLoaded == true) {
		//Update physics
		for (int i = 0; i < m_world1->GetSpaceshipCount(); i++) {
			m_world1->GetSpaceship(i)->ApplyPhysics();
		}

		if (m_world1->GetPlayer()->GetMounted() == 0) {
			m_world1->GetPlayer()->ApplyPhysics();
		}
		else {
			m_world1->GetPlayer()->SetObjectPos(m_world1->GetPlayer()->GetMounted()->GetObjectPos());
			m_world1->GetPlayer()->SetObjectRot(m_world1->GetPlayer()->GetMounted()->GetObjectRot());
		}

		//Instancing for terrain modification cursor (isosphere)
		XMMATRIX defaultInstanceMatrix = XMMatrixIdentity();
		//Getting the player position to lock camera to it
		XMFLOAT4 playerPos, playerRot;
		playerPos = m_world1->GetPlayer()->GetObjectPos();
		playerRot = m_world1->GetPlayer()->GetObjectRot();
		XMVECTOR playerPosVec, playerRotVec;
		playerPosVec = XMLoadFloat4(&playerPos);
		playerRotVec = XMLoadFloat4(&playerRot);
		camera->SetPosition(playerPosVec, playerRotVec);
		XMMATRIX camRayMatrix = XMMatrixAffineTransformation(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			camera->GetForwardRay(10.0f));
		//Get array of cube matrix positions
		std::vector<XMMATRIX> cubeWorldMatrix;
		for (int i = 0; i < m_world1->GetBlockCount(); i++) {
			cubeWorldMatrix.push_back(m_world1->GetBlockMatrix(i));
		}
		//Get array of spaceship matrix positions
		std::vector<XMMATRIX> spaceshipWorldMatrix;
		for (int i = 0; i < m_world1->GetSpaceshipCount(); i++) {
			cubeWorldMatrix.push_back(m_world1->GetSpaceshipMatrix(i));
		}

		//Actually pushing matrices into the instance array
		std::vector<DirectX::XMMATRIX> instanceMatrices;
		instanceMatrices.push_back(defaultInstanceMatrix);
		//Camera and player using the isosphere model
		instanceMatrices.push_back(camRayMatrix);

		instanceMatrices.push_back(m_world1->GetPlayer()->GetObjectMatrix());
		//Maybe change this later.  Right now each cube matrix is being added as its own instance.
		//This is probably correct, but I'll have to be careful about specifying the instance index when addding additional multi-instanced objects
		for (int i = 0; i < cubeWorldMatrix.size(); i++) {
			instanceMatrices.push_back(cubeWorldMatrix[i]);
		}
		for (int i = 0; i < spaceshipWorldMatrix.size(); i++) {
			instanceMatrices.push_back(spaceshipWorldMatrix[i]);
		}

		int instanceCounter = instanceMatrices.size();
		D3D11_BUFFER_DESC instanceDesc = {};
		instanceDesc.Usage = D3D11_USAGE_DEFAULT;
		instanceDesc.ByteWidth = sizeof(XMMATRIX) * instanceCounter;
		instanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA instanceData = { instanceMatrices.data(), 0, 0 };
		m_device->CreateBuffer(&instanceDesc, &instanceData, &m_instanceBuffer);
		UpdateGraphicsBuffers();


		//Reestablish its planet geometry in the GPU
		m_planetVertexCount = GetPlanet(0)->GetVertexCount();
		m_planetIndexCount = m_planetVertexCount;

		MatrixData matData;
		//Load transform matrices
		matData.worldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&float4x4Data.worldMatrix));
		matData.viewMatrix = camera->getCameraMatrix();
		matData.viewMatrix = XMMatrixTranspose(matData.viewMatrix);
		matData.perspectiveMatrix = XMMatrixTranspose(XMLoadFloat4x4(&float4x4Data.perspectiveMatrix));

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		DX::ThrowIfFailed(m_deviceContext->Map(m_constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		CopyMemory(mappedResource.pData, &matData, sizeof(MatrixData));
		m_deviceContext->Unmap(m_constBuffer.Get(), 0);

		m_deviceContext->VSSetConstantBuffers(0, 1, m_constBuffer.GetAddressOf());
	}
}

void Game::Render() {
	Clear();

	//Render code

	if (m_worldLoaded == true) {
		//Index for where each index array and vertex array start to tell the gpu how to draw shapes.
		UINT planetIndex = 0;
		UINT planetVertex = 0;

		UINT cubeIndex = m_planetIndexCount;
		UINT cubeVertex = m_planetVertexCount;

		UINT isoSphereIndex = cubeIndex + m_cubeIndexCount;
		UINT isoSphereVertex = cubeVertex + m_cubeVertexCount;

		UINT spaceshipIndex = isoSphereIndex + m_isoSphereIndexCount;
		UINT spaceshipVertex = isoSphereVertex + m_isoSphereVertexCount;

		//Preparing proper alignment of instance data for drawing
		UINT planetInstances = m_world1->GetPlanetCount();
		UINT planetInstanceStart = 0;
		UINT cameraToolRayInstances = 1;
		UINT cameraToolRayInstanceStart = planetInstances;
		UINT playerInstances = 1;
		UINT playerInstanceStart = cameraToolRayInstanceStart + cameraToolRayInstances;
		UINT cubeInstances = m_world1->GetBlockCount();
		UINT cubeInstanceStart = playerInstanceStart + playerInstances;
		UINT spaceshipInstances = m_world1->GetSpaceshipCount();
		UINT spaceshipInstanceStart = cubeInstanceStart + cubeInstances;

		//Show planet
		m_deviceContext->DrawIndexedInstanced(m_planetVertexCount, planetInstances, 0, 0, planetInstanceStart);
		//Choose which tool to show
		switch (m_currentTool) {
		case 1: {
			m_deviceContext->DrawIndexedInstanced(m_isoSphereIndexCount, cameraToolRayInstances, isoSphereIndex, isoSphereVertex, cameraToolRayInstanceStart);
			break;
		}
		case 2: {
			m_deviceContext->DrawIndexedInstanced(m_cubeIndexCount, cameraToolRayInstances, cubeIndex, cubeVertex, cameraToolRayInstanceStart);
			break;
		}
		case 3: {
			m_deviceContext->DrawIndexedInstanced(m_spaceshipIndexCount, cameraToolRayInstances, spaceshipIndex, spaceshipVertex, cameraToolRayInstanceStart);
			break;
		}
		}
		//Show Player
		////m_deviceContext->DrawIndexedInstanced(m_isoSphereIndexCount, playerInstances, isoSphereIndex, isoSphereVertex, playerInstanceStart);
		//Show cubes
		m_deviceContext->DrawIndexedInstanced(m_cubeIndexCount, cubeInstances, cubeIndex, cubeVertex, cubeInstanceStart);
		//Show spaceships
		m_deviceContext->DrawIndexedInstanced(m_spaceshipIndexCount, spaceshipInstances, spaceshipIndex, spaceshipVertex, spaceshipInstanceStart);
	}
	
	//2D Rendering
	m_2dRenderTarget->BeginDraw();
	
	//Draw menus
	for (int i = 0; i < m_menuStack.size(); i++) {
		//Draw menu frame, then menu buttons
		m_2dRenderTarget->FillRectangle(m_menuStack.at(i)->GetMenuFrame(), m_2dBrushSolidBlue.Get());
		m_2dRenderTarget->DrawRectangle(m_menuStack.at(i)->GetMenuFrame(), m_2dBrush.Get());
		for (int j = 0; j < m_menuStack.at(i)->GetButtonCount(); j++) {
			MenuButton button = m_menuStack.at(i)->GetButton(j);
			//Draw button rectangle
			m_2dRenderTarget->DrawRectangle(button.GetButtonRectangle(), m_2dBrush.Get());
			m_2dRenderTarget->DrawText(
				button.GetButtonText().c_str(), 
				button.GetButtonText().length(), 
				m_textFormat.Get(), 
				button.GetButtonRectangle(),
				m_2dBrush.Get());
		}
	}
	//End 2D drawing
	m_2dRenderTarget->EndDraw();

	Present();
}

//For clearing back buffers
void Game::Clear() {

	//Clear views
	float backgroundColor[4] = { 0.05f, 0.05f, 0.05f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), backgroundColor);
	//Below statement is for depth stencils
	////m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//Set render targets
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), NULL);

	//Set viewport
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(m_screenWidth), static_cast<float>(m_screenHeight), 0.0f, 1.0f };
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

void Game::CreateDevice() {
	UINT createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

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
		D3D_FEATURE_LEVEL_10_0
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
		//(Note, in this context the objects are released because they are being overwritten.  If a COM object is not overwritten, a it should not be preemtively Release()'ed)
	));

	DX::ThrowIfFailed(device.As(&m_device));
	DX::ThrowIfFailed(deviceContext.As(&m_deviceContext));
}

void Game::CreateResources() 
{
	//Clear previous window size-specific contexts
	m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_renderTargetView.Reset();
	//m_depthStencilView.Reset();
	m_deviceContext->Flush();

	const UINT backBufferWidth = static_cast<UINT>(m_screenWidth);
	const UINT backBufferHeight = static_cast<UINT>(m_screenHeight);
	const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM; //32-bit color format RGBA
	constexpr UINT backBufferCount = 2;
	//If swap chain exists, do nothing, otherwise create it
	if (m_swapChain) {
		OutputDebugString(L" Resizing buffers.\n");
		DX::ThrowIfFailed(m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
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
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//Allow switch to full screen with Alt+Enter
		
		//Create Swapchain
		DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(
			m_device.Get(),
			&swapChainDesc,
			m_swapChain.ReleaseAndGetAddressOf()
		));
	}

	//COM pointers should be released, but WRL makes it so that this is not always necessary.
	//Also COM pointers should not use Release(), as the Com object will handle releasing, and if already released will cause an error on releasing a nullptr.
	ComPtr<ID3D11Texture2D> backBuffer; //Stores a flat image
	//IID_PPV_ARGS retrieves the Interface ID of a COM, and retrieves a pointer to the interface.
	//Alternative line // DX::ThrowIfFailed(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf()));
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	//Use back buffer address to create a render target
	DX::ThrowIfFailed(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

	//Set render target for device context to use in pipeline
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), NULL);


	//Initiatizing 2D elements
	DX::ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_2dFactory.GetAddressOf()));

	Microsoft::WRL::ComPtr<IDXGISurface> surfaceBackBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(surfaceBackBuffer.GetAddressOf())));

	//2D Render Target Properties
	D2D1_RENDER_TARGET_PROPERTIES targetProperties2D = {};
	targetProperties2D.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	//DXIG surface render target and DXGI surface must use the same format. UNKNOWN automatically uses surface's format.
	targetProperties2D.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	targetProperties2D.dpiX = 0;
	targetProperties2D.dpiY = 0;

	//Use factory to create render target
	DX::ThrowIfFailed(m_2dFactory->CreateDxgiSurfaceRenderTarget(surfaceBackBuffer.Get(), &targetProperties2D, m_2dRenderTarget.ReleaseAndGetAddressOf()));

	//Create a brush
	m_2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cyan), m_2dBrush.GetAddressOf());
	m_2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkBlue), m_2dBrushSolidBlue.GetAddressOf());

	//Text formatting
	ComPtr<IDWriteFactory> writeFactory;
	DX::ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(writeFactory.GetAddressOf())));
	writeFactory->CreateTextFormat(
		L"Helvetica",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		32.0f,
		L"en-us",
		m_textFormat.ReleaseAndGetAddressOf()
	);
}

//If a device is somehow lost or disconnected, cleanup and creation of a new device and resources occurs
void Game::OnDeviceLost() {

	//Further Direct3d resource cleanup can go here.

	//m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_deviceContext.Reset();
	m_device.Reset();
	m_inputLayout.Reset();
	m_pVBuffer.Reset();

	CreateDevice();
	CreateResources();
	if (m_world1->CheckIfLoaded()) {
		InitializeShaders();
	}
}

void Game::GetDefaultSize(int& width, int& height) 
{
	OutputDebugString(L"Copying default window size.");
	width = 1920;
	height = 1080;
}

void Game::GetCurrentSize(int& width, int& height) {
	OutputDebugString(L"Copying current window size.");
	width = m_screenWidth;
	height = m_screenHeight;
}

void Game::OnWindowSizeChanged(int width, int height) {
	if (!m_windowHandle) {
		return;
	}
	OutputDebugString(L" Handle exists.\n");
	m_screenWidth = std::max(width, 1); //Window width cannot be less than 1
	OutputDebugString(L" New Width: "); OutputDebugString(std::to_wstring(m_screenWidth).c_str()); OutputDebugString(L"\n");
	m_screenHeight = std::max(height, 1); //Window length cannot be less than 1
	OutputDebugString(L" New Height: "); OutputDebugString(std::to_wstring(m_screenHeight).c_str()); OutputDebugString(L"\n");
	//Resize perspective matrix
	XMStoreFloat4x4(&float4x4Data.perspectiveMatrix, XMMatrixPerspectiveFovLH(m_FOV, (static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight)), 0.1f, 1000.0f));

	CreateResources();
}

//Suggested methods to implement by DirectX template
void Game::OnActivated() {
	OutputDebugString(L"Activated.\n");
	//When game becomes active window (Opportunity to ignore the first message received, likely due to clicking into the window)
	//Lock cursor to screen
	RECT cursorBox = { m_screenWidth / 2, m_screenHeight / 2, m_screenWidth / 2 + 1, m_screenHeight / 2 + 1 };
	//If a menu is active, release and show the cursor, otherwise lock it
	if (m_menuActive == 1) {
		while (ShowCursor(true) < 0) {};
		ClipCursor(NULL);
	}
	else {
		ClipCursor(&cursorBox);
		ShowCursor(false);
	}
}

void Game::OnDeactivated() {
	//Game becomes background window (Opportunity to auto-pause the game if necessary)
	//Release cursor from screen
	OutputDebugString(L"Deactivated.\n");
	ClipCursor(NULL);
	while(ShowCursor(true) < 0) {}
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

void Game::InitializeShaders() {
	std::wstring msg;
	//Retrieve vertex and index arrays, and add them to vertex buffer with AddGeometry()
	if (m_world1->GetPlanetCount() == 0) {
		OutputDebugString(L"No planet to initialize\n");
	}
	else {
		OutputDebugString(L"Planet exists.\n");
	}
	GetPlanet(0)->GenerateGeometry();
	m_planetVertexCount = GetPlanet(0)->GetVertexCount();
	m_planetIndexCount = m_planetVertexCount;
	std::vector<UINT> planetIndexArray = GetPlanet(0)->GetIndexArray();
	msg = L"Size of planet index array: " + std::to_wstring(planetIndexArray.size()); msg += L"\n";
	msg += L"Size of planet vertex array: " + std::to_wstring(GetPlanet(0)->GetVertexCount()); msg += L"\n";
	msg += L"Size of m_planetIndexCount: " + std::to_wstring(m_planetIndexCount); msg += L"\n";
	m_graphicsObj.SetGeometry(*GetPlanet(0)->GetGeometry(), planetIndexArray);
	msg += L"Size of cube index array: " + std::to_wstring(m_cubeIndices.size()); msg += L"\n";
	m_graphicsObj.AddGeometry(m_cubeVertices, m_cubeIndices);
	msg += L"Size of isoSphere index array: " + std::to_wstring(m_isoSphereIndices.size()); msg += L"\n";
	m_graphicsObj.AddGeometry(m_isoSphereVertices, m_isoSphereIndices);
	//Add spaceship
	m_graphicsObj.AddGeometry(m_spaceshipVertices, m_spaceshipIndices);
	m_graphicsObj.SendToPipeline(m_device.Get());
	OutputDebugString(msg.c_str());

	OutputDebugString(L"Starting Input Element Desc.\n");
	//Description of the vertex buffer to be submitted.  The semantic in argument 1 should match the .hlsl shader semantics it intends to use
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		//Vertex data
		{static_cast<LPCSTR>("POSITION"), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{static_cast<LPCSTR>("COLOR"), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//Instance data
		{static_cast<LPCSTR>("INSTANCE_POSITION"), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{static_cast<LPCSTR>("INSTANCE_POSITION"), 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{static_cast<LPCSTR>("INSTANCE_POSITION"), 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{static_cast<LPCSTR>("INSTANCE_POSITION"), 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};
	OutputDebugString(L"VertexBuffer Input Element Desc created.\n");

	OutputDebugString(L"Compiling shaders.\n");
	//Compiling the shader
	Microsoft::WRL::ComPtr<ID3DBlob> VshaderBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> VerrorBlob = nullptr;
	DX::ThrowIfFailed(D3DCompileFromFile(L"VertexInstanceShader.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &VshaderBlob, &VerrorBlob));
	OutputDebugString(L"Creating vertex shader.\n");
	//Create vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> createdVertexShader;
	DX::ThrowIfFailed(m_device->CreateVertexShader(VshaderBlob->GetBufferPointer(), VshaderBlob->GetBufferSize(), NULL, &createdVertexShader));


	OutputDebugString(L"Compiling and creating geometry shader.\n");
	//Geometry Shader
	Microsoft::WRL::ComPtr<ID3DBlob> GshaderBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> GerrorBlob = nullptr;
	DX::ThrowIfFailed(D3DCompileFromFile(L"GeometryShader1.hlsl", nullptr, nullptr, "main", "gs_5_0", 0, 0, &GshaderBlob, &GerrorBlob));
	DX::ThrowIfFailed(m_device->CreateGeometryShader(GshaderBlob->GetBufferPointer(), GshaderBlob->GetBufferSize(), NULL, &m_createdGeometryShader));

	OutputDebugString(L"Compiling and creating pixel shader.\n");
	//Pixel Shader
	Microsoft::WRL::ComPtr<ID3DBlob> PshaderBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> PerrorBlob = nullptr;
	DX::ThrowIfFailed(D3DCompileFromFile(L"PixelShader1.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &PshaderBlob, &PerrorBlob));
	Microsoft::WRL::ComPtr<ID3D11PixelShader> createdPixelShader;
	DX::ThrowIfFailed(m_device->CreatePixelShader(PshaderBlob->GetBufferPointer(), PshaderBlob->GetBufferSize(), NULL, &createdPixelShader));

	OutputDebugString(L"Creating input layout.\n");
	//Create input layout
	DX::ThrowIfFailed(m_device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), VshaderBlob->GetBufferPointer(), VshaderBlob->GetBufferSize(), &m_inputLayout));
	//Set shaders to use
	m_deviceContext->VSSetShader(createdVertexShader.Get(), NULL, 0);
	m_deviceContext->GSSetShader(m_createdGeometryShader.Get(), NULL, 0);
	m_deviceContext->PSSetShader(createdPixelShader.Get(), NULL, 0);
	
	OutputDebugString(L"Binding vertices to context\n");
	//Bind Vertices to context
	m_graphicsObj.Bind(m_deviceContext.Get());
	//Set input layout
	m_deviceContext->IASetInputLayout(m_inputLayout.Get());
	//Set topology type for primitive
	m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	OutputDebugString(L"Constant buffer description.\n");
	//Constant Buffers
	D3D11_BUFFER_DESC constDesc = {};
	constDesc.Usage = D3D11_USAGE_DYNAMIC;
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = static_cast<UINT>(sizeof(MatrixData) + (16 - (sizeof(MatrixData) % 16)));
	msg = L"ByteWidth: ";
	msg += std::to_wstring(static_cast<UINT>(sizeof(MatrixData) + (16 - (sizeof(MatrixData) % 16))));
	msg += L"\n";
	OutputDebugString(msg.c_str());
	constDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constDesc.MiscFlags = 0;
	constDesc.StructureByteStride = 0;

	DX::ThrowIfFailed(m_device->CreateBuffer(&constDesc, 0, m_constBuffer.GetAddressOf()));
	OutputDebugString(L"Constant Buffer successfully created.\n");
	
	//Store matrices into members
	XMStoreFloat4x4(&float4x4Data.worldMatrix, XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, 3.0f), XMMatrixIdentity()));
	XMStoreFloat4x4(&float4x4Data.perspectiveMatrix, XMMatrixPerspectiveFovLH(m_FOV, (static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight)), 0.1f, 1000.0f));
	OutputDebugString(L"Matrices successfuly stored into float4x4.\n");

	msg = L"Number of instances: "; msg += std::to_wstring(m_worldObjects.size()); msg += L".\n";
	OutputDebugString(msg.c_str());
}

void Game::UpdateGraphicsBuffers() {
	//std::wstring msg;
	std::vector<UINT> planetIndexArray = GetPlanet(0)->GetIndexArray();
	m_graphicsObj.SetGeometry(*GetPlanet(0)->GetGeometry(), planetIndexArray);
	m_graphicsObj.AddGeometry(m_cubeVertices, m_cubeIndices);
	m_graphicsObj.AddGeometry(m_isoSphereVertices, m_isoSphereIndices);
	m_graphicsObj.AddGeometry(m_spaceshipVertices, m_spaceshipIndices);
	m_graphicsObj.SendToPipeline(m_device.Get());
	m_graphicsObj.Bind(m_deviceContext.Get(), m_instanceBuffer.Get());
}

//1 to 3, indicates current tool for rendering purposes
void Game::SetCurrentTool(int tool) {
	m_currentTool = tool;
}

void Game::SetWireframe(bool on) {
	if (on) {
		m_deviceContext->GSSetShader(m_createdGeometryShader.Get(), NULL, 0);
	}
	else {
		m_deviceContext->GSSetShader(NULL, NULL, 0);
	}
}

void Game::AddPlanet(DirectX::XMVECTOR position, float radius) {
	m_world1->AddPlanet(position, radius);
}
void Game::AddBlock() {
	m_world1->AddBlock(camera->GetForwardRay(10.0f), camera->GetOrientationVector());
}
void Game::RemoveBlock() {
	m_world1->RemoveBlock(camera->GetForwardRay(10.0f));
}
void Game::AddShip() {
	m_world1->AddShip(camera->GetForwardRay(10.0f), camera->GetOrientationVector());
}
void Game::RemoveShip() {
	m_world1->RemoveShip(camera->GetForwardRay(10.0f));
}
Planet* Game::GetPlanet(int index) {
	return m_world1->GetPlanet(index);
}
World* Game::GetWorld() {
	return m_world1.get();
}

void Game::LoadWorld() {
	OutputDebugString(L"World Load Activated.\n");
	m_menuStack.clear();
	m_IOHandler.ImportWorldInfo(L"TestWorld.cwd", m_world1.get());
	m_world1->Loaded();
	m_worldLoaded = true;
	InitializeShaders();
}
void Game::SaveWorld() {
	m_IOHandler.ExportWorldInfo(L"TestWorld.cwd", m_world1.get());
}
void Game::NewWorld() {
	OutputDebugString(L"Creating new world\n");
	m_menuStack.clear();
	m_world1->AddPlanet(XMQuaternionIdentity(), 10.0f);
	m_worldLoaded = true;
	InitializeShaders();
}
void Game::ExitWorld() {
	OutputDebugString(L"Exiting world to main menu\n");
	m_IOHandler.ExportWorldInfo(L"TestWorld.cwd", m_world1.get());
	m_world1->Unloaded();
	m_worldLoaded = false;
	OpenMainMenu();
}

float Game::GetFOV() {
	return m_FOV;
}
void Game::SetFOV(float fov) {
	m_settingsIO->SetSetting(m_settingsFileName, "FieldOfView", std::to_string(fov));
	m_FOV = fov;
}

void Game::GetResolution(int& width, int& height) {
	width = m_screenWidth;
	height = m_screenHeight;
}
//Possibly a placeholder for more robust resolution system.
void Game::SetResolution(int width, int height) {
	m_settingsIO->SetSetting(m_settingsFileName, "ResolutionWidth", std::to_string(width));
	m_settingsIO->SetSetting(m_settingsFileName, "ResolutionHeight", std::to_string(height));
	OnWindowSizeChanged(width, height);
}

void Game::Exit() {
	PostMessage(m_windowHandle, WM_CLOSE, 0, 0);
}

void Game::CheckMenuClick(int posX, int posY){
	/*
	for (auto menu : m_menuStack) {
		//OutputDebugString(L"Menu clicked.\n");
		menu->GetClickedButton(posX, posY).OnClick();
	}
	*/
	
	MenuButton button(L"", 0, 0, 0, 0, 0, 0, 0, 0);
	if (!m_menuStack.empty()) {
		m_menuStack.back()->ClickButton(posX, posY);
		OutputDebugString(L"Button clicked.\n");
	}
	OutputDebugString(L"No menu to click.\n");
	
}

bool Game::CheckMenuState() {
	if (m_menuStack.empty()) {
		m_menuActive = false;
		while (ShowCursor(false) >= 0) {};
		RECT cursorBox = { m_screenWidth / 2, m_screenHeight / 2, m_screenWidth / 2 + 1, m_screenHeight / 2 + 1 };
		ClipCursor(&cursorBox);
	}
	else {
		m_menuActive = true;
		while (ShowCursor(true) < 0) {};
		ClipCursor(NULL);
	}
	return m_menuActive;
}


void Game::OpenMainMenu() {
	m_menuStack.push_back(std::make_shared<MainMenu>(m_screenWidth, m_screenHeight, this));
	OutputDebugString(L"Opening main menu.\n");
}
void Game::OpenInGameMenu() {
	m_menuStack.push_back(std::make_unique<InGameMenu>(m_screenWidth, m_screenHeight, this));
}
void Game::CloseMenus() {
	OutputDebugString(L"Clearing menus\n");
	if (m_worldLoaded == true) {
		m_menuStack.clear();
	}
	else {
		for(int i = 1; i < m_menuStack.size(); i++){
			m_menuStack.pop_back();
		}
	}
}
void Game::OpenSettingsMenu() {
	m_menuStack.push_back(std::make_unique<SettingsMenu>(m_screenWidth, m_screenHeight, this));
}