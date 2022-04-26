#pragma once
#include <memory>
#include "dxptr.h"
#include <d3d11.h>
#include "SimpleMath.h"

class PSColor
{
public:
	PSColor();

	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;
};