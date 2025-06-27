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

	struct Shape {
		std::vector<Vertex> vertexArray;
		Shape(std::vector<Vertex> v) {
			vertexArray = v;
		}
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
	const Shape Cube({
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		});
}