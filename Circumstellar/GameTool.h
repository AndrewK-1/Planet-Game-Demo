#pragma once
#include "pch.h"
#include "Planet.h"
#include "Camera.h"

class GameTool {
public:
	GameTool();
	GameTool(float terrainChangeRadius);
	bool ChangeTerrain(Planet* planet, Camera* currentCam, float changePower);
	int GetCurrentTool();
	void SetCurrentTool(int tool);
private:
	float m_terrainChangeRadius;
	enum Toolsets {
		terrain=0,
		block=1,
		ship=2
	};
	int m_currentTool = 0;
};