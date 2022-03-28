#pragma once
#include <DirectXMath.h>
#include "PerspectiveCamera.h"
using namespace DirectX;

class ArcCameraModel: public PerspectiveCamera
{
public:
	ArcCameraModel(XMFLOAT3 targetPosition, float distance, float fieldOfView, float aspectRatio, float nearZ, float farZ);
		
	DirectX::SimpleMath::Matrix GetViewMatrix();
	DirectX::SimpleMath::Matrix GetPerspectiveMatrix();

	void Rotate(float firstAxis, float secondAxis);
	void ChangeDistance(float distanceChange);
	void LookAt(float x, float y, float z);

private:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 targetPosition;
	void UpdateViewMatrix();
	DirectX::SimpleMath::Vector3 GetCameraDirection();
};