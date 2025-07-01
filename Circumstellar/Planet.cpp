#pragma once
#include "pch.h"
#include "Planet.h"

using namespace DirectX;

Planet::Planet() 
	: WorldObject(), m_radius(10), m_voxelChangeData({}), m_tempVertices({}), m_cubeValues{}, m_visitedEmpty{}, m_currentCoords{} {}

Planet::Planet(float radius) 
	: WorldObject(), m_voxelChangeData({}), m_radius(radius), m_tempVertices({}), m_cubeValues{}, m_visitedEmpty{}, m_currentCoords{} {}

Planet::Planet(float radius, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale) 
	: WorldObject(position, rotation, scale), m_radius(radius), m_voxelChangeData({}), m_tempVertices({}), m_cubeValues{}, m_visitedEmpty{}, m_currentCoords{} {}

void Planet::GenerateGeometry() {
	m_geometry.clear();
	//truncated radius to assist in defining the loop variables
	int ceilradius = (int)std::ceil(m_radius);
	//Array maximum.  Beyond this point groupings will be outside the sphere's intersection.
	int arraymax = (int)std::ceil(m_radius)*2+1;
	//Geometry loop.  i is z, j is y, k is x.  ii is z, ij is y, ik is x.
	for (int i = -ceilradius; i < arraymax; i++) {
		m_currentCoords[2] = static_cast<float>(i);
		for (int j = -ceilradius; i < arraymax; i++) {
			m_currentCoords[1] = static_cast<float>(j);
			for (int k = -ceilradius; i < arraymax; i++) {
				//value of each vertice in relation to the sphere's radius
				bool allNegative = 1;
				bool allPositive = 1;
				m_currentCoords[0] = static_cast<float>(k);
				for (int ii = 0; i < 2; i++) {
					for (int ij = 0; ij < 2; ij++) {
						for (int ik = 0; ik < 2; ik++) {
							//Negatives are air, positives are material
							m_cubeValues[ik][ij][ii] = static_cast<float>(m_radius - std::sqrt((((k + ik) ^ 2) + ((j + ij) ^ 2) + ((i + ii) ^ 2))));
							if (!(m_cubeValues[ik][ij][ii] && m_cubeValues[0][0][0] < 0)) {
								allNegative = 0;
							}
							if (!(m_cubeValues[ik][ij][ii] && m_cubeValues[0][0][0] > 0)) {
								allPositive = 0;
							}
						}
					}
				}
				//If all vertices are empty or full, skip geometry drawing
				if (allNegative == 0 || allPositive == 0) {
					//Generating the actual geometry
					m_visitedEmpty[2][2][2] = {};
					for (int z = 0; z < 2; z++) {
						for (int y = 0; y < 2; y++) {
							for (int x = 0; x < 2; x++) {
								//If current is some value of empty
								if (m_cubeValues[x][y][z] < 0) {
									//If it hasn't been visited yet
									if (m_visitedEmpty[x][y][z] == 0) {
										m_tempVertices.clear();
										m_visitedEmpty[x][y][z] = 1;
										RecursiveCubeCheck(x, y, z, 'n');
										for (int triIndex = 2; triIndex < m_tempVertices.size(); triIndex++) {
											m_geometry.push_back(m_tempVertices[0]);
											m_geometry.push_back(m_tempVertices[triIndex-1]);
											m_geometry.push_back(m_tempVertices[triIndex]);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//Recursively iterate over points to generate geometry
void Planet::RecursiveCubeCheck(int x, int y, int z, char translationAxis)
{
	//If the current point is one of four points going in a certain direction.  This is for clockwise winding.
	if ((x == 0, y == 0, z == 0) || (x == 1, y == 1, z == 0) || (x == 0, y == 1, z == 1) || (x == 1, y == 0, z == 1)) {
		//Check and visitedEmpty adjustment is also done in Axis Check, so might change that
		//If the vertex is negative (empty), then it is visited and will generate geometry
		//if (m_cubeValues[x][y][z] < 0) {
			//m_visitedEmpty[x][y][z] = 1;
			switch (translationAxis) {
			case 'x': {
				//yz
				AxisCheck(x, y, z, 'y');
				AxisCheck(x, y, z, 'z');
			}
			case 'y': {
				//zx
				AxisCheck(x, y, z, 'z');
				AxisCheck(x, y, z, 'x');
			}
			case 'z': {
				//xy
				AxisCheck(x, y, z, 'x');
				AxisCheck(x, y, z, 'y');
			}
			case 'n': {
				//xyz			
				AxisCheck(x, y, z, 'x');
				AxisCheck(x, y, z, 'y');
				AxisCheck(x, y, z, 'z');
			}
			}
		//}
	}
	else {
		//if (m_cubeValues[x][y][z] < 0) {
			//m_visitedEmpty[x][y][z] = 1;
			switch (translationAxis) {
			case 'x': {
				//zy
				AxisCheck(x, y, z, 'z');
				AxisCheck(x, y, z, 'y');
			}
			case 'y': {
				//xz
				AxisCheck(x, y, z, 'x');
				AxisCheck(x, y, z, 'z');
			}
			case 'z': {
				//yx
				AxisCheck(x, y, z, 'y');
				AxisCheck(x, y, z, 'x');
			}
			case 'n': {
				//xzy			
				AxisCheck(x, y, z, 'x');
				AxisCheck(x, y, z, 'z');
				AxisCheck(x, y, z, 'y');
			}
			}
		//}
	}
}

int Planet::Wraparound(int a) 
{
	return (a + 1) % 2;
}

void Planet::AxisCheck(int x, int y, int z, char axis) {
	int wrap;
	switch (axis) {
	case 'x': {
		wrap = Wraparound(x);
		//If next point is also negative and hasn't been visited, call recursive check
		if (m_cubeValues[wrap][y][z] < 0) {
			//if (m_visitedEmpty[wrap][y][z] == 0) {
				m_visitedEmpty[wrap][y][z] == 1;
				RecursiveCubeCheck(wrap, y, z, 'x');
			//}
		}
		//If next point is positive:
		else {
			CalculateVertex(x, y, z, 'x');
		}
	}
	case 'y': {
		wrap = Wraparound(y);
		if (m_cubeValues[x][wrap][z] < 0) {
			//if (m_visitedEmpty[x][wrap][z] == 0) {
				m_visitedEmpty[x][wrap][z] == 1;
				RecursiveCubeCheck(x, wrap, z, 'y');
			//}
		}
		else {
			CalculateVertex(x, y, z, 'y');
		}
	}
	case 'z': {
		wrap = Wraparound(z);
		if (m_cubeValues[x][y][wrap] < 0) {
			//if (m_visitedEmpty[x][y][wrap] == 0) {
				m_visitedEmpty[x][y][wrap] == 1;
				RecursiveCubeCheck(x, y, wrap, 'z');
			//}
		}
		else {
			CalculateVertex(x, y, z, 'z');
		}
	}
	}
	
}

void Planet::CalculateVertex(int x, int y, int z, char axis) {
	XMFLOAT4 Color = { 0.5f, 0.5f, 0.5f, 1.0f };
	switch (axis) {
	case 'x': {
		int wrap = Wraparound(x);
		if (x == 0) {
			m_tempVertices.push_back({ { (m_currentCoords[0] + std::abs(m_cubeValues[x][y][z] / (std::abs(m_cubeValues[x][y][z]) + std::abs(m_cubeValues[wrap][y][z])))),
			(m_currentCoords[1]), (m_currentCoords[2]), 1.0f }, Color });
		}
		else {
			m_tempVertices.push_back({ { (m_currentCoords[0] + (1 - std::abs(m_cubeValues[x][y][z] / (std::abs(m_cubeValues[x][y][z]) + std::abs(m_cubeValues[wrap][y][z]))))),
			m_currentCoords[1], m_currentCoords[2], 1.0f }, Color });
		}
	}
	case 'y': {
		int wrap = Wraparound(y);
		if (y == 0) {
			m_tempVertices.push_back({ { m_currentCoords[0],
				(m_currentCoords[1] + std::abs(m_cubeValues[x][y][z] / (std::abs(m_cubeValues[x][y][z]) + std::abs(m_cubeValues[x][wrap][z])))),
				m_currentCoords[2], 1.0f }, Color });
		}
		else {
			m_tempVertices.push_back({ {m_currentCoords[0],
				(m_currentCoords[1] + (1 - std::abs(m_cubeValues[x][y][z] / (std::abs(m_cubeValues[x][y][z]) + std::abs(m_cubeValues[x][wrap][z]))))),
				m_currentCoords[2], 1.0f}, Color });
		}
	}
	case 'z': {
		int wrap = Wraparound(z);
		if (z == 0) {
			m_tempVertices.push_back({ {m_currentCoords[0], m_currentCoords[1],
				(m_currentCoords[2] + std::abs(m_cubeValues[x][y][z] / (std::abs(m_cubeValues[x][y][z]) + std::abs(m_cubeValues[x][y][wrap])))), 1.0f},
				Color });
		}
		else {
			m_tempVertices.push_back({ {m_currentCoords[0], m_currentCoords[1],
				(m_currentCoords[2] + (1 - std::abs(m_cubeValues[x][y][z] / (std::abs(m_cubeValues[x][y][z]) + std::abs(m_cubeValues[x][y][wrap]))))), 1.0f},
				Color });
		}
	}
	}
}