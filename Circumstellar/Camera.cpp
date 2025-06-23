#pragma once
#include "pch.h"
#include "Camera.h"
#include <cwchar>

using namespace DirectX;

Camera::Camera() :
	posMatrix(XMMatrixIdentity()),
	orientQuaternion(XMQuaternionIdentity()),
	position(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
{
}

//Return XMMATRIX of the current camera for transformation
XMMATRIX Camera::getCameraMatrix() 
{
	return XMMatrixInverse(nullptr, posMatrix);
}

void Camera::YawAndPitch(float aYaw, float aPitch) {
	upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuaternion);
	XMVECTOR yawQuat = XMQuaternionRotationAxis(upVec, aYaw);
	orientQuaternion = XMQuaternionNormalize(XMQuaternionMultiply(orientQuaternion, yawQuat));

	sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuaternion);
	XMVECTOR pitchQuat = XMQuaternionRotationAxis(sideVec, aPitch);
	orientQuaternion = XMQuaternionNormalize(XMQuaternionMultiply(orientQuaternion, pitchQuat));

	UpdateMatrix();
}

//Rotate the camera by a local up axis horizontally
void Camera::Yaw(float angle) 
{
	//Worldspace up vector
	upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuaternion);
	XMVECTOR yawQuat = XMQuaternionRotationAxis(upVec, angle);
	orientQuaternion = XMQuaternionNormalize(XMQuaternionMultiply(orientQuaternion, yawQuat));
	UpdateMatrix();
}

//Rotate the camera by a local side axis vertically
void Camera::Pitch(float angle) 
{
	//Worldspace side vector
	sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuaternion);
	XMVECTOR pitchQuat = XMQuaternionRotationAxis(sideVec, angle);
	orientQuaternion = XMQuaternionNormalize(XMQuaternionMultiply(orientQuaternion, pitchQuat));
	UpdateMatrix();
}

void Camera::Roll(float angle) {
	forwardVec = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), orientQuaternion);
	XMVECTOR rollQuat = XMQuaternionRotationAxis(forwardVec, angle);
	orientQuaternion = XMQuaternionNormalize(XMQuaternionMultiply(orientQuaternion, rollQuat));
	UpdateMatrix();
}

//Move along an axis
void Camera::Up(float distance) 
{
	upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuaternion);
	XMVECTOR moveVec = XMVectorScale(upVec, distance);
	position = XMVectorAdd(position, moveVec);
	UpdateMatrix();
}

void Camera::Down(float distance)
{
	upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuaternion);
	XMVECTOR moveVec = XMVectorScale(upVec, -distance);
	position = XMVectorAdd(position, moveVec);
	UpdateMatrix();
}

void Camera::Right(float distance)
{
	sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuaternion);
	XMVECTOR moveVec = XMVectorScale(sideVec, distance);
	position = XMVectorAdd(position, moveVec);
	UpdateMatrix();
}

void Camera::Left(float distance)
{
	sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuaternion);
	XMVECTOR moveVec = XMVectorScale(sideVec, -distance);
	position = XMVectorAdd(position, moveVec);
	UpdateMatrix();
}

void Camera::Forward(float distance)
{
	forwardVec = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), orientQuaternion);
	XMVECTOR moveVec = XMVectorScale(forwardVec, distance);
	position = XMVectorAdd(position, moveVec);
	UpdateMatrix();
}

void Camera::Backward(float distance)
{
	forwardVec = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), orientQuaternion);
	XMVECTOR moveVec = XMVectorScale(forwardVec, -distance);
	position = XMVectorAdd(position, moveVec);
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), orientQuaternion);
	XMVECTOR target = XMVectorAdd(position, forward);
	XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuaternion);

	posMatrix = XMMatrixLookAtLH(position, target, up);
}