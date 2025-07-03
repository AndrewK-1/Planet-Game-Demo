#pragma once
#include "pch.h"
#include "World.h"
#include <random>

using namespace DirectX;

World::World() : m_worldObjectArray() {}
World::World(std::vector<WorldObject> objectArray) : m_worldObjectArray(objectArray){}

void World::pushObject(WorldObject obj) {
	m_worldObjectArray.push_back(obj);
}