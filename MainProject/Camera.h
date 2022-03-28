#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "SimpleMath.h"

// Interface
class Camera
{
public:
	virtual DirectX::SimpleMath::Matrix GetViewMatrix() = 0;
	virtual DirectX::SimpleMath::Matrix GetPerspectiveMatrix() = 0;
protected:
	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix perspectiveMatrix;
	DirectX::SimpleMath::Vector3 upVector = { 0,1,0 };
};