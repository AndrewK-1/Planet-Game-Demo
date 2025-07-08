#pragma once
#include "pch.h"
#include "World.h"
#include <random>

using namespace DirectX;

World::World() : m_planetArray(), m_blockArray() { m_player = std::make_unique<Player>(1.0f, 1.0f, 3.0f); }
World::World(std::vector<Planet> planetArray) : m_planetArray(planetArray) { m_player = std::make_unique<Player>(1.0f, 1.0f, 3.0f); }
World::World(std::vector<Block> blockArray) : m_blockArray(blockArray) { m_player = std::make_unique<Player>(1.0f, 1.0f, 3.0f); }
World::World(std::vector<Planet> planetArray, std::vector<Block> blockArray) : m_planetArray(planetArray), m_blockArray(blockArray) { m_player = std::make_unique<Player>(1.0f, 1.0f, 3.0f); }

void World::PushObject(Planet planet) {
	m_planetArray.push_back(planet);
}
void World::PushObject(Block block) {
	m_blockArray.push_back(block);
}
void World::RemoveObject(Planet planet, UINT index) {
	m_planetArray.erase(m_planetArray.begin() + index);
}
void World::RemoveObject(Block block, UINT index) {
	m_blockArray.erase(m_blockArray.begin() + index);
}

Planet* World::GetPlanet(UINT index) {
	return &m_planetArray[index];
}
Block* World::GetBlock(UINT index) {
	return &m_blockArray[index];
}
Player* World::GetPlayer() {
	return m_player.get();
}

float World::GetPlanetDistance(int index, XMFLOAT4 objPosition) {
	XMFLOAT4 planetPos = m_planetArray[index].GetObjectPos();
	objPosition.x -= planetPos.x;
	objPosition.y -= planetPos.y;
	objPosition.z -= planetPos.z;
	return std::sqrt(std::pow(objPosition.x, 2.0f) + std::pow(objPosition.y, 2.0f) + std::pow(objPosition.z, 2.0f));
}
float World::GetBlockDistance(int index, XMFLOAT4 objPosition){
	XMVECTOR objPos = XMLoadFloat4(&objPosition);
	XMFLOAT4 blockPos = m_blockArray[index].GetObjectPos();
	objPosition.x -= blockPos.x;
	objPosition.y -= blockPos.y;
	objPosition.z -= blockPos.z;
	return std::sqrt(std::pow(objPosition.x, 2.0f) + std::pow(objPosition.y, 2.0f) + std::pow(objPosition.z, 2.0f));
}

//Returns index of closest planet to specified position
UINT World::GetClosestPlanet(XMFLOAT4 objPosition) {
	UINT lowestIndex = 0;
	//Initialize lowestDistance with the first planet's 'distance' without using expensive sqrt function.
	XMFLOAT4 firstPlanetPos = m_planetArray[0].GetObjectPos();
	firstPlanetPos.x -= objPosition.x;
	firstPlanetPos.y -= objPosition.y;
	firstPlanetPos.z -= objPosition.z;
	float lowestDistance = std::pow(firstPlanetPos.x, 2.0f) + std::pow(firstPlanetPos.y, 2.0f) + std::pow(firstPlanetPos.z, 2.0f);
	float distanceCheck;
	for (UINT i = 0; i < m_planetArray.size(); i++) {
		XMFLOAT4 planetPos = m_planetArray[i].GetObjectPos();
		planetPos.x -= objPosition.x;
		planetPos.y -= objPosition.y;
		planetPos.z -= objPosition.z;
		distanceCheck = std::pow(planetPos.x, 2.0f) + std::pow(planetPos.y, 2.0f) + std::pow(planetPos.z, 2.0f);
		if (lowestDistance > distanceCheck) {
			lowestDistance = distanceCheck;
			lowestIndex = i;
		}
	}
	return lowestIndex;
}
UINT World::GetClosestBlock(XMFLOAT4 objPosition) {
	UINT lowestIndex = 0;
	//Initialize lowestDistance with the first block's 'distance' without using expensive sqrt function.
	XMFLOAT4 firstBlockPos = m_blockArray[0].GetObjectPos();
	firstBlockPos.x -= objPosition.x;
	firstBlockPos.y -= objPosition.y;
	firstBlockPos.z -= objPosition.z;
	float lowestDistance = std::pow(firstBlockPos.x, 2.0f) + std::pow(firstBlockPos.y, 2.0f) + std::pow(firstBlockPos.z, 2.0f);
	float distanceCheck;
	for (UINT i = 0; i < m_blockArray.size(); i++) {
		XMFLOAT4 blockPos = m_blockArray[i].GetObjectPos();
		blockPos.x -= objPosition.x;
		blockPos.y -= objPosition.y;
		blockPos.z -= objPosition.z;
		distanceCheck = std::pow(blockPos.x, 2.0f) + std::pow(blockPos.y, 2.0f) + std::pow(blockPos.z, 2.0f);
		if (lowestDistance > distanceCheck) {
			lowestDistance = distanceCheck;
			lowestIndex = i;
		}
	}
	return lowestIndex;
}

void World::AddPlanet(XMVECTOR position, float radius) {
	Planet newPlanet(radius, position, XMQuaternionIdentity(), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	m_planetArray.push_back(newPlanet);
}
void World::RemovePlanet(XMVECTOR position) {
	XMFLOAT4 positionFloat;
	XMStoreFloat4(&positionFloat, position);
	UINT closestPlanetIndex = GetClosestPlanet(positionFloat);
	m_planetArray.erase(m_planetArray.begin() + closestPlanetIndex);
}

void World::AddBlock(XMVECTOR position, XMVECTOR rotation) {
	Block newBlock(position, rotation, XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	m_blockArray.push_back(newBlock);
}
void World::RemoveBlock(XMVECTOR position) {
	XMFLOAT4 positionFloat;
	XMStoreFloat4(&positionFloat, position);
	UINT closestBlockIndex = GetClosestBlock(positionFloat);
	float distance = GetBlockDistance(closestBlockIndex, positionFloat);
	if (distance < 3.0f) {
		m_blockArray.erase(m_blockArray.begin() + closestBlockIndex);
	}
}

int World::GetBlockCount() {
	return m_blockArray.size();
}
int World::GetPlanetCount() {
	return m_planetArray.size();
}
XMMATRIX World::GetBlockMatrix(int index) {
	return m_blockArray[index].GetObjectMatrix();
}