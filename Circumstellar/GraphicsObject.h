#pragma once
#include "pch.h"
#include "GeometryStructures.h"

class GraphicsObject {
public:
	GraphicsObject();
	//Send this object's geometry to the pipeline
	bool SendToPipeline(ID3D11Device* device);
	void Bind(ID3D11DeviceContext* context);
	UINT GetVertexCount();
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vBuffer;
	UINT m_vCount;
	std::vector<CustomGeometry::Vertex> m_vertices;
};