#pragma once
#include "pch.h"
#include "InputOutput.h"
#include "World.h"
#include <fstream>
#include <winsock2.h>

//Warning: this import/export does not currently support serialization and handling variations in Endianness arrangements.
//Files export from here may not import correctly on other machines.

using namespace DirectX;

InputOutput::InputOutput() : m_planetExists(0){}

bool InputOutput::ImportWorldInfo(std::wstring worldName, World* world) {
	std::wstring msg;
	OutputDebugString(L"Importing world.\n");
	std::ifstream inputStream(worldName, std::ios::in | std::ios::binary);
	std::unique_ptr<char[]> ID = std::make_unique<char[]>(1);
	std::unique_ptr<char[]> positionElement = std::make_unique<char[]>(4);
	
	//Values for importing data
	float posX;	float posY;	float posZ;
	float rotX;	float rotY;	float rotZ; float rotW;
	float planetRadius;
	XMVECTOR planetPos;
	int arraySize;
	UINT arrayElementCount;
	float voxelData;

	//If there is no information in the file, return 0 for failed load.
	if (inputStream.peek() == EOF) {
		return 0;
	}
	//Loop through file for data.
	while (inputStream.read(ID.get(), sizeof(char))) {
		//If a cube
		if (static_cast<int>(ID[0]) == 1) {
			//Getting positions
			inputStream.read(reinterpret_cast<char*>(&posX), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&posY), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&posZ), sizeof(float));
			//Getting rotations
			inputStream.read(reinterpret_cast<char*>(&rotX), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&rotY), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&rotZ), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&rotW), sizeof(float));
			world->AddBlock(XMVectorSet(posX, posY, posZ, 1.0f), XMVectorSet(rotX, rotY, rotZ, rotW));
		}
		//If a planet
		else if (static_cast<int>(ID[0]) == 2) {
			OutputDebugString(L"Planet imported.\n");
			m_planetExists = 1;
			//Get position
			inputStream.read(reinterpret_cast<char*>(&posX), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&posY), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&posZ), sizeof(float));
			//Get Radius
			inputStream.read(reinterpret_cast<char*>(&planetRadius), sizeof(float));
			planetPos = XMVectorSet(posX, posY, posZ, 1.0f);
			world->AddPlanet(planetPos, planetRadius);
			arraySize = (int)std::ceil(planetRadius) * 2 + 1;
			//Get number of elements in array
			inputStream.read(reinterpret_cast<char*>(&arrayElementCount), sizeof(UINT));
			//Get voxel data
			for (int x = 0; x < arraySize; x++) {
				for (int y = 0; y < arraySize; y++) {
					for (int z = 0; z < arraySize; z++) {
						inputStream.read(reinterpret_cast<char*>(&voxelData), sizeof(float));
						if (voxelData > 1.0f || voxelData < -1.0f) {
							OutputDebugString(L"Float is out of bounds.\n");
						}
						world->GetPlanet(world->GetPlanetCount() - 1)->SetData(x, y, z, voxelData);
					}
				}
			}
		}
		//If a player
		if (static_cast<int>(ID[0]) == 3) {
			//Getting positions
			inputStream.read(reinterpret_cast<char*>(&posX), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&posY), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&posZ), sizeof(float));
			//Getting rotations
			inputStream.read(reinterpret_cast<char*>(&rotX), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&rotY), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&rotZ), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&rotW), sizeof(float));
			XMFLOAT4 playerPos, playerRot; 
			XMStoreFloat4(&playerPos, XMVectorSet(posX, posY, posZ, 1.0f));
			world->GetPlayer()->SetObjectPos(playerPos);
			XMStoreFloat4(&playerRot, XMVectorSet(rotX, rotY, rotZ, rotW));
			world->GetPlayer()->SetObjectPos(playerPos);
		}

		//If a Spaceship
		if (static_cast<int>(ID[0]) == 4) {
			//Getting positions
			inputStream.read(reinterpret_cast<char*>(&posX), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&posY), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&posZ), sizeof(float));
			//Getting rotations
			inputStream.read(reinterpret_cast<char*>(&rotX), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&rotY), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&rotZ), sizeof(float));
			inputStream.read(reinterpret_cast<char*>(&rotW), sizeof(float));
			world->AddShip(XMVectorSet(posX, posY, posZ, 1.0f), XMVectorSet(rotX, rotY, rotZ, rotW));
		}
	}
	inputStream.close();
	return 1;
}

