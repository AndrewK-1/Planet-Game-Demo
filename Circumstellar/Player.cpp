#pragma once
#include "pch.h"
#include "Player.h"

Player::Player(float mass, float forcePower, float maxVelocity) : PhysicsObject(mass, forcePower, maxVelocity) {}