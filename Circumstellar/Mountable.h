#pragma once
#include "pch.h"
#include "WorldObject.h"

class Mountable {
private:
	std::vector<WorldObject*> m_mounted;
};