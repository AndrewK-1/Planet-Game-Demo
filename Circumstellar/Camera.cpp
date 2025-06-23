#pragma once
#include "pch.h"
#include "Camera.h"

using namespace DirectX;

Camera::Camera() :
	posMatrix(XMMatrixIdentity())
{
}

//Return XMMATRIX of the current camera for transformation
XMMATRIX Camera::getCameraMatrix() 
{
	return XMMatrixInverse(nullptr, posMatrix);
}

//Rotate the camera by a local up axis horizontally
void Camera::Yaw(float angle) 
{
	upVec = XMVectorSet(XMVectorGetY(posMatrix.r[0]), XMVectorGetY(posMatrix.r[1]), XMVectorGetY(posMatrix.r[2]), 1);
	XMMATRIX horizontalRotateMatrix = XMMatrixRotationAxis(upVec, -angle);
	posMatrix = XMMatrixMultiply(horizontalRotateMatrix, posMatrix);
}

//Rotate the camera by a local side axis vertically
void Camera::Pitch(float angle) 
{
	sideVec = XMVectorSet(XMVectorGetX(posMatrix.r[0]), XMVectorGetX(posMatrix.r[1]), XMVectorGetX(posMatrix.r[2]), 1);
	XMMATRIX vericalRotateMatrix = XMMatrixRotationAxis(sideVec, -angle);
	posMatrix = XMMatrixMultiply(vericalRotateMatrix, posMatrix);
}

//Move along an axis
void Camera::Up(float distance) 
{
	upVec = XMVectorSet(XMVectorGetY(posMatrix.r[0]), XMVectorGetY(posMatrix.r[1]), XMVectorGetY(posMatrix.r[2]), 1);
	upVec = XMVectorScale(upVec, distance);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(upVec);
	posMatrix = XMMatrixMultiply(translationMatrix, posMatrix);
	OutputDebugString(L" Up ");
}

void Camera::Down(float distance)
{
	distance *= -1;
	upVec = XMVectorSet(XMVectorGetY(posMatrix.r[0]), XMVectorGetY(posMatrix.r[1]), XMVectorGetY(posMatrix.r[2]), 1);
	upVec = XMVectorScale(upVec, distance);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(upVec);
	posMatrix = XMMatrixMultiply(translationMatrix, posMatrix);
	OutputDebugString(L" Down ");
}

void Camera::Right(float distance)
{
	sideVec = XMVectorSet(XMVectorGetX(posMatrix.r[0]), XMVectorGetX(posMatrix.r[1]), XMVectorGetX(posMatrix.r[2]), 1);
	sideVec = XMVectorScale(sideVec, distance);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(sideVec);
	posMatrix = XMMatrixMultiply(translationMatrix, posMatrix);
	OutputDebugString(L" Right ");
}

void Camera::Left(float distance)
{
	distance *= -1;
	sideVec = XMVectorSet(XMVectorGetX(posMatrix.r[0]), XMVectorGetX(posMatrix.r[1]), XMVectorGetX(posMatrix.r[2]), 1);
	sideVec = XMVectorScale(sideVec, distance);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(sideVec);
	posMatrix = XMMatrixMultiply(translationMatrix, posMatrix);
	OutputDebugString(L" Left ");
}

void Camera::Forward(float distance)
{
	sideVec = XMVectorSet(XMVectorGetZ(posMatrix.r[0]), XMVectorGetZ(posMatrix.r[1]), XMVectorGetZ(posMatrix.r[2]), 1);
	sideVec = XMVectorScale(sideVec, distance);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(sideVec);
	posMatrix = XMMatrixMultiply(translationMatrix, posMatrix);
	OutputDebugString(L" Forward ");
}

void Camera::Backward(float distance)
{
	distance *= -1;
	sideVec = XMVectorSet(XMVectorGetZ(posMatrix.r[0]), XMVectorGetZ(posMatrix.r[1]), XMVectorGetZ(posMatrix.r[2]), 1);
	sideVec = XMVectorScale(sideVec, distance);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(sideVec);
	posMatrix = XMMatrixMultiply(translationMatrix, posMatrix);
	OutputDebugString(L" Backward ");
}