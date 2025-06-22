#pragma once
#include "pch.h"
#include "Camera.h"

using namespace DirectX;

Camera::Camera() {
	upVec = XMVectorSet(0, 1, 0, 0);
	zYawAngle = 0;
	yPitchAngle = 0;
}

XMMATRIX Camera::getMatrixData() {
	return posMatrix;
}

void Camera::rotateRight(float angle) {
	XMMATRIX right = XMMatrixRotationY(angle);
	posMatrix = XMMatrixMultiply(right, posMatrix);
	XMVectorSet(XMVectorGetX(posMatrix.r[0]), XMVectorGetX(posMatrix.r[1]), XMVectorGetX(posMatrix.r[2]), 1);
}

void Camera::rotateLeft(float angle) {

}

void Camera::rotateUp(float angle) {
	XMMATRIX up = XMMatrixRotationZ(angle);

}

void Camera::rotateDown(float angle) {

}