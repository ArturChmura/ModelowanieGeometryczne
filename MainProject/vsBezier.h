#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"

class VSBezier
{
public:
	VSBezier();

	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;

	void SetVertexBuffer(ID3D11Buffer* vertexBuffer);
};

struct VSBezierIn
{
    DirectX::SimpleMath::Vector4 X ; // SV_POSITION;   
    DirectX::SimpleMath::Vector4 Y ; // Y
    DirectX::SimpleMath::Vector4 Z ; // Z
	int SIZE; // Z
};