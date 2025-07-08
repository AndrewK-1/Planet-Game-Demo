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
	OutputDebugString(L"Accelerating Forward.\n");
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