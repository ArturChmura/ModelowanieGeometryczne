#pragma once
#include <memory>
#include "dxDevice.h"
#include "Camera.h"

class IModelDrawer
{
public:
	virtual void Draw(std::shared_ptr<DxDevice> m_device, std::shared_ptr<Camera> camera) = 0;

	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;
	mini::dx_ptr<ID3D11Buffer> constantBuffer;
};