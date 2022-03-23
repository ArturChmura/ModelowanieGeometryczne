#pragma once
#include "ShaderInfo.h"
#include "dxDevice.h"
#include "Camera.h"

class GlobalAxisDrawer
{
public:
	GlobalAxisDrawer();
	void Draw(std::shared_ptr<Camera> camera);
	ShaderInfo shaderInfo;
	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
};