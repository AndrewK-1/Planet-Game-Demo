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

std::vector<CustomGeometry::Vertex>* Planet::GetGeometry() {
	return &m_geometry;
}

void Planet::GenerateGeometry() {
	#if DEBUG
	OutputDebugString(L"Generating Geometry.\n");
	#endif
	m_geometry.clear();
	//truncated radius to assist in defining the loop variables
	int ceilradius = (int)std::ceil(m_radius);
	//Array maximum.  Beyond this point groupings will be outside the sphere's intersection.
	int arraymax = (int)std::ceil(m_radius)+1;
	#if DEBUG
	OutputDebugString(L"Array start: "); OutputDebugString(std::to_wstring(-ceilradius).c_str()); OutputDebugString(L"\n");
	OutputDebugString(L"Array limit: "); OutputDebugString(std::to_wstring(arraymax).c_str()); OutputDebugString(L"\n");
	#endif
	//Geometry loop.  i is z, j is y, k is x.  ii is z, ij is y, ik is x.
	for (int i = -ceilradius; i < arraymax; i++) {
		m_currentCoords[2] = static_cast<float>(i);
		for (int j = -ceilradius; j < arraymax; j++) {
			m_currentCoords[1] = static_cast<float>(j);
			for (int k = -ceilradius; k < arraymax; k++) {
				m_currentCoords[0] = static_cast<float>(k);
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
							OutputDebugString(std::to_wstring(static_cast<float>(m_radius - std::sqrt((std::pow((k + ik),2) + std::pow((j + ij),2) + std::pow((i + ii),2))))).c_str());
							OutputDebugString(L"\n");
							#endif
							//Negatives are air, positives are material
							m_cubeValues[ik][ij][ii] = static_cast<float>(m_radius - std::sqrt((std::pow((k + ik),2) + std::pow((j + ij),2) + std::pow((i + ii),2))));
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
	if ((x == 0 && y == 0 && z == 0) || (x == 1 && y == 1 && z == 0) || (x == 0 && y == 1 && z == 1) || (x == 1 && y == 0 && z == 1)) {
		//Check and visitedEmpty adjustment is also done in Axis Check, so might change that
		//If the vertex is negative (empty), then it is visited and will generate geometry
		//if (m_cubeValues[x][y][z] < 0) {
			//m_visitedEmpty[x][y][z] = 1;
			switch (translationAxis) {
			case 'x': {
				//yz
				OutputDebugString(L"Winding 1, Case X, Check Y.\n");
				AxisCheck(x, y, z, 'y');
				OutputDebugString(L"Winding 1, Case X, Check Z.\n");
				AxisCheck(x, y, z, 'z');
				break;
			}
			case 'y': {
				//zx
				OutputDebugString(L"Winding 1, Case Y, Check Z.\n");
				AxisCheck(x, y, z, 'z');
				OutputDebugString(L"Winding 1, Case Y, Check X.\n");
				AxisCheck(x, y, z, 'x');
				break;
			}
			case 'z': {
				//xy
				OutputDebugString(L"Winding 1, Case Z, Check X.\n");
				AxisCheck(x, y, z, 'x');
				OutputDebugString(L"Winding 1, Case Z, Check Y.\n");
				AxisCheck(x, y, z, 'y');
				break;
			}
			case 'n': {
				//xyz		
				OutputDebugString(L"Winding 1, Case N, Check X.\n");
				AxisCheck(x, y, z, 'x');
				OutputDebugString(L"Winding 1, Case N, Check Y.\n");
				AxisCheck(x, y, z, 'y');
				OutputDebugString(L"Winding 1, Case N, Check Z.\n");
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
				OutputDebugString(L"Winding 2, Case X, Check Z.\n");
				AxisCheck(x, y, z, 'z');
				OutputDebugString(L"Winding 2, Case X, Check Y.\n");
				AxisCheck(x, y, z, 'y');
				break;
			}
			case 'y': {
				//xz
				OutputDebugString(L"Winding 2, Case Y, Check X.\n");
				AxisCheck(x, y, z, 'x');
				OutputDebugString(L"Winding 2, Case Y, Check Z.\n");
				AxisCheck(x, y, z, 'z');
				break;
			}
			case 'z': {
				//yx
				OutputDebugString(L"Winding 2, Case Z, Check Y.\n");
				AxisCheck(x, y, z, 'y');
				OutputDebugString(L"Winding 2, Case Z, Check X.\n");
				AxisCheck(x, y, z, 'x');
				break;
			}
			case 'n': {
				//xzy			
				OutputDebugString(L"Winding 2, Case N, Check X.\n");
				AxisCheck(x, y, z, 'x');
				OutputDebugString(L"Winding 2, Case N, Check Z.\n");
				AxisCheck(x, y, z, 'z');
				OutputDebugString(L"Winding 2, Case N, Check Y.\n");
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
		std::wstring msg = (L" Checking axis X. From: ");
		msg += std::to_wstring(x); msg += L", "; msg += std::to_wstring(y); msg += L", "; msg += std::to_wstring(z); msg += L"\n";
		OutputDebugString(msg.c_str());
		wrap = Wraparound(x);
		//If next point is also negative and hasn't been visited, call recursive check
		if (m_cubeValues[wrap][y][z] < 0) {
			OutputDebugString(L" Is Negative.\n");
			if (m_visitedEmpty[wrap][y][z] == 0) {
				OutputDebugString(L" Not visited. \n");
				m_visitedEmpty[wrap][y][z] = 1;
				msg = {};
				msg = (L" Moving to point: ");
				msg += std::to_wstring(wrap); msg += L", "; msg += std::to_wstring(y); msg += L", "; msg += std::to_wstring(z); msg += L"\n";
				OutputDebugString(msg.c_str());
				RecursiveCubeCheck(wrap, y, z, 'x');
			}
		}
		//If next point is positive:
		else {
			OutputDebugString(L"Calculating Vertex.\n");
			CalculateVertex(x, y, z, 'x');
		}
		break;
	}
	case 'y': {
		std::wstring msg = (L" Checking axis Y. Form: ");
		msg += std::to_wstring(x); msg += L", "; msg += std::to_wstring(y); msg += L", "; msg += std::to_wstring(z); msg += L"\n";
		OutputDebugString(msg.c_str());
		wrap = Wraparound(y);
		if (m_cubeValues[x][wrap][z] < 0) {
			OutputDebugString(L" Is Negative.\n");
			if (m_visitedEmpty[x][wrap][z] == 0) {
				OutputDebugString(L" Not visited. \n");
				m_visitedEmpty[x][wrap][z] = 1;
				msg = {};
				msg = (L" Moving to point: ");
				msg += std::to_wstring(x); msg += L", "; msg += std::to_wstring(wrap); msg += L", "; msg += std::to_wstring(z); msg += L"\n";
				OutputDebugString(msg.c_str());
				RecursiveCubeCheck(x, wrap, z, 'y');
			}
		}
		else {
			OutputDebugString(L"Calculating Vertex.\n");
			CalculateVertex(x, y, z, 'y');
		}
		break;
	}
	case 'z': {
		std::wstring msg = (L" Checking axis Z. From: ");
		msg += std::to_wstring(x); msg += L", "; msg += std::to_wstring(y); msg += L", "; msg += std::to_wstring(z); msg += L"\n";
		OutputDebugString(msg.c_str());
		wrap = Wraparound(z);
		if (m_cubeValues[x][y][wrap] < 0) {
			OutputDebugString(L" Is Negative.\n");
			if (m_visitedEmpty[x][y][wrap] == 0) {
				OutputDebugString(L" Not visited. \n");
				m_visitedEmpty[x][y][wrap] = 1;
				msg = {};
				msg = (L" Moving to point: ");
				msg += std::to_wstring(x); msg += L", "; msg += std::to_wstring(y); msg += L", "; msg += std::to_wstring(wrap); msg += L"\n";
				OutputDebugString(msg.c_str());
				RecursiveCubeCheck(x, y, wrap, 'z');
			}
		}
		else {
			OutputDebugString(L"Calculating Vertex.\n");
			CalculateVertex(x, y, z, 'z');
		}
		break;
	}
	}
	
}

void Planet::CalculateVertex(int x, int y, int z, char axis) {
	std::wstring calcmsg = L"Calculating vertex from ";
	calcmsg += std::to_wstring(x); calcmsg += L", "; calcmsg += std::to_wstring(y); calcmsg += L", "; calcmsg += std::to_wstring(z);
	calcmsg += L" along axis "; calcmsg += axis; calcmsg += L".\n";
	OutputDebugString(calcmsg.c_str());
	XMFLOAT4 Color = { 0.5f, 0.5f, 0.5f, 1.0f };
	switch (axis) {
	case 'x': {
		int wrap = Wraparound(x);
		float air = std::abs(m_cubeValues[x][y][z]);
		float solid = std::abs(m_cubeValues[wrap][y][z]);
		std::wstring switchmsg = L"Air value at xyz: "; switchmsg += std::to_wstring(air);
		switchmsg += L". Solid value at wrap yz: "; switchmsg += std::to_wstring(solid) + L"\n";
		OutputDebugString(switchmsg.c_str());
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
		std::wstring switchmsg = L"Air value at xyz: "; switchmsg += std::to_wstring(air);
		switchmsg += L". Solid value at x wrap z: "; switchmsg += std::to_wstring(solid) + L"\n";
		OutputDebugString(switchmsg.c_str());
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
		std::wstring switchmsg = L"Air value at xyz: "; switchmsg += std::to_wstring(air);
		switchmsg += L". Solid value at xy wrap: "; switchmsg += std::to_wstring(solid) + L"\n";
		OutputDebugString(switchmsg.c_str());
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