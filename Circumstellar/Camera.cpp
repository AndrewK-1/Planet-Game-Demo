#pragma once
#include "pch.h"
#include "Camera.h"
#include <cwchar>

using namespace DirectX;

Camera::Camera() :
	posMatrix({ 1.0f, 0.0f, 0.0f, 0.0f, 
				0.0f, 1.0f, 0.0f, 0.0f, 
				0.0f, 0.0f, 1.0f, 0.0f, 
				0.0f, 0.0f, 0.0f, 1.0f }),
	orientQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
	position(0.0f, 0.0f, 0.0f, 1.0f)
{
}

//Return XMMATRIX of the current camera for transformation
XMFLOAT4X4 Camera::getCameraFloat4x4()
{
	return posMatrix;
}

XMMATRIX Camera::getCameraMatrix() 
{
	return XMLoadFloat4x4(&posMatrix);
}

void Camera::YawAndPitch(float aYaw, float aPitch) 
{
	//Set temporary vectors to stored postion and rotation values;
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	//Complete rotations, and store the normalized result in the orientation float4
	XMVECTOR upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuatVec);
	XMVECTOR yawQuat = XMQuaternionRotationAxis(upVec, aYaw);
	orientQuatVec = XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, yawQuat));
	//Do the same for the pitch
	XMVECTOR sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuatVec);
	XMVECTOR pitchQuat = XMQuaternionRotationAxis(sideVec, aPitch);
	XMStoreFloat4(&orientQuaternion, XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, pitchQuat)));

	UpdateMatrix();
}

//Rotate the camera by a local up axis horizontally
void Camera::Yaw(float angle) 
{
	//Set temporary vectors to stored postion and rotation values
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	//Worldspace up vector
	XMVECTOR upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuatVec);
	XMVECTOR yawQuat = XMQuaternionRotationAxis(upVec, angle);
	XMStoreFloat4(&orientQuaternion, XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, yawQuat)));
	UpdateMatrix();
}

//Rotate the camera by a local side axis vertically
void Camera::Pitch(float angle) 
{
	//Set temporary vectors to stored postion and rotation values
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	//Worldspace side vector
	XMVECTOR sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuatVec);
	XMVECTOR pitchQuat = XMQuaternionRotationAxis(sideVec, angle);
	XMStoreFloat4(&orientQuaternion, XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, pitchQuat)));
	UpdateMatrix();
}

void Camera::Roll(float angle) 
{
	//Set temporary vectors to stored postion and rotation values
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), orientQuatVec);
	XMVECTOR rollQuat = XMQuaternionRotationAxis(forwardVec, angle);
	XMStoreFloat4(&orientQuaternion, XMQuaternionNormalize(XMQuaternionMultiply(orientQuatVec, rollQuat)));
	UpdateMatrix();
}

//Move along an axis
void Camera::Up(float distance) 
{
	XMVECTOR positionVec = XMLoadFloat4(&position);
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	XMVECTOR upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuatVec);
	XMVECTOR moveVec = XMVectorScale(upVec, distance);
	XMStoreFloat4(&position, XMVectorAdd(positionVec, moveVec));
	UpdateMatrix();
}

void Camera::Down(float distance)
{
	XMVECTOR positionVec = XMLoadFloat4(&position);
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	XMVECTOR upVec = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuatVec);
	XMVECTOR moveVec = XMVectorScale(upVec, -distance);
	XMStoreFloat4(&position, XMVectorAdd(positionVec, moveVec));
	UpdateMatrix();
}

void Camera::Right(float distance)
{
	XMVECTOR positionVec = XMLoadFloat4(&position);
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	XMVECTOR sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuatVec);
	XMVECTOR moveVec = XMVectorScale(sideVec, distance);
	XMStoreFloat4(&position, XMVectorAdd(positionVec, moveVec));
	UpdateMatrix();
}

void Camera::Left(float distance)
{
	XMVECTOR positionVec = XMLoadFloat4(&position);
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	XMVECTOR sideVec = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), orientQuatVec);
	XMVECTOR moveVec = XMVectorScale(sideVec, -distance);
	XMStoreFloat4(&position, XMVectorAdd(positionVec, moveVec));
	UpdateMatrix();
}

void Camera::Forward(float distance)
{
	XMVECTOR positionVec = XMLoadFloat4(&position);
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), orientQuatVec);
	XMVECTOR moveVec = XMVectorScale(forwardVec, distance);
	XMStoreFloat4(&position, XMVectorAdd(positionVec, moveVec));
	UpdateMatrix();
}

void Camera::Backward(float distance)
{
	XMVECTOR positionVec = XMLoadFloat4(&position);
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	XMVECTOR forwardVec = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), orientQuatVec);
	XMVECTOR moveVec = XMVectorScale(forwardVec, -distance);
	XMStoreFloat4(&position, XMVectorAdd(positionVec, moveVec));
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	XMVECTOR positionVec = XMLoadFloat4(&position);
	XMVECTOR orientQuatVec = XMLoadFloat4(&orientQuaternion);
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), orientQuatVec);
	XMVECTOR target = XMVectorAdd(positionVec, forward);
	XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), orientQuatVec);
	//Create matrix and store as a float4x4
	XMStoreFloat4x4(&posMatrix, XMMatrixLookAtLH(positionVec, target, up));
}