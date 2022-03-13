#pragma once
#include <DirectXMath.h>


class ArcCameraModel
{
public:
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 perspectiveMatrix;
	void Rotate(float upAxis, float secondAxis);
	void ChangeDistance(float distanceChange);
	void LookAt(float x, float y, float z);
	void SetPerspective(float fieldOfView, float aspectRatioYperX, float nearZ, float farZ);

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 targerPostion;
	DirectX::XMFLOAT3 upVector = {0,1,0};
	void UpdateViewMatrix();
	DirectX::XMVECTOR GetCameraDirection();
};