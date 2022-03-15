#pragma once
#include <DirectXMath.h>
#include "PerspectiveCamera.h"
using namespace DirectX;

class ArcCameraModel: public PerspectiveCamera
{
public:
	ArcCameraModel(XMFLOAT3 targetPosition, float distance, float fieldOfView, float aspectRatio, float nearZ, float farZ);
		
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetPerspectiveMatrix();

	void Rotate(float firstAxis, float secondAxis);
	void ChangeDistance(float distanceChange);
	void LookAt(float x, float y, float z);

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 targetPosition;
	void UpdateViewMatrix();
	DirectX::XMVECTOR GetCameraDirection();
};