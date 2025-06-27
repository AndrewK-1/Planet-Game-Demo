#pragma once
#include "pch.h"
#include "WorldRenderedObject.h"
#include "TerrainVoxel.h"

class SimpleVoxelObject : public WorldRenderedObject{
public:
	SimpleVoxelObject(int objectShape);
	virtual void generateVoxelGrid(int objectShape) = 0;
	enum ObjectShape {
		Sphere = 0,
		Cube = 1
	};
private:
	std::vector<std::vector<std::vector<TerrainVoxel>>> voxelGrid;
	
};