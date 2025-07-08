#pragma once
#include "pch.h"
#include "PhysicsObject.h"

class Player : public PhysicsObject {
public:
	Player(float mass, float forcePower, float maxVelocity);
};