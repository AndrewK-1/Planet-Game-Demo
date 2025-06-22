#pragma once
#include "pch.h"

class Camera {
public:
	Camera();
	DirectX::XMMATRIX getMatrixData();
	void rotateRight(float angle);
	void rotateLeft(float angle);
	void rotateUp(float angle);
	void rotateDown(float angle);
private:
	DirectX::XMMATRIX posMatrix;
	DirectX::XMVECTOR upVec;
	float	zYawAngle;
	float	yPitchAngle;
};