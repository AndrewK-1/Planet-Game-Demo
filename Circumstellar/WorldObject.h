#pragma once
#include "pch.h"

class WorldObject {
public:
	WorldObject();
	WorldObject(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale);
	DirectX::XMFLOAT4 getObjectPos();
	DirectX::XMFLOAT4 getObjectRot();
	DirectX::XMMATRIX getObjectMatrix();
	void rotateAboutWorldAxisX(float angle);
	void rotateAboutWorldAxisY(float angle);
	void rotateAboutWorldAxisZ(float angle);
	void rotateAboutWorldVector(DirectX::XMVECTOR rotationVector, float angle);
	void rotateAboutLocalAxisX(float angle);
	void rotateAboutLocalAxisY(float angle);
	void rotateAboutLocalAxisZ(float angle);
	void rotateYaw(float angle);
	void rotatePitch(float angle);
	void rotateRoll(float angle);
	void rotateAboutLocalVector(DirectX::XMVECTOR rotationVector, float angle);
	void moveAlongAxisX(float distance);
	void moveAlongAxisY(float distance);
	void moveAlongAxisZ(float distance);
	void moveAlongVector(DirectX::XMVECTOR translationDirection, float distance);
	void moveUsingVector(DirectX::XMVECTOR translationVector);
private:
	DirectX::XMFLOAT4 objectPos;
	DirectX::XMFLOAT4 objectRot;
	DirectX::XMFLOAT4 objectScale;
};