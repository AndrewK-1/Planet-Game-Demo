#pragma once
#include "pch.h"
#include "Player.h"

using namespace DirectX;

Player::Player(float mass, float forcePower, float maxVelocity) : PhysicsObject(mass, forcePower, maxVelocity), m_mounted(0), m_mountedShip(nullptr) {}

//Mounts the player to a ship
void Player::MountShip(Spaceship* ship) {
	m_mountedShip = ship;
	m_mounted = 1;
}
//Returns whether a player is currently mounted in a ship or not
bool Player::IsMounted() {
	return m_mounted;
}

void Player::Unmount() {
	m_mounted = 0;
	m_mountedShip = nullptr;
}

Spaceship* Player::GetMounted() {
	return m_mountedShip;
}