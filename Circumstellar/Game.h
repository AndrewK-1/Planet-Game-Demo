#pragma once
#include "pch.h"
#include "GeometryStructures.h"
#include "Camera.h"
#include <list>
#include "GraphicsObject.h"
#include "World.h"
#include "Planet.h"
#include "InputOutput.h"
#include "GraphicsSettingsIO.h"
#include "Menus.h"

class InputController;
class Menus;

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

	void SetInputController(InputController* inputController);
	InputController* GetInputController();

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
	void GetCurrentSize(int& width, int& height);
	void OnWindowSizeChanged(int width, int height);
	std::unique_ptr<Camera> camera;

	std::unique_ptr<Planet> m_planet1;
	
	bool updatePlanetGeometryFlag;

	void SetCurrentTool(int tool);
	void SetWireframe(bool on);

	void AddPlanet(DirectX::XMVECTOR position, float radius);
	void AddBlock();
	void RemoveBlock();
	void AddShip();
	void RemoveShip();

	Planet* GetPlanet(int index);
	World* GetWorld();

	void LoadWorld(std::wstring worldName);
	void SaveWorld(std::wstring worldName);
	void NewWorld();
	void ExitWorld();

	float GetFOV();
	void SetFOV(float fov);
	void GetResolution(int& width, int& height);
	void SetResolution(int width, int height);

	void Exit();
	void CheckMenuClick(int posX, int posY);
	bool CheckMenuState();
	void OpenMainMenu();
	void OpenInGameMenu();
	void CloseMenus();
	void CloseTopmostMenu();
	Menus& GetTopmostMenu();
	void OpenSettingsMenu();
	void OpenGraphicsSettingsMenu();
	void OpenKeybindMenu();
	void OpenKeybindPromptMenu();
	void OpenSaveMenu();
	void OpenLoadMenu();
	void ChangeFontSize(float fontSize);
	GraphicsSettingsIO* GetSettingIO();
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
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;			
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_pVBuffer;

	struct MatrixData {
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX perspectiveMatrix;
		DirectX::XMVECTOR cameraPos;
	};

	struct Float4x4Data {
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 perspectiveMatrix;
	};

	float m_FOV;

	Float4x4Data float4x4Data;

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_constBuffer;

	GraphicsObject m_graphicsObj;
	GraphicsObject m_graphicsObjVox;

	std::vector<WorldObject> m_worldObjects;

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_instanceBuffer;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_createdGeometryShader;

	void UpdateGraphicsBuffers();
	int m_planetVertexCount;
	int m_planetIndexCount;
	int m_cubeIndexCount = CustomGeometry::CubeVertexCount;
	int m_cubeVertexCount = CustomGeometry::CubeVertexCount;
	std::vector<CustomGeometry::VertexWNormal> m_cubeVertices = CustomGeometry::CubeArray;
	std::vector<UINT> m_cubeIndices = CustomGeometry::CubeIndexArray;
	int m_isoSphereIndexCount = CustomGeometry::IsoSphereIndexCount;
	int m_isoSphereVertexCount = CustomGeometry::IsoSphereVertexCount;
	std::vector<CustomGeometry::VertexWNormal> m_isoSphereVertices = CustomGeometry::IsoSphereArray;
	std::vector<UINT> m_isoSphereIndices = CustomGeometry::IsoSphereIndexArray;
	int m_spaceshipIndexCount = CustomGeometry::SpaceshipIndexCount;
	int m_spaceshipVertexCount = CustomGeometry::SpaceshipVertexCount;
	std::vector<CustomGeometry::VertexWNormal> m_spaceshipVertices = CustomGeometry::SpaceshipVertexArray;
	std::vector<UINT> m_spaceshipIndices = CustomGeometry::SpaceshipIndexArray;

	int m_currentTool;

	//World holding all the objects currently present
	std::unique_ptr<World> m_world1;

	InputOutput m_IOHandler;

	//2D graphics members
	Microsoft::WRL::ComPtr<ID2D1Factory> m_2dFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_2dRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_2dBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_2dBrushSolidBlue;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>m_2dBrushCyan;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	//Setting IO
	std::unique_ptr<GraphicsSettingsIO> m_settingsIO;
	std::string m_settingsFileName;

	//Menus
	bool m_worldLoaded;
	bool m_menuActive;
	std::vector<std::shared_ptr<Menus>> m_menuStack;
	InputController* m_inputController;
	GameplayUIMenu m_uiMenu;
	
	bool m_wireframe;
	int m_planetVoxCount;
};