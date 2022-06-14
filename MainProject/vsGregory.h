#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"
#include <array>

struct VSGregoryIn
{
	DirectX::SimpleMath::Vector4 pos; // : POSITION;
};


class VSGregory
{
public:
	VSGregory();

	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;

	void SetVertexBuffer(ID3D11Buffer* vertexBuffer);
};
