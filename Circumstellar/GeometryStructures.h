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
	
	const Color CubeColor = { DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f) };

	const std::vector<Vertex> CubeArray = {
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), CubeColor.ColorData },

		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), CubeColor.ColorData },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), CubeColor.ColorData },
	};
}