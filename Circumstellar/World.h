#pragma once
#include "pch.h"
#include "WorldObject.h"
class World {
public:
	World();
	World(std::vector<WorldObject> objectArray);
	void pushObject(WorldObject obj);
	void removeObject(WorldObject obj);
private:
	std::vector<WorldObject> m_worldObjectArray;
};