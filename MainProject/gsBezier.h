#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"

struct GSBezierConstantBuffer
{
	DirectX::SimpleMath::Matrix mvp;
	int screenWidth;
	int screenHeight;
};
class GSBezier
{
public:
	GSBezier();

	mini::dx_ptr<ID3D11GeometryShader> m_geometryShader;
	mini::dx_ptr<ID3D11Buffer> constantBufferGS;
	void SetConstantBuffer(GSBezierConstantBuffer buffer);
};
