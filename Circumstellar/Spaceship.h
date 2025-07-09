#pragma once
#include "pch.h"
#include "PhysicsObject.h"

class Spaceship : public PhysicsObject {
public:
	Spaceship(float mass, float forcePower, float maxVelocity);
	Spaceship(float mass, float forcePower, float maxVelocity, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale);
};