#pragma once
#include "pch.h"
#include "WorldObject.h"
#include "GeometryStructures.h"

class Planet : public WorldObject{
public:
	Planet(float radius);
	Planet(float radius, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale);
	Planet();
	void GenerateGeometry();
	
private:
	std::vector<std::vector<std::vector<float>>> m_voxelChangeData;
	std::vector<CustomGeometry::Vertex> m_geometry;
	float m_radius;
	float m_cubeValues[2][2][2];
	bool m_visitedEmpty[2][2][2];
	std::vector<CustomGeometry::Vertex> m_tempVertices;
	float m_currentCoords[3];
	void RecursiveCubeCheck(int x, int y, int z, char translationAxis);
	int Wraparound(int a);
	void AxisCheck(int x, int y, int z, char axis);
	void CalculateVertex(int x, int y, int z, char axis);
};