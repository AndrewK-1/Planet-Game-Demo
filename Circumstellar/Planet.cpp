#pragma once
#include "pch.h"
#include "Planet.h"
#include <algorithm>

using namespace DirectX;

Planet::Planet() 
	: WorldObject(), m_radius(10), m_voxelPadding(5), m_voxelData({}), m_tempVertices({}), m_cubeValues{}, m_visitedEmpty{}, m_currentCoords{} 
{
	m_arrayRadius = static_cast<int>(std::ceil(m_radius) + m_voxelPadding + 1);
	GenerateData();
}

Planet::Planet(float radius) 
	: WorldObject(), m_voxelData({}), m_radius(radius), m_voxelPadding(5), m_tempVertices({}), m_cubeValues{}, m_visitedEmpty{}, m_currentCoords{}
{
	m_arrayRadius = static_cast<int>(std::ceil(m_radius) + m_voxelPadding + 1);
	GenerateData();
}

Planet::Planet(float radius, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale) 
	: WorldObject(position, rotation, scale), m_radius(radius), m_voxelPadding(5), m_voxelData({}), m_tempVertices({}), m_cubeValues{}, m_visitedEmpty{}, m_currentCoords{}
{
	m_arrayRadius = static_cast<int>(std::ceil(m_radius) + m_voxelPadding + 1);
	GenerateData();
}

std::vector<CustomGeometry::VertexWNormal>* Planet::GetGeometry() {
	return &m_geometry;
}

int Planet::GetArrayRadius() {
	return m_arrayRadius;
}

int Planet::GetVertexCount() {
	return m_vertexCount;
}

std::vector<UINT> Planet::GetIndexArray() {
	return m_indexArray;
}

std::vector<std::vector<std::vector<float>>>& Planet::GetVoxelData() {
	return m_voxelData;
}

float Planet::GetPlanetRadius() {
	return m_radius;
}

//Setting voxel array size and inserting float elements
void Planet::GenerateData() {
	OutputDebugString(L"Generating data...");
	m_voxelData.clear();
	//Values adjusted for zero box around data
	int ceilradius = (int)std::ceil(m_radius) + 1 + m_voxelPadding;
	int arraysize = (ceilradius) * 2 + 1;
	int arraymax = ceilradius + 1;
	m_voxelData.resize(arraysize);
	//Setting size of voxel array
	for (int i = 0; i < arraysize; i++) {
		m_voxelData[i].resize(arraysize);
		for (int j = 0; j < arraysize; j++) {
			m_voxelData[i][j].resize(arraysize);
		}
	}
	//Loop for inserting floats into voxel array
	for (int z = -ceilradius; z < arraymax; z++) {
		for (int y = -ceilradius; y < arraymax; y++) {
			for (int x = -ceilradius; x < arraymax; x++) {
				if(x == -ceilradius || y == -ceilradius || z == -ceilradius || x == arraymax || y == arraymax || z == arraymax) {
					//If any coordinate is zero, it is the outside box, set to empty
					m_voxelData[x + ceilradius][y + ceilradius][z + ceilradius] = -1.0f;
				}
				else {
					float num = std::clamp(static_cast<float>(m_radius - std::sqrt((std::pow((x), 2) + std::pow((y), 2) + std::pow((z), 2)))), -1.0f, 1.0f);
					//std::wstring msg = L"Float at " + std::to_wstring(x); msg += L", " + std::to_wstring(y); msg += L", " + std::to_wstring(z); msg += L": " + std::to_wstring(num); msg += L"\n";
					//OutputDebugString(msg.c_str());
					m_voxelData[x + ceilradius][y + ceilradius][z + ceilradius] = num;
				}
			}
		}
	}
}

void Planet::EditData(int x, int y, int z, float value) {
	if(x == 0 || x == m_voxelData.size() -1 || y == 0 || y == m_voxelData[0].size() - 1 || z == 0 || z == m_voxelData[0][0].size() - 1) {

	}
	else {
		m_voxelData[x][y][z] = std::clamp(m_voxelData[x][y][z] + value, -1.0f, 1.0f);
	}
	GenerateGeometry();
}