bool InputOutput::ExportWorldInfo(std::wstring worldName, World* world) {
	OutputDebugString(L"Exporting world.\n");
	std::ofstream outputStream(worldName, std::ios::out | std::ios::binary);
	bool sucessfullyOpened = outputStream.is_open();
	if (sucessfullyOpened) {
		OutputDebugString(L"File sucessfully created/opened.\n");
	}
	else {
		OutputDebugString(L"File failed to open.\n");
	}
	//Block is 00000001 or 1
	char cubeID = 0b00000001;
	for (int i = 0; i < world->GetBlockCount(); i++) {
		//Write block ID
		outputStream.write(reinterpret_cast<char*>(&cubeID), sizeof(cubeID));
		XMFLOAT4 blockPos = world->GetBlock(i)->GetObjectPos();
		outputStream.write(reinterpret_cast<char*>(&blockPos.x), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&blockPos.y), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&blockPos.z), sizeof(float));
		XMFLOAT4 rotation = world->GetBlock(i)->GetObjectRot();
		outputStream.write(reinterpret_cast<char*>(&rotation.x), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&rotation.y), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&rotation.z), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&rotation.w), sizeof(float));
	}

	//Planet is 00000010 or 2
	char planetID = 0b00000010;
	for (int i = 0; i < world->GetPlanetCount(); i++) {
		//Write ID
		outputStream.write(reinterpret_cast<char*>(&planetID), sizeof(planetID));
		XMFLOAT4 planetPos = world->GetPlanet(i)->GetObjectPos();
		//Write three position elements
		outputStream.write(reinterpret_cast<char*>(&planetPos.x), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&planetPos.y), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&planetPos.z), sizeof(float));
		//Write planet radius for later reconstruction
		float radius = world->GetPlanet(i)->GetPlanetRadius();
		outputStream.write(reinterpret_cast<char*>(&radius), sizeof(float));
		std::vector<std::vector<std::vector<float>>> planetData = world->GetPlanet(i)->GetVoxelData();
		UINT arraySize = planetData.size() * planetData[0].size() * planetData[0][0].size();
		//Next byte is the number of voxels to save/load
		outputStream.write(reinterpret_cast<char*>(&arraySize), sizeof(UINT));
		float value;

		//Output loop for voxel data.  Just positions for now.  Maybe materials later.
		for (UINT x = 0; x < planetData.size(); x++) {
			for (UINT y = 0; y < planetData[x].size(); y++) {
				for (UINT z = 0; z < planetData[x][y].size(); z++) {
					value = planetData[x][y][z];
					outputStream.write(reinterpret_cast<char*>(&value), sizeof(float));
				}
			}
		}
	}

	{
		//Player is 00000011 or 3
		char playerID = 0b00000011;
		outputStream.write(reinterpret_cast<char*>(&playerID), sizeof(playerID));
		Player* player = world->GetPlayer();
		XMFLOAT4 playerPos = player->GetObjectPos();
		//Write three position elements
		outputStream.write(reinterpret_cast<char*>(&playerPos.x), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&playerPos.y), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&playerPos.z), sizeof(float));
		XMFLOAT4 playerRot = player->GetObjectRot();
		outputStream.write(reinterpret_cast<char*>(&playerRot.x), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&playerRot.y), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&playerRot.z), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&playerRot.w), sizeof(float));
	}

	//Spaceship is 00000100 or 4
	char spaceshipID = 0b00000100;
	for (int i = 0; i < world->GetSpaceshipCount(); i++) {
		//Write block ID
		outputStream.write(reinterpret_cast<char*>(&spaceshipID), sizeof(spaceshipID));
		XMFLOAT4 spaceshipPos = world->GetSpaceship(i)->GetObjectPos();
		outputStream.write(reinterpret_cast<char*>(&spaceshipPos.x), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&spaceshipPos.y), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&spaceshipPos.z), sizeof(float));
		XMFLOAT4 spaceshipRot = world->GetSpaceship(i)->GetObjectRot();
		outputStream.write(reinterpret_cast<char*>(&spaceshipRot.x), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&spaceshipRot.y), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&spaceshipRot.z), sizeof(float));
		outputStream.write(reinterpret_cast<char*>(&spaceshipRot.w), sizeof(float));
	}

	outputStream.close();
	return sucessfullyOpened;
}

bool InputOutput::DoesPlanetExist() {
	return m_planetExists;
}
