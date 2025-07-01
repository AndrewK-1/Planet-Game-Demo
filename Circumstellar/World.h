#pragma once
#include "pch.h"
#include "WorldObject.h"
class World {
public:
	World();
private:
	std::vector<std::vector<WorldObject>> m_worldObjectArray;
};