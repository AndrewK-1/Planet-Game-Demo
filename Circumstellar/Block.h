#pragma once
#include "pch.h"
#include "WorldObject.h"

class Block : public WorldObject {
public:
	Block() : WorldObject() {}
	Block(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale) : WorldObject(position, rotation, scale) {}
};