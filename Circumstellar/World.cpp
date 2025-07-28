#pragma once
#include "pch.h"
#include "World.h"
#include <random>

using namespace DirectX;

World::World() : m_planetArray(), m_blockArray(), m_spaceshipArray(), m_isLoaded(0) { m_player = std::make_unique<Player>(1.0f, 0.5f, 3.0f, XMVectorSet(20.0f, 20.0f, 10.0f, 1.0f), XMQuaternionRotationRollPitchYaw(3.14f-3.14f/6, 3.14f/4, 0.0f)); }
World::World(std::vector<Planet> planetArray) : m_planetArray(planetArray), m_spaceshipArray(), m_isLoaded(0) { m_player = std::make_unique<Player>(1.0f, 0.5f, 3.0f); }
World::World(std::vector<Block> blockArray) : m_blockArray(blockArray), m_spaceshipArray(), m_isLoaded(0) { m_player = std::make_unique<Player>(1.0f, 0.5f, 3.0f); }
World::World(std::vector<Planet> planetArray, std::vector<Block> blockArray) : m_planetArray(planetArray), m_blockArray(blockArray), m_spaceshipArray(), m_isLoaded(0) { m_player = std::make_unique<Player>(1.0f, 0.5f, 3.0f); }

void World::PushObject(Planet planet) {
	m_planetArray.push_back(planet);
}
void World::PushObject(Block block) {
	m_blockArray.push_back(block);
}
void World::RemoveObject(Planet planet, UINT index) {
	if (!m_planetArray.empty()) {
		m_planetArray.erase(m_planetArray.begin() + index);
	}
}
void World::RemoveObject(Block block, UINT index) {
	if (!m_blockArray.empty()) {
		m_blockArray.erase(m_blockArray.begin() + index);
	}
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
Spaceship* World::GetSpaceship(UINT index) {
	return &m_spaceshipArray[index];
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
float World::GetSpaceshipDistance(int index, XMFLOAT4 objPosition) {
	XMVECTOR objPos = XMLoadFloat4(&objPosition);
	XMFLOAT4 spaceshipPos = m_spaceshipArray[index].GetObjectPos();
	objPosition.x -= spaceshipPos.x;
	objPosition.y -= spaceshipPos.y;
	objPosition.z -= spaceshipPos.z;
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
Planet* World::GetClosestPlanetPointer(XMFLOAT4 objPosition) {
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
	return &m_planetArray[lowestIndex];
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
UINT World::GetClosestSpaceship(XMFLOAT4 objPosition) {
	UINT lowestIndex = 0;
	//Initialize lowestDistance with the first block's 'distance' without using expensive sqrt function.
	XMFLOAT4 firstSpaceshipPos = m_spaceshipArray[0].GetObjectPos();
	firstSpaceshipPos.x -= objPosition.x;
	firstSpaceshipPos.y -= objPosition.y;
	firstSpaceshipPos.z -= objPosition.z;
	float lowestDistance = std::pow(firstSpaceshipPos.x, 2.0f) + std::pow(firstSpaceshipPos.y, 2.0f) + std::pow(firstSpaceshipPos.z, 2.0f);
	float distanceCheck;
	for (UINT i = 0; i < m_spaceshipArray.size(); i++) {
		XMFLOAT4 spaceshipPos = m_spaceshipArray[i].GetObjectPos();
		spaceshipPos.x -= objPosition.x;
		spaceshipPos.y -= objPosition.y;
		spaceshipPos.z -= objPosition.z;
		distanceCheck = std::pow(spaceshipPos.x, 2.0f) + std::pow(spaceshipPos.y, 2.0f) + std::pow(spaceshipPos.z, 2.0f);
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
	//If array is empty, do nothing
	if(m_planetArray.empty()) {
		return;
	}
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
	//If array is empty, do nothing
	if(m_blockArray.empty()) {
		return;
	}
	XMFLOAT4 positionFloat;
	XMStoreFloat4(&positionFloat, position);
	UINT closestBlockIndex = GetClosestBlock(positionFloat);
	float distance = GetBlockDistance(closestBlockIndex, positionFloat);
	if (distance < 3.0f) {
		m_blockArray.erase(m_blockArray.begin() + closestBlockIndex);
	}
}

void World::AddShip(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation) {
	Spaceship newSpaceship(10.0f, 3.0f, 5.0f, position, rotation, XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	m_spaceshipArray.push_back(newSpaceship);
}
void World::RemoveShip(DirectX::XMVECTOR position) {
	//If array is empty, do nothing
	if(m_spaceshipArray.empty()) {
		return;
	}
	XMFLOAT4 positionFloat;
	XMStoreFloat4(&positionFloat, position);
	UINT closestSpaceshipIndex = GetClosestSpaceship(positionFloat);
	float distance = GetSpaceshipDistance(closestSpaceshipIndex, positionFloat);
	if (distance < 3.0f) {
		m_spaceshipArray.erase(m_spaceshipArray.begin() + closestSpaceshipIndex);
	}
}

void World::BindPlayerPositionToNearestShip(Player* player) {
	XMFLOAT4 positionFloat = player->GetObjectPos();
	UINT closestSpaceshipIndex = GetClosestSpaceship(positionFloat);
	float distance = GetSpaceshipDistance(closestSpaceshipIndex, positionFloat);
	if (distance < 10.0f) {
		player->MountShip(&m_spaceshipArray[closestSpaceshipIndex]);
	}
}

int World::GetBlockCount() {
	return m_blockArray.size();
}
int World::GetPlanetCount() {
	return m_planetArray.size();
}
int World::GetSpaceshipCount() {
	return m_spaceshipArray.size();
}
XMMATRIX World::GetBlockMatrix(int index) {
	return m_blockArray[index].GetObjectMatrix();
}
XMMATRIX World::GetSpaceshipMatrix(int index) {
	return m_spaceshipArray[index].GetObjectMatrix();
}

bool World::CheckIfLoaded() {
	return m_isLoaded;
}
void World::Loaded() {
	m_isLoaded = 1;
}
void World::Unloaded(){
	m_isLoaded = 0;
	m_planetArray.clear();
	m_blockArray.clear();
	m_spaceshipArray.clear();
}