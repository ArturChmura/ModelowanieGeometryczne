#pragma once
#include <memory>
#include "dxptr.h"
#include <d3d11.h>

class ShaderInfo
{
public:
	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;
	mini::dx_ptr<ID3D11Buffer> constantBuffer;
};