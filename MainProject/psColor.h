#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>

class PSColor
{
public:
	PSColor();

	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;
};