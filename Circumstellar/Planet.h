#pragma once
#include "pch.h"
#include "WorldObject.h"
#include "GeometryStructures.h"
#include <map>

class Planet : public WorldObject{
public:
	Planet(float radius);
	Planet(float radius, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale);
	Planet();
	void GenerateData();
	void GenerateGeometry();
	void EditData(int x, int y, int z, float value);
	void SetData(int x, int y, int z, float value);
	float GetData(int x, int y, int z);
	std::vector<CustomGeometry::VertexWNormal>* GetGeometry();
	int GetArrayRadius();
	int GetArraySize();
	int GetVertexCount();
	std::vector<UINT> GetIndexArray();
	std::vector<std::vector<std::vector<float>>>& GetVoxelData();
	float GetPlanetRadius();
private:
	std::vector<std::vector<std::vector<float>>> m_voxelData;
	std::vector<CustomGeometry::VertexWNormal> m_geometry;
	std::vector<UINT> m_indexArray;
	float m_radius;
	int m_arrayRadius;
	float m_cubeValues[2][2][2];
	bool m_visitedEmpty[2][2][2];
	std::vector<CustomGeometry::Vertex> m_tempVertices;
	float m_currentCoords[3];
	void RecursiveCubeCheck(int x, int y, int z, char translationAxis);
	int Wraparound(int a);
	void AxisCheck(int x, int y, int z, char axis);
	void CalculateVertex(int x, int y, int z, char axis);
	int m_vertexCount;
	int m_voxelPadding;
	//Map for more efficient vertex changes
	//std::map<int, std::map<int, std::map<int, std::vector<CustomGeometry::Vertex>>>> m_trianglemap;
};