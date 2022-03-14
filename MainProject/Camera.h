#pragma once
#include <DirectXMath.h>

// Interface
class Camera
{
public:
	virtual DirectX::XMFLOAT4X4 GetViewMatrix() = 0;
	virtual DirectX::XMFLOAT4X4 GetPerspectiveMatrix() = 0;
};