void Planet::SetData(int x, int y, int z, float value) {
	if (x == 0 || x == m_voxelData.size() - 1 || y == 0 || y == m_voxelData[0].size() - 1 || z == 0 || z == m_voxelData[0][0].size() - 1) {

	}
	else {
		m_voxelData[x][y][z] = std::clamp(value, -1.0f, 1.0f);
	}
	GenerateGeometry();
}

void Planet::GenerateGeometry() {
	m_vertexCount = 0;
	#if DEBUG
	OutputDebugString(L"Generating Geometry.\n");
	#endif
	m_geometry.clear();
	m_indexArray.clear();
	m_vertexCount = 0;
	//truncated radius to assist in defining the loop variables
	int ceilradius = (int)std::ceil(m_radius) + 1 + m_voxelPadding;
	//Array maximum.  Beyond this point groupings will be outside the sphere's intersection.
	int arraymax = ceilradius*2;
	#if DEBUG
	OutputDebugString(L"Array start: "); OutputDebugString(std::to_wstring(-ceilradius).c_str()); OutputDebugString(L"\n");
	OutputDebugString(L"Array limit: "); OutputDebugString(std::to_wstring(arraymax).c_str()); OutputDebugString(L"\n");
	#endif
	//Geometry loop.  i is z, j is y, k is x.  ii is z, ij is y, ik is x.
	
	for (int i = 0; i < arraymax; i++) {
		m_currentCoords[2] = static_cast<float>(i) - static_cast<float>(m_arrayRadius);
		for (int j = 0; j < arraymax; j++) {
			m_currentCoords[1] = static_cast<float>(j) - static_cast<float>(m_arrayRadius);
			for (int k = 0; k < arraymax; k++) {
				m_currentCoords[0] = static_cast<float>(k) - static_cast<float>(m_arrayRadius);

				#if DEBUG
				OutputDebugString(L"K: "); OutputDebugString(std::to_wstring(k).c_str());
				OutputDebugString(L" J: "); OutputDebugString(std::to_wstring(j).c_str());
				OutputDebugString(L" I: "); OutputDebugString(std::to_wstring(i).c_str());
				OutputDebugString(L"\n");
				OutputDebugString(L"K: "); OutputDebugString(std::to_wstring(m_currentCoords[0]).c_str());
				OutputDebugString(L" J: "); OutputDebugString(std::to_wstring(m_currentCoords[1]).c_str());
				OutputDebugString(L" I: "); OutputDebugString(std::to_wstring(m_currentCoords[2]).c_str());
				OutputDebugString(L"\n");
				#endif			
				
				bool allNegative = 1;
				bool allPositive = 1;
				//value of each vertex in relation to the sphere's radius
				
				for (int ii = 0; ii < 2; ii++) {
					
					for (int ij = 0; ij < 2; ij++) {
						
						for (int ik = 0; ik < 2; ik++) {
							
							#if DEBUG
							OutputDebugString(L"Inserting value of cube at xyz: "); 
							OutputDebugString(std::to_wstring(m_voxelData[ik + k][ij + j][ii + i]).c_str());
							OutputDebugString(L"\n");
							#endif					
							//Negatives are air, positives are material
							m_cubeValues[ik][ij][ii] = m_voxelData[ik + k][ij + j][ii + i];//std::clamp(static_cast<float>(m_radius - std::sqrt((std::pow((k + ik),2) + std::pow((j + ij),2) + std::pow((i + ii),2)))), -1.0f, 1.0f);
							if (m_cubeValues[ik][ij][ii] > 0) {
								allNegative = 0;

							}
							if (m_cubeValues[ik][ij][ii] < 0) {
								allPositive = 0;
							}
						}
					}
				}
				#if DEBUG
				OutputDebugString(L"All Negative: ");
				OutputDebugString(std::to_wstring(allNegative).c_str());
				OutputDebugString(L"  All Positive: ");
				OutputDebugString(std::to_wstring(allPositive).c_str());
				OutputDebugString(L".\n");
				#endif
				//If all vertices are empty or full, skip geometry drawing
				if ((allNegative == 0) && (allPositive == 0)) {
					#if DEBUG
					OutputDebugString(L"Creating Geometry...\n");
					#endif
					//Generating the actual geometry
					for (int zZero = 0; zZero < 2; zZero++) {
						for (int yZero = 0; yZero < 2; yZero++) {
							for (int xZero = 0; xZero < 2; xZero++) {
								m_visitedEmpty[xZero][yZero][zZero] = 0;
							}
						}
					}
					for (int z = 0; z < 2; z++) {
						for (int y = 0; y < 2; y++) {
							for (int x = 0; x < 2; x++) {
								#if DEBUG
								OutputDebugString(L"Value of cube at xyz: "); OutputDebugString(std::to_wstring(m_cubeValues[x][y][z]).c_str()); OutputDebugString(L"\n");
								#endif
								//If current is some value of empty
								if (m_cubeValues[x][y][z] < 0) {
									//If it hasn't been visited yet
									#if DEBUG
									OutputDebugString(L"Has value been visited?: "); OutputDebugString(std::to_wstring(m_visitedEmpty[x][y][z] == 1).c_str()); OutputDebugString(L"\n");
									#endif
									if (m_visitedEmpty[x][y][z] == 0) {
										m_tempVertices.clear();
										m_visitedEmpty[x][y][z] = 1;
										RecursiveCubeCheck(x, y, z, 'n');
										
										#if DEBUG
										OutputDebugString(L"Generating "); OutputDebugString(std::to_wstring(m_tempVertices.size() - 2).c_str()); 
										OutputDebugString(L" triangle(s).\n");
										#endif	

										for (int triIndex = 0; triIndex < m_tempVertices.size(); triIndex++) {
											XMFLOAT4 vertex = m_tempVertices[triIndex].V_Position;
											
											#if DEBUG
											std::wstring triMsg = L"Vertex "; triMsg += std::to_wstring(triIndex); triMsg += L": ";
											triMsg += std::to_wstring(vertex.x); triMsg += L", ";
											triMsg += std::to_wstring(vertex.y); triMsg += L", ";
											triMsg += std::to_wstring(vertex.z); triMsg += L"\n";
											OutputDebugString(triMsg.c_str());
											#endif

										}
										for (int triIndex = 2; triIndex < m_tempVertices.size(); triIndex++) {
											m_vertexCount += 3;
											XMVECTOR vertX, vertY, vertZ, normalVec;
											vertX = XMLoadFloat4(&m_tempVertices[0].V_Position);
											vertY = XMLoadFloat4(&m_tempVertices[triIndex - 1].V_Position);
											vertZ = XMLoadFloat4(&m_tempVertices[triIndex].V_Position);
											normalVec = XMVector3Cross(XMVectorSubtract(vertZ, vertY), XMVectorSubtract(vertX, vertY));
											XMFLOAT4 normalFloat;
											XMStoreFloat4(&normalFloat, normalVec);

											m_geometry.push_back({ m_tempVertices[0].V_Position, normalFloat, m_tempVertices[0].V_Color });
											m_geometry.push_back({ m_tempVertices[triIndex - 1].V_Position, normalFloat, m_tempVertices[triIndex - 1].V_Color });
											m_geometry.push_back({ m_tempVertices[triIndex].V_Position, normalFloat, m_tempVertices[triIndex].V_Color });
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
	for (UINT i = 0; i < m_geometry.size(); i++) {
		m_indexArray.push_back(i);
	}
}

//Recursively iterate over points to generate geometry
void Planet::RecursiveCubeCheck(int x, int y, int z, char translationAxis)
{
	//If the current point is one of four points going in a certain direction.  This is for clockwise winding.
	if (!((x == 0 && y == 0 && z == 0) || (x == 1 && y == 1 && z == 0) || (x == 0 && y == 1 && z == 1) || (x == 1 && y == 0 && z == 1))) {
		//Check and visitedEmpty adjustment is also done in Axis Check, so might change that
		//If the vertex is negative (empty), then it is visited and will generate geometry
		//if (m_cubeValues[x][y][z] < 0) {
			//m_visitedEmpty[x][y][z] = 1;
			switch (translationAxis) {
			case 'x': {
				//yz
				AxisCheck(x, y, z, 'y');
				AxisCheck(x, y, z, 'z');
				break;
			}
			case 'y': {
				//zx
				AxisCheck(x, y, z, 'z');
				AxisCheck(x, y, z, 'x');
				break;
			}
			case 'z': {
				//xy
				AxisCheck(x, y, z, 'x');
				AxisCheck(x, y, z, 'y');
				break;
			}
			case 'n': {
				//xyz		
				AxisCheck(x, y, z, 'x');
				AxisCheck(x, y, z, 'y');
				AxisCheck(x, y, z, 'z'); 
				break;
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
				break;
			}
			case 'y': {
				//xz
				AxisCheck(x, y, z, 'x');
				AxisCheck(x, y, z, 'z');
				break;
			}
			case 'z': {
				//yx
				AxisCheck(x, y, z, 'y');
				AxisCheck(x, y, z, 'x');
				break;
			}
			case 'n': {
				//xzy			
				AxisCheck(x, y, z, 'x');
				AxisCheck(x, y, z, 'z');
				AxisCheck(x, y, z, 'y');
				break;
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
			if (m_visitedEmpty[wrap][y][z] == 0) {
				m_visitedEmpty[wrap][y][z] = 1;
				RecursiveCubeCheck(wrap, y, z, 'x');
			}
		}
		//If next point is positive:
		else {
			CalculateVertex(x, y, z, 'x');
		}
		break;
	}
	case 'y': {
		wrap = Wraparound(y);
		if (m_cubeValues[x][wrap][z] < 0) {
			if (m_visitedEmpty[x][wrap][z] == 0) {
				m_visitedEmpty[x][wrap][z] = 1;
				RecursiveCubeCheck(x, wrap, z, 'y');
			}
		}
		else {
			CalculateVertex(x, y, z, 'y');
		}
		break;
	}
	case 'z': {
		wrap = Wraparound(z);
		if (m_cubeValues[x][y][wrap] < 0) {
			if (m_visitedEmpty[x][y][wrap] == 0) {
				m_visitedEmpty[x][y][wrap] = 1;
				RecursiveCubeCheck(x, y, wrap, 'z');
			}
		}
		else {
			CalculateVertex(x, y, z, 'z');
		}
		break;
	}
	}
	
}

void Planet::CalculateVertex(int x, int y, int z, char axis) {
	XMFLOAT4 Color = { 0.5f, 0.5f, 0.5f, 1.0f };
	switch (axis) {
	case 'x': {
		int wrap = Wraparound(x);
		float air = std::abs(m_cubeValues[x][y][z]);
		float solid = std::abs(m_cubeValues[wrap][y][z]);
		//Determines which direction along the x axis the vertex is being calculated.
		if (x == 0) {
			m_tempVertices.push_back({ { (m_currentCoords[0] + (air / (air + solid))),
			(m_currentCoords[1]) + static_cast<float>(y), (m_currentCoords[2]) + static_cast<float>(z), 1.0f }, Color });
		}
		else {
			m_tempVertices.push_back({ { (m_currentCoords[0] + (1.0f - (air / (air + solid)))),
			m_currentCoords[1] + static_cast<float>(y), m_currentCoords[2] + static_cast<float>(z), 1.0f }, Color });
		}
		break;
	}
	case 'y': {
		int wrap = Wraparound(y);
		float air = std::abs(m_cubeValues[x][y][z]);
		float solid = std::abs(m_cubeValues[x][wrap][z]);
		if (y == 0) {
			m_tempVertices.push_back({ { m_currentCoords[0] + static_cast<float>(x),
				(m_currentCoords[1] + (air / (air + solid))),
				m_currentCoords[2] + static_cast<float>(z), 1.0f }, Color });
		}
		else {
			m_tempVertices.push_back({ {m_currentCoords[0] + static_cast<float>(x),
				(m_currentCoords[1] + (1.0f - (air / (air + solid)))),
				m_currentCoords[2] + static_cast<float>(z), 1.0f}, Color });
		}
		break;
	}
	case 'z': {
		int wrap = Wraparound(z);
		float air = std::abs(m_cubeValues[x][y][z]);
		float solid = std::abs(m_cubeValues[x][y][wrap]);
		if (z == 0) {
			m_tempVertices.push_back({ {m_currentCoords[0] + static_cast<float>(x), m_currentCoords[1] + static_cast<float>(y),
				(m_currentCoords[2] + (air / (air +solid))), 1.0f},
				Color });
		}
		else {
			m_tempVertices.push_back({ {m_currentCoords[0] + static_cast<float>(x), m_currentCoords[1] + static_cast<float>(y),
				(m_currentCoords[2] + (1.0f - (air / (air + solid)))), 1.0f},
				Color });
		}
		break;
	}
	}
}