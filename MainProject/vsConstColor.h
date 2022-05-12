#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"

class VSConstColor
{
public:
	VSConstColor();

	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;
	mini::dx_ptr<ID3D11Buffer> constantBufferVS;

	void SetVertexBuffer(ID3D11Buffer* vertexBuffer);

	void SetConstantBuffer(DirectX::SimpleMath::Matrix mvp);
};


struct VSConstColorIn
{
	DirectX::SimpleMath::Vector3 pos; // : POSITION
};