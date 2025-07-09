#pragma once
#include "pch.h"
#include "WorldObject.h"

class PhysicsObject : public WorldObject {
public:
	PhysicsObject(float mass, float forcePower, float maxVelocity);
	PhysicsObject(float mass, float forcePower, float maxVelocity, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale);
	void AccelerateVector(DirectX::XMVECTOR accelerationVec);
	void AccelerateForward();
	void AccelerateBackward();
	void AccelerateUp();
	void AccelerateDown();
	void AccelerateRight();
	void AccelerateLeft();
	void ApplyPhysics();	//Apply acceleration to velocity, and velocity to position

	void YawAndPitch(float aYaw, float aPitch);
	void Yaw(float angle);
	void Pitch(float angle);
	void Roll(float angle);

	void SetForcePower(float power);
private:
	DirectX::XMFLOAT4 m_velocity;
	bool m_activePhysics;	//Whether the object needs to have its physics update called
	float m_mass;	//Mass of the object
	float m_forcePower;	//Force in newtons the object can apply
	float m_maxVelocity;	//Maximum speed that acceleration will not add to.  Terminal velocity.
};