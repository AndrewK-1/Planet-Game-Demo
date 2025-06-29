#pragma once
#include "pch.h"
#include "WorldObject.h"
#include "GraphicsObject.h"

class WorldRenderedObject : public WorldObject, public GraphicsObject {
public:
	WorldRenderedObject();
	WorldRenderedObject(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale);
};