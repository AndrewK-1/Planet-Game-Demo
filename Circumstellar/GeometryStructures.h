#pragma once
#include "pch.h"

namespace CustomGeometry {
	
	struct Color {
		DirectX::XMFLOAT4 ColorData;
	};

	struct Vertex {
		DirectX::XMFLOAT4 V_Position;
		DirectX::XMFLOAT4 V_Color;
	};
	
	struct VertexWNormal {
		DirectX::XMFLOAT4 V_Position;
		DirectX::XMFLOAT4 V_Normal;
		DirectX::XMFLOAT4 V_Color;
	};

	inline const DirectX::XMFLOAT4 tempNormal = { 1.0f, 0.0f, 0.0f, 1.0f };

	inline const Color CubeColor = { DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f) };
	

	inline const std::vector<VertexWNormal> CubeArray = {
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData},
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), CubeColor.ColorData },
	};
	

	int const CubeVertexCount = 36;
	inline const std::vector<UINT> CubeIndexArray = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35 };

	inline const std::vector<VertexWNormal> IsoSphereArray = {
		{ DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), CubeColor.ColorData},
		{ DirectX::XMFLOAT4(0.0f, 0.447215f, 0.894425f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.447215f, 0.894425f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-0.85064f, 0.447215f, 0.276385f, 1.0f), DirectX::XMFLOAT4(-0.85064f, 0.447215f, 0.276385f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-0.52572f, 0.447215f, -0.7236f, 1.0f), DirectX::XMFLOAT4(-0.52572f, 0.447215f, -0.7236f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(0.52572f, 0.447215f, -0.7236f, 1.0f), DirectX::XMFLOAT4(0.52572f, 0.447215f, -0.7236f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(0.85064f, 0.447215f, 0.276385f, 1.0f), DirectX::XMFLOAT4(0.85064f, 0.447215f, 0.276385f, 1.0f), CubeColor.ColorData }, //5
		{ DirectX::XMFLOAT4(0.52572f, -0.447215f, 0.7236f, 1.0f), DirectX::XMFLOAT4(0.52572f, -0.447215f, 0.7236f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-0.52572f, -0.447215f, 0.7236f, 1.0f), DirectX::XMFLOAT4(-0.5257f, -0.4472f, 0.7236, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-0.85064f, -0.447215f, -0.276385f, 1.0f), DirectX::XMFLOAT4(-0.85064f, -0.447215f, -0.276385f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(0.0f, -0.447215f, -0.894425f, 1.0f), DirectX::XMFLOAT4(0.0f, -0.447215f, -0.894425f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(0.85064f, -0.447215f, -0.276385f, 1.0f), DirectX::XMFLOAT4(0.85064f, -0.447215f, -0.276385f, 1.0f), CubeColor.ColorData }, //10
		{ DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f), CubeColor.ColorData },
	};
	int const IsoSphereVertexCount = 12;
	inline const std::vector<UINT> IsoSphereIndexArray = { 0,2,1,0,3,2,0,4,3,0,5,4,0,1,5,
		1,6,5,1,7,6,2,7,1,2,8,7,3,8,2,3,9,8,4,9,3,4,10,9,5,10,4,5,6,10,
		11,6,7,11,7,8,11,8,9,11,9,10,11,10,6};
	int const IsoSphereIndexCount = 60;

	inline const Color SpaceshipColor = { DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 0.0f) };
	inline const std::vector<VertexWNormal> SpaceshipVertexArray = {
		{ DirectX::XMFLOAT4(0.0f, -0.3048f, 1.40445f, 1.0f), DirectX::XMFLOAT4(0.0f, -0.3048f, 1.40445f, 1.0f), SpaceshipColor.ColorData },	//0

		{ DirectX::XMFLOAT4(0.0f, 0.304799f, 0.304799f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.304799f, 0.304799f, 1.0f), SpaceshipColor.ColorData },	//1
		{ DirectX::XMFLOAT4(0.3048f, 0.3048f, 0.3048f, 1.0f), DirectX::XMFLOAT4(0.3048f, 0.3048f, 0.3048f, 1.0f), SpaceshipColor.ColorData },
		{ DirectX::XMFLOAT4(0.462012f, -0.3048f, 0.3048f, 1.0f), DirectX::XMFLOAT4(0.462012f, -0.3048f, 0.3048f, 1.0f), SpaceshipColor.ColorData },
		{ DirectX::XMFLOAT4(0.0f, -0.3048f, 0.304766f, 1.0f), DirectX::XMFLOAT4(0.0f, -0.3048f, 0.304766f, 1.0f), SpaceshipColor.ColorData },
		{ DirectX::XMFLOAT4(-0.462012f, -0.3048f, 0.3048f, 1.0f), DirectX::XMFLOAT4(-0.462012f, -0.3048f, 0.3048f, 1.0f), SpaceshipColor.ColorData },
		{ DirectX::XMFLOAT4(-0.3048f, 0.3048f, 0.3048f, 1.0f), DirectX::XMFLOAT4(-0.3048f, 0.3048f, 0.3048f, 1.0f), SpaceshipColor.ColorData },	//6

		{ DirectX::XMFLOAT4(0.0f, 0.3048f, -0.820414f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.3048f, -0.820414f, 1.0f), SpaceshipColor.ColorData },	//7
		{ DirectX::XMFLOAT4(0.685801f, 0.441425f, -1.27367f, 1.0f), DirectX::XMFLOAT4(0.685801f, 0.441425f, -1.27367f, 1.0f), SpaceshipColor.ColorData },
		{ DirectX::XMFLOAT4(0.3048f, -0.3048f, -0.3048f, 1.0f), DirectX::XMFLOAT4(0.3048f, -0.3048f, -0.3048f, 1.0f), SpaceshipColor.ColorData },
		{ DirectX::XMFLOAT4(0.0f, -0.3048f, -0.522118f, 1.0f), DirectX::XMFLOAT4(0.0f, -0.3048f, -0.522118f, 1.0f), SpaceshipColor.ColorData },
		{ DirectX::XMFLOAT4(-0.3048f, -0.3048f, -0.3048f, 1.0f), DirectX::XMFLOAT4(-0.3048f, -0.3048f, -0.3048f, 1.0f), SpaceshipColor.ColorData },
		{ DirectX::XMFLOAT4(-0.685801f, 0.441425f, -1.27367f, 1.0f), DirectX::XMFLOAT4(-0.685801f, 0.441425f, -1.27367f, 1.0f), SpaceshipColor.ColorData },	//12
	};
	int const SpaceshipVertexCount = 13;
	inline const std::vector<UINT> SpaceshipIndexArray = {
		2,1,0,3,2,0,4,3,0,5,4,0,6,5,0,1,6,0,
		8,7,1,2,8,1,12,6,1,7,12,1,3,8,2,6,12,5,
		9,8,3,10,8,9,7,8,10,12,7,10,11,12,10,5,12,11,
		4,9,3,10,9,4,11,10,4,5,11,4
	};
	int const SpaceshipIndexCount = 66;
}