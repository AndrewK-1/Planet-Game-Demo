#pragma once
#include "pch.h"

class Camera {
public:
	Camera();
	DirectX::XMFLOAT4X4 getCameraFloat4x4();
	DirectX::XMMATRIX getCameraMatrix();
	DirectX::XMFLOAT4 GetOrientation();
	DirectX::XMVECTOR GetOrientationVector();
	DirectX::XMFLOAT4 GetPosition();
	void YawAndPitch(float aYaw, float aPitch);
	void Yaw(float angle);
	void Pitch(float angle);
	void Roll(float angle);
	void Up(float distance);
	void Down(float distance);
	void Left(float distance);
	void Right(float distance);
	void Forward(float distance);
	void Backward(float distance);
	void UpdateMatrix();
	DirectX::XMVECTOR GetForwardRay(float distance);
private:
	DirectX::XMFLOAT4X4 posMatrix;	//Actual rotation matrix of the camera.  For now I will work on the z and 
	DirectX::XMFLOAT4 orientQuaternion;
	DirectX::XMFLOAT4 position;
};