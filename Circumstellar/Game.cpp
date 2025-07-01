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

using Microsoft::WRL::ComPtr;
using namespace DirectX;

Game::Game() noexcept :
	m_windowHandle(nullptr),
	m_screenWidth(1920),
	m_screenHeight(1080),
	m_feature_level(D3D_FEATURE_LEVEL_9_1),
	float4x4Data(
		{ {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f} }),
	planet1(4.0f)
{
	camera = std::make_unique<Camera>();
	m_graphicsObj = std::make_unique<GraphicsObject>();
}

void Game::Initialize(HWND windowHandle) {

	m_windowHandle = windowHandle;
	CreateDevice();
	CreateResources();
	InitializeShaders();
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

void Game::Render() {
	Clear();
	
	//Render code
	m_deviceContext->DrawInstanced(m_graphicsObj->GetVertexCount(), m_worldObjects.size(), 0, 0);

	Present();
}

//For clearing back buffers
void Game::Clear() {

	//Clear views
	float backgroundColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), /*DirectX::Colors::CornflowerBlue */  backgroundColor);
	//Below statement is for depth stencils
	////m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//Set render targets
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), NULL);

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
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//Allow switch to full screen with Alt+Enter
		
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

	//m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_deviceContext.Reset();
	m_device.Reset();
	m_inputLayout.Reset();
	m_pVBuffer.Reset();

	CreateDevice();
	CreateResources();
	InitializeShaders();
}

void Game::GetDefaultSize(int& width, int& height) 
{
	width = 1920;
	height = 1080;
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

void Game::InitializeShaders() {
	planet1.GenerateGeometry();
	m_graphicsObj->SetGeometry(*planet1.GetGeometry());
	m_graphicsObj->SendToPipeline(m_device.Get());


	OutputDebugString(L"Beginning instancing information.\n");
	//Instancing
	int instanceCount = 0;
	for (int i = 0; i < 1; i++) {
		instanceCount++;
		float ifloat = static_cast<float>(i/10.0f);
		XMVECTOR instpos = XMVectorSet(ifloat, ifloat, ifloat, 1.0f);
		XMVECTOR instrot = XMVectorSet(ifloat, 0.0f, 0.0f, 1.0f);
		XMVECTOR instscale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		WorldObject worldobj(instpos, instrot, instscale);
		m_worldObjects.push_back(worldobj);
	}
	//Instancing
	std::vector<DirectX::XMMATRIX> instanceMatrices;
	for (const auto& objIterator : m_worldObjects) {
		instanceMatrices.push_back(objIterator.getObjectMatrix());
		OutputDebugString(L"New instance Matrix\n");
	}
	//Instancing
	D3D11_BUFFER_DESC instanceDesc = {};
	instanceDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceDesc.ByteWidth = sizeof(XMMATRIX) * instanceCount;
	OutputDebugString(L"Instance ByteWidth: "); OutputDebugString(std::to_wstring(sizeof(XMMATRIX) * instanceCount).c_str()); OutputDebugString(L".\n");
	instanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA instanceData = { instanceMatrices.data(), 0, 0 };
	m_device->CreateBuffer(&instanceDesc, &instanceData, &m_instanceBuffer);

	OutputDebugString(L"Instancing properly instantiated.\n");

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
	m_deviceContext->PSSetShader(createdPixelShader.Get(), NULL, 0);
	
	OutputDebugString(L"Binding vertices to context\n");
	//Bind Vertices to context
	m_graphicsObj->Bind(m_deviceContext.Get(), m_instanceBuffer.Get());
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
	std::wstring msg = L"ByteWidth: ";
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
	XMStoreFloat4x4(&float4x4Data.perspectiveMatrix, XMMatrixPerspectiveFovLH(3.14159f / 4.0f, 16.0f / 9.0f, 0.1f, 100.0f));
	OutputDebugString(L"Matrices successfuly stored into float4x4.\n");
	
	msg = L"Vertex count per instance: ";
	msg += std::to_wstring(m_graphicsObj->GetVertexCount());
	msg += L".\n";
	OutputDebugString(msg.c_str());

	msg = L"Number of instances: "; msg += std::to_wstring(m_worldObjects.size()); msg += L".\n";
	OutputDebugString(msg.c_str());
}
