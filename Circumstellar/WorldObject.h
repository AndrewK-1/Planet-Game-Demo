#pragma once
#include "pch.h"

class WorldObject {
public:
	WorldObject();
	WorldObject(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale);
	WorldObject(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale, int objType);
	DirectX::XMFLOAT4 GetObjectPos() const;
	DirectX::XMFLOAT4 GetObjectRot() const;
	DirectX::XMMATRIX GetObjectMatrix() const;
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
	DirectX::XMFLOAT4 m_objectPos;
	DirectX::XMFLOAT4 m_objectRot;
	DirectX::XMFLOAT4 m_objectScale;
	int m_objectType;
};