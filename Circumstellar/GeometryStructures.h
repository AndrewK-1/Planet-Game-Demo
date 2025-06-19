#pragma once
#include "pch.h"

namespace CustomGeometry {
	struct Vertex {
		DirectX::XMFLOAT4 V_Position;
		DirectX::XMFLOAT4 V_Color;
	};

	struct Triangle {
		Vertex vertices[3];
	};
}