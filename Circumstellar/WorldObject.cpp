#pragma once
#include "pch.h"
#include "WorldObject.h"

using namespace DirectX;

WorldObject::WorldObject() : objectScale(1.0f, 1.0f, 1.0f, 1.0f), objectPos(0.0f, 0.0f, 0.0f, 1.0f), objectRot(0.0f, 0.0f, 0.0f, 1.0f)
{

}

DirectX::XMFLOAT4 WorldObject::getObjectPos() {
	return objectPos;
}
DirectX::XMFLOAT4 WorldObject::getObjectRot() {
	return objectRot;
}
DirectX::XMMATRIX WorldObject::getObjectMatrix() {
	return XMMatrixAffineTransformation(XMLoadFloat4(&objectScale), XMQuaternionIdentity(), XMLoadFloat4(&objectRot), XMLoadFloat4(&objectPos));
}
//Rotate object about the current world origin
void WorldObject::rotateAboutWorldAxisX(float angle) {
	XMMATRIX matrix;
	matrix = XMMatrixMultiply(XMMatrixRotationX(angle), getObjectMatrix());
	XMVECTOR tScale, tPos, tRot;
	XMMatrixDecompose(&tScale, &tPos, &tRot, matrix);
	XMStoreFloat4(&objectScale, tScale);
	XMStoreFloat4(&objectPos, tPos);
	XMStoreFloat4(&objectRot, tRot);
}
void WorldObject::rotateAboutWorldAxisY(float angle) {
	XMMATRIX matrix;
	matrix = XMMatrixMultiply(XMMatrixRotationY(angle), getObjectMatrix());
	XMVECTOR tScale, tPos, tRot;
	XMMatrixDecompose(&tScale, &tPos, &tRot, matrix);
	XMStoreFloat4(&objectScale, tScale);
	XMStoreFloat4(&objectPos, tPos);
	XMStoreFloat4(&objectRot, tRot);
}
void WorldObject::rotateAboutWorldAxisZ(float angle) {
	XMMATRIX matrix;
	matrix = XMMatrixMultiply(XMMatrixRotationZ(angle), getObjectMatrix());
	XMVECTOR tScale, tPos, tRot;
	XMMatrixDecompose(&tScale, &tPos, &tRot, matrix);
	XMStoreFloat4(&objectScale, tScale);
	XMStoreFloat4(&objectPos, tPos);
	XMStoreFloat4(&objectRot, tRot);
}
void WorldObject::rotateAboutWorldVector(DirectX::XMVECTOR rotationVector, float angle) {
	XMMATRIX matrix, rotation;
	matrix = getObjectMatrix();
	rotation = XMMatrixRotationAxis(rotationVector, angle);
	matrix = XMMatrixMultiply(rotation, matrix);
	XMVECTOR tScale, tPos, tRot;
	XMMatrixDecompose(&tScale, &tPos, &tRot, matrix);
	XMStoreFloat4(&objectScale, tScale);
	XMStoreFloat4(&objectPos, tPos);
	XMStoreFloat4(&objectRot, tRot);
}
//Rotate object about its local origin's axes
void WorldObject::rotateAboutLocalAxisX(float angle) {
	XMVECTOR localRotQuat;
	localRotQuat = XMQuaternionNormalize(XMQuaternionMultiply(XMLoadFloat4(&objectRot), XMQuaternionRotationRollPitchYaw(0, angle, 0)));
	XMStoreFloat4(&objectRot, localRotQuat);
}
void WorldObject::rotateAboutLocalAxisY(float angle) {
	XMVECTOR localRotQuat;
	localRotQuat = XMQuaternionNormalize(XMQuaternionMultiply(XMLoadFloat4(&objectRot), XMQuaternionRotationRollPitchYaw(angle, 0, 0)));
	XMStoreFloat4(&objectRot, localRotQuat);
}
void WorldObject::rotateAboutLocalAxisZ(float angle) {
	XMVECTOR localRotQuat;
	localRotQuat = XMQuaternionNormalize(XMQuaternionMultiply(XMLoadFloat4(&objectRot), XMQuaternionRotationRollPitchYaw(0, 0, angle)));
	XMStoreFloat4(&objectRot, localRotQuat);
}
//Rotate object about its local origin using a vector as an axis
void WorldObject::rotateAboutLocalVector(DirectX::XMVECTOR rotationVector, float angle) {
	XMVECTOR localRotQuat;
	localRotQuat = XMQuaternionNormalize(XMQuaternionMultiply(XMLoadFloat4(&objectRot), XMQuaternionRotationAxis(rotationVector, angle)));
	XMStoreFloat4(&objectRot, localRotQuat);
}
//Rotate an object based on its local forward, up, and right axes
void WorldObject::rotateYaw(float angle) {
	XMVECTOR localRotQuat, rotation;
	localRotQuat = XMLoadFloat4(&objectRot);
	rotation = XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), localRotQuat);
	rotation = XMQuaternionRotationAxis(rotation, angle);
	XMStoreFloat4(&objectRot, XMQuaternionNormalize(XMQuaternionMultiply(localRotQuat, rotation)));

}
void WorldObject::rotatePitch(float angle) {
	XMVECTOR localRotQuat, rotation;
	localRotQuat = XMLoadFloat4(&objectRot);
	rotation = XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), localRotQuat);
	rotation = XMQuaternionRotationAxis(rotation, angle);
	XMStoreFloat4(&objectRot, XMQuaternionNormalize(XMQuaternionMultiply(localRotQuat, rotation)));
}
void WorldObject::rotateRoll(float angle) {
	XMVECTOR localRotQuat, rotation;
	localRotQuat = XMLoadFloat4(&objectRot);
	rotation = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), localRotQuat);
	rotation = XMQuaternionRotationAxis(rotation, angle);
	XMStoreFloat4(&objectRot, XMQuaternionNormalize(XMQuaternionMultiply(localRotQuat, rotation)));
}

void WorldObject::moveAlongAxisX(float distance){
	XMStoreFloat4(&objectPos, XMVectorAdd(XMLoadFloat4(&objectPos), XMVectorSet(distance, 0.0f, 0.0f, 0.0f)));
}
void WorldObject::moveAlongAxisY(float distance) {
	XMStoreFloat4(&objectPos, XMVectorAdd(XMLoadFloat4(&objectPos), XMVectorSet(0.0f, distance, 0.0f, 0.0f)));
}
void WorldObject::moveAlongAxisZ(float distance) {
	XMStoreFloat4(&objectPos, XMVectorAdd(XMLoadFloat4(&objectPos), XMVectorSet(0.0f, 0.0f, distance, 0.0f)));
}
//Move the object in the direction of the vector, with the specified distance
void WorldObject::moveAlongVector(DirectX::XMVECTOR translationDirection, float distance) {
	//Normalize the vector and scale it by distance desired.
	//Add the object's current position to the scaled vector
	//Store the result into the current position
	XMStoreFloat4(&objectPos, XMVectorAdd(
		XMLoadFloat4(&objectPos), XMVectorScale(
			XMQuaternionNormalize(translationDirection), distance)));
}
//Add the value of the vector to the current position
void WorldObject::moveUsingVector(DirectX::XMVECTOR translationVector) {
	XMStoreFloat4(&objectPos, XMVectorAdd(XMLoadFloat4(&objectPos), translationVector));
}