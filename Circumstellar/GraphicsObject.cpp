#include "pch.h"
#include "GraphicsObject.h"
#include "GeometryStructures.h"

GraphicsObject::GraphicsObject() :
	//Cube of triangles
	m_vertices(CustomGeometry::CubeArray),
		m_vCount(0)
{
	OutputDebugString(L"Constructed \n");
}

bool GraphicsObject::SendToPipeline(ID3D11Device* device) {
	m_vCount = static_cast<UINT>(m_vertices.size());
	OutputDebugString(L"Vertex count: ");
	OutputDebugString(std::to_wstring(m_vertices.size()).c_str());
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CustomGeometry::Vertex) * m_vertices.size();	//Size of array passed to the GPU
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//Type of buffer i.e. vertex buffer
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = m_vertices.data();
	OutputDebugString(L"vertice data: ");
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, &subData, m_vBuffer.GetAddressOf()));
	OutputDebugString(L"Sent To Pipeline\n");
}

void GraphicsObject::Bind(ID3D11DeviceContext* context) {
	UINT stride = sizeof(CustomGeometry::Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_vBuffer.GetAddressOf(), &stride, &offset);
	OutputDebugString(L"Object Bound \n");
}

UINT GraphicsObject::GetVertexCount() {
	return m_vCount;
}