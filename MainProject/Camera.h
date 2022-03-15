#pragma once
#include <DirectXMath.h>

// Interface
class Camera
{
public:
	virtual DirectX::XMFLOAT4X4 GetViewMatrix() = 0;
	virtual DirectX::XMFLOAT4X4 GetPerspectiveMatrix() = 0;
protected:
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 perspectiveMatrix;
	DirectX::XMFLOAT3 upVector = { 0,1,0 };
};