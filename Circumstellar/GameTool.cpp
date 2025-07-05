#pragma once
#include "pch.h"
#include "GameTool.h"

using namespace DirectX;

GameTool::GameTool() : m_terrainChangeRadius(1.5f){}

GameTool::GameTool(float terrainChangeRadius) : m_terrainChangeRadius(terrainChangeRadius) {}

int GameTool::GetCurrentTool() {
	return m_currentTool;
}
void GameTool::SetCurrentTool(int tool) {
	m_currentTool = tool;
}

bool GameTool::ChangeTerrain(Planet* planet, Camera* currentCam, float changePower) {
	std::wstring msg;

	XMFLOAT4 tempLoadFloat = planet->getObjectRot();
	XMVECTOR planetRot = XMLoadFloat4(&tempLoadFloat); tempLoadFloat = planet->getObjectPos();
	XMVECTOR planetPos = XMLoadFloat4(&tempLoadFloat); tempLoadFloat = currentCam->GetOrientation();

	XMVECTOR invertedPlanetRot = XMQuaternionInverse(planetRot);

	XMVECTOR camRayPos = currentCam->GetForwardRay(10.0f);
	camRayPos = XMVectorSubtract(camRayPos, planetPos);
	camRayPos = XMVector3Rotate(camRayPos, invertedPlanetRot);
	XMVECTOR camRayRot = XMVector3Rotate(currentCam->GetOrientationVector(), invertedPlanetRot);
	XMFLOAT4 rayPosition;
	XMStoreFloat4(&rayPosition, camRayPos);

	XMFLOAT4 debugFloat;
	XMStoreFloat4(&debugFloat, camRayPos);
	//msg = L"Camera ray position: " + std::to_wstring(debugFloat.x); msg += L", " + std::to_wstring(debugFloat.y); msg += L", " + std::to_wstring(debugFloat.z);
	//msg += L".\n"; OutputDebugString(msg.c_str());
	
	XMStoreFloat4(&debugFloat, planetPos);
	//msg = L"Planet position: " + std::to_wstring(debugFloat.x); msg += L", " + std::to_wstring(debugFloat.y); msg += L", " + std::to_wstring(debugFloat.z);
	//msg += L".\n"; OutputDebugString(msg.c_str());
	
	XMStoreFloat4(&debugFloat, camRayPos);
	//msg = L"Camera position: " + std::to_wstring(debugFloat.x); msg += L", " + std::to_wstring(debugFloat.y); msg += L", " + std::to_wstring(debugFloat.z);
	//msg += L".\n"; OutputDebugString(msg.c_str());
	
	float distance = std::sqrt(std::pow(rayPosition.x, 2) + std::pow(rayPosition.y, 2) + std::pow(rayPosition.z, 2));
	//msg = L"Distance of ray end from planet: " + std::to_wstring(distance);  msg += L"\n"; OutputDebugString(msg.c_str());
	
	float planetArrayRadius = static_cast<float>(planet->GetArrayRadius()); //Get planet's integer array maximum
	float arrayRadiusCorner = static_cast<float>(std::sqrt(std::pow(planetArrayRadius, 2) * 3));
	msg = L"Is it close enough? " + std::to_wstring((distance - m_terrainChangeRadius) < arrayRadiusCorner); msg += L"\n"; OutputDebugString(msg.c_str());
	if ((distance - m_terrainChangeRadius) < arrayRadiusCorner) {
		//OutputDebugString(L"Making array and changes.\n");
		int planetArrayLength = planetArrayRadius*2+1;
		int arrayMinX = std::clamp(static_cast<int>(std::ceil(rayPosition.x - m_terrainChangeRadius) + planetArrayRadius), 0, planetArrayLength - 1);
		int arrayMaxX = std::clamp(static_cast<int>(std::floor(rayPosition.x + m_terrainChangeRadius) + planetArrayRadius), 0, planetArrayLength - 1);
		int arrayMinY = std::clamp(static_cast<int>(std::ceil(rayPosition.y - m_terrainChangeRadius) + planetArrayRadius), 0, planetArrayLength - 1);
		int arrayMaxY = std::clamp(static_cast<int>(std::floor(rayPosition.y + m_terrainChangeRadius) + planetArrayRadius), 0, planetArrayLength - 1);
		int arrayMinZ = std::clamp(static_cast<int>(std::ceil(rayPosition.z - m_terrainChangeRadius) + planetArrayRadius), 0, planetArrayLength - 1);
		int arrayMaxZ = std::clamp(static_cast<int>(std::floor(rayPosition.z + m_terrainChangeRadius) + planetArrayRadius), 0, planetArrayLength - 1);
		//msg = L"Array min/max x/y/z respectively: " + std::to_wstring(arrayMinX);
		//msg += L", " + std::to_wstring(arrayMaxX);
		//msg += L", " + std::to_wstring(arrayMinY);
		//msg += L", " + std::to_wstring(arrayMaxY);
		//msg += L", " + std::to_wstring(arrayMinZ);
		//msg += L", " + std::to_wstring(arrayMaxZ);
		//msg += L".\n"; OutputDebugString(msg.c_str());
		for (int z = arrayMinZ; z <= arrayMaxZ; z++) {
			for (int y = arrayMinY; y <= arrayMaxY; y++) {
				for (int x = arrayMinX; x <= arrayMaxX; x++) {
					//msg = L"Editing point " + std::to_wstring(x); msg += L", " + std::to_wstring(y); msg += L", " + std::to_wstring(z); msg += L"\n"; OutputDebugString(msg.c_str());
					planet->EditData(x, y, z, changePower);
				}
			}
		}
		return true;
	}
	return false;
}