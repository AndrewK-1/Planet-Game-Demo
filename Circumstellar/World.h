#pragma once
#include "pch.h"
#include "WorldRenderedObject.h"
class World {
public:
	World();
private:
	std::vector<WorldRenderedObject> m_worldObjects;
	std::vector<WorldRenderedObject> m_visibleWorldObjects;
};