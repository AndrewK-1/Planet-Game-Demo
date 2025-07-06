#pragma once
#include "pch.h"
#include "WorldObject.h"
#include "Planet.h"
#include "Block.h"

class World {
public:
	World();
	World(std::vector<Planet> planetArray);
	World(std::vector<Block> blockArray);
	World(std::vector<Planet> planetArray, std::vector<Block> blockArray);
	void PushObject(Planet planet);
	void PushObject(Block block);
	void RemoveObject(Planet planet, UINT index);
	void RemoveObject(Block block, UINT index);
	float GetPlanetDistance(int index, DirectX::XMFLOAT4 objPosition);
	float GetBlockDistance(int index, DirectX::XMFLOAT4 objPosition);
	UINT GetClosestPlanet(DirectX::XMFLOAT4 objPosition);
	UINT GetClosestBlock(DirectX::XMFLOAT4 objPosition);
	Planet* GetPlanet(UINT index);
	Block* GetBlock(UINT index);
	void AddPlanet(DirectX::XMVECTOR position, float radius);
	void RemovePlanet(DirectX::XMVECTOR position);
	void AddBlock(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	void RemoveBlock(DirectX::XMVECTOR position);
	int GetBlockCount();
	int GetPlanetCount();
	DirectX::XMMATRIX GetBlockMatrix(int index);
private:
	std::vector<Planet> m_planetArray;
	std::vector<Block> m_blockArray;
};