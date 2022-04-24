#pragma once
#include <memory>
#include "dxptr.h"
#include <d3d11.h>
#include "SimpleMath.h"

class PSConstColor
{
public:
	PSConstColor();

	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;
	mini::dx_ptr<ID3D11Buffer> constantBufferPS;
	void SetConstantBuffer(DirectX::SimpleMath::Vector3 color);
};