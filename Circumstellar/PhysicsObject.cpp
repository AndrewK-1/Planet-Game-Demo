#pragma once
#include "pch.h"
#include "PhysicsObject.h"

using namespace DirectX;

PhysicsObject::PhysicsObject(float mass, float forcePower, float maxVelocity) : m_mass(mass), m_forcePower(forcePower), m_maxVelocity(maxVelocity), m_activePhysics(0) {}

void PhysicsObject::AccelerateVector(DirectX::XMVECTOR accelerationVec) {
	XMVECTOR momentum = XMLoadFloat4(&m_velocity);
	accelerationVec = XMVectorScale(accelerationVec, 0.0166f);
	XMVectorAdd(momentum, accelerationVec);
	XMStoreFloat4(&m_velocity, momentum);
}
void PhysicsObject::AccelerateForward() {
	XMFLOAT4 objectRot = GetObjectRot();
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(0, 0, m_forcePower / 60.0f, 0), XMLoadFloat4(&objectRot));
	XMStoreFloat4(&m_velocity, XMVectorAdd(XMLoadFloat4(&m_velocity), forwardVec));
}
void PhysicsObject::AccelerateBackward() {
	XMFLOAT4 objectRot = GetObjectRot();
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(0, 0, -m_forcePower / 60.0f, 0), XMLoadFloat4(&objectRot));
	XMStoreFloat4(&m_velocity, XMVectorAdd(XMLoadFloat4(&m_velocity), forwardVec));
}
void PhysicsObject::AccelerateUp() {
	XMFLOAT4 objectRot = GetObjectRot();
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(0, m_forcePower / 60.0f, 0, 0), XMLoadFloat4(&objectRot));
	XMStoreFloat4(&m_velocity, XMVectorAdd(XMLoadFloat4(&m_velocity), forwardVec));
}
void PhysicsObject::AccelerateDown() {
	XMFLOAT4 objectRot = GetObjectRot();
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(0, -m_forcePower / 60.0f, 0, 0), XMLoadFloat4(&objectRot));
	XMStoreFloat4(&m_velocity, XMVectorAdd(XMLoadFloat4(&m_velocity), forwardVec));
}
void PhysicsObject::AccelerateRight() {
	XMFLOAT4 objectRot = GetObjectRot();
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(m_forcePower / 60.0f, 0, 0, 0), XMLoadFloat4(&objectRot));
	XMStoreFloat4(&m_velocity, XMVectorAdd(XMLoadFloat4(&m_velocity), forwardVec));
}
void PhysicsObject::AccelerateLeft() {
	XMFLOAT4 objectRot = GetObjectRot();
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(-m_forcePower / 60.0f, 0, 0, 0), XMLoadFloat4(&objectRot));
	XMStoreFloat4(&m_velocity, XMVectorAdd(XMLoadFloat4(&m_velocity), forwardVec));
}
void PhysicsObject::ApplyPhysics() {
	XMVECTOR velocityVec = XMLoadFloat4(&m_velocity);
	moveUsingVector(velocityVec);
	XMVECTOR newVelocity = XMVectorScale(velocityVec, 0.95f);
	XMStoreFloat4(&m_velocity, newVelocity);
	/*
	XMVECTOR lengthVec = XMVector3Length(velocityVec);
	XMFLOAT4 lengthFloat;
	XMStoreFloat4(&lengthFloat, lengthVec);
	XMVECTOR newVelocity = XMVectorScale(lengthVec, lengthFloat.x - (m_forcePower))
	*/
}



void PhysicsObject::YawAndPitch(float aYaw, float aPitch)
{
	XMFLOAT4 objRot = GetObjectRot();
	//Set temporary vectors to stored postion and rotation values;
	XMVECTOR orientQuatVec = XMLoadFloat4(&objRot);
	//Complete rotations, and store the normalized result in the orientation float4
	XMVECTOR upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuatVec);
	XMVECTOR yawQuat = XMQuaternionRotationAxis(upVec, aYaw);
	orientQuatVec = XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, yawQuat));
	//Do the same for the pitch
	XMVECTOR sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuatVec);
	XMVECTOR pitchQuat = XMQuaternionRotationAxis(sideVec, aPitch);
	XMFLOAT4 returnRot;
	XMStoreFloat4(&returnRot, XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, pitchQuat)));
	SetObjectRot(returnRot);
}

//Rotate the camera by a local up axis horizontally
void PhysicsObject::Yaw(float angle)
{
	XMFLOAT4 objRot = GetObjectRot();
	//Set temporary vectors to stored postion and rotation values
	XMVECTOR orientQuatVec = XMLoadFloat4(&objRot);
	//Worldspace up vector
	XMVECTOR upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuatVec);
	XMVECTOR yawQuat = XMQuaternionRotationAxis(upVec, angle);
	XMFLOAT4 returnRot;
	XMStoreFloat4(&returnRot, XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, yawQuat)));
	SetObjectRot(returnRot);
}

//Rotate the camera by a local side axis vertically
void PhysicsObject::Pitch(float angle)
{
	XMFLOAT4 objRot = GetObjectRot();
	//Set temporary vectors to stored postion and rotation values
	XMVECTOR orientQuatVec = XMLoadFloat4(&objRot);
	//Worldspace side vector
	XMVECTOR sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuatVec);
	XMVECTOR pitchQuat = XMQuaternionRotationAxis(sideVec, angle);
	XMFLOAT4 returnRot;
	XMStoreFloat4(&returnRot, XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, pitchQuat)));
	SetObjectRot(returnRot);
}

void PhysicsObject::Roll(float angle)
{
	XMFLOAT4 objRot = GetObjectRot();
	//Set temporary vectors to stored postion and rotation values
	XMVECTOR orientQuatVec = XMLoadFloat4(&objRot);
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), orientQuatVec);
	XMVECTOR rollQuat = XMQuaternionRotationAxis(forwardVec, angle);
	XMFLOAT4 returnRot;
	XMStoreFloat4(&returnRot, XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, rollQuat)));
	SetObjectRot(returnRot);
}