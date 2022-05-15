#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"

class VSSurfaceBezier
{
public:
	VSSurfaceBezier();

	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;

	void SetVertexBuffer(ID3D11Buffer* vertexBuffer);
};

struct SurfaceBezierIn
{
	DirectX::SimpleMath::Vector4 UV; // SV_POSITION
};