#pragma once
#include "pch.h"
#include "PhysicsObject.h"
#include "Spaceship.h"

class Player : public PhysicsObject {
public:
	Player(float mass, float forcePower, float maxVelocity);
	void MountShip(Spaceship* ship);
	bool IsMounted();
	void Unmount();
	Spaceship* GetMounted();
private:
	bool m_mounted;
	Spaceship* m_mountedShip;
};