#pragma once
#include "pch.h"
#include "Spaceship.h"

using namespace DirectX;

Spaceship::Spaceship(float mass, float forcePower, float maxVelocity) : PhysicsObject(mass, forcePower, maxVelocity) {}
Spaceship::Spaceship(float mass, float forcePower, float maxVelocity, XMVECTOR position, XMVECTOR rotation, XMVECTOR scale) : PhysicsObject(mass, forcePower, maxVelocity, position, rotation, scale) {}