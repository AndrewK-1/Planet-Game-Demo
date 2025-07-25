#pragma once
#include "pch.h"
#include "GeometryStructures.h"

class GraphicsObject {
public:
	GraphicsObject();
	//Send this object's geometry to the pipeline
	bool SendToPipeline(ID3D11Device* device);
	void Bind(ID3D11DeviceContext* context);
	void Bind(ID3D11DeviceContext* context, ID3D11Buffer* instancing);
	UINT GetVertexCount();
	void SetGeometry(std::vector<CustomGeometry::VertexWNormal> &geometry, std::vector<UINT>& indexArray);
	void AddGeometry(std::vector<CustomGeometry::VertexWNormal> &geometry, std::vector<UINT>& indexArray);
	void ResetGeometry();
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_iBuffer;
	UINT m_vCount;
	std::vector<CustomGeometry::VertexWNormal> m_vertices;
	std::vector<UINT> m_indexArray;

};