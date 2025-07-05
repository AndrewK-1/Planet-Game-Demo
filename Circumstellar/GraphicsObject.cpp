#include "pch.h"
#include "GraphicsObject.h"
#include "GeometryStructures.h"

GraphicsObject::GraphicsObject() :
	//Cube of triangles
	m_vertices(CustomGeometry::CubeArray),
	m_vCount(0)
{
	OutputDebugString(L"Graphics Object Constructed.\n");
}

bool GraphicsObject::SendToPipeline(ID3D11Device* device) {
	m_vCount = static_cast<UINT>(m_vertices.size());
	//OutputDebugString(L"Vertex count: ");
	//OutputDebugString(std::to_wstring(m_vertices.size()).c_str());
	//OutputDebugString(L"\n");
	D3D11_BUFFER_DESC vertBufferDesc = {};
	vertBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc.ByteWidth = sizeof(CustomGeometry::Vertex) * m_vertices.size();	//Size of array passed to the GPU
	vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//Type of buffer i.e. vertex buffer
	vertBufferDesc.CPUAccessFlags = 0;
	vertBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = m_vertices.data();
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	DX::ThrowIfFailed(device->CreateBuffer(&vertBufferDesc, &subData, m_vBuffer.GetAddressOf()));
	//OutputDebugString(L"Sent To Pipeline\n");
	

	//Index Buffer Description
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexArray.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = m_indexArray.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the buffer with the device.(
	DX::ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexData, &m_iBuffer));

	return 1;
}

void GraphicsObject::Bind(ID3D11DeviceContext* context){
	UINT stride = sizeof(CustomGeometry::Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, m_vBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_iBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//OutputDebugString(L"Object Bound \n");
}

void GraphicsObject::Bind(ID3D11DeviceContext* context, ID3D11Buffer* instancing) {
	UINT stride[] = {sizeof(CustomGeometry::Vertex), sizeof(DirectX::XMMATRIX)};
	UINT offset[] = { 0, 0 };
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffers[] = { m_vBuffer.Get(), instancing};
	context->IASetVertexBuffers(0, 2, buffers->GetAddressOf(), stride, offset);
	context->IASetIndexBuffer(m_iBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//OutputDebugString(L"Instancing Objects Bound \n");
}

UINT GraphicsObject::GetVertexCount() {
	return m_vCount;
}

void GraphicsObject::SetGeometry(std::vector<CustomGeometry::Vertex>& geometry, std::vector<UINT>& indexArray) {
	m_vertices = geometry;
	m_indexArray = indexArray;
}

void GraphicsObject::AddGeometry(std::vector<CustomGeometry::Vertex>& geometry, std::vector<UINT>& indexArray) {
	m_vertices.insert(m_vertices.end(), geometry.begin(), geometry.end());
	m_indexArray.insert(m_indexArray.end(), indexArray.begin(), indexArray.end());
}

void GraphicsObject::ResetGeometry() {
	m_vertices.clear();
	m_indexArray.clear();
}