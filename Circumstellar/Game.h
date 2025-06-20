#pragma once
#include "pch.h"
#include "GeometryStructures.h"

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

	//Initilization and game tick
	void Initialize(HWND windowHandle);
	void Tick();

	//Window minimize/maximize/size change methods
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnClosing();
	void GetDefaultSize(int& width, int& height);
	void OnWindowSizeChanged(int width, int height);

private:
	//Rendering and views methods
	void Update();
	void Render();
	void Clear();
	void Present();
	void CreateDevice();
	void CreateResources();
	
	void OnDeviceLost();
	void InitializeShaders();
	
	//Resources
	HWND											m_windowHandle;
	int												m_screenWidth;
	int												m_screenHeight;

	D3D_FEATURE_LEVEL								m_feature_level;		//Feature Levels
	Microsoft::WRL::ComPtr<ID3D11Device>			m_device;				//GPU device to use
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_deviceContext;		//Rendering Pipeline
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_swapChain;			//Swapchain for showing backbuffers
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_renderTargetView;		//Thing targeted for rendering
	//Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_depthStencilView;		//Depth stuff
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_inputLayout;			//Input Layout object for receiveing input descriptions or something
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_pVBuffer;
};