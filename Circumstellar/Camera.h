#pragma once
#include "pch.h"

class Camera {
public:
	Camera();
	DirectX::XMMATRIX getCameraMatrix();
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
private:
	DirectX::XMMATRIX posMatrix;	//Actual rotation matrix of the camera.  For now I will work on the z and 
	DirectX::XMVECTOR sideVec;
	DirectX::XMVECTOR upVec;
	DirectX::XMVECTOR forwardVec;
	float	zYawAngle;
	float	yPitchAngle;
	DirectX::XMVECTOR orientQuaternion;
	DirectX::XMVECTOR position;
};