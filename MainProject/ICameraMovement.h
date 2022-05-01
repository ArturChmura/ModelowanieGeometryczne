#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include "SimpleMath.h"

class ICameraMovement
{
public:
	virtual DirectX::SimpleMath::Matrix GetViewMatrix() = 0;
	virtual void DrawGUI() = 0;
};