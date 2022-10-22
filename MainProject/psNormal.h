#pragma once
#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>

class PSNormal
{
public:
	PSNormal();

	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;
};