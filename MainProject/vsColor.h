#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"

class VSColor
{
public:
	VSColor();

	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;
	mini::dx_ptr<ID3D11Buffer> constantBufferVS;

	void SetVertexBuffer(ID3D11Buffer* vertexBuffer);

	void SetConstantBuffer(DirectX::SimpleMath::Matrix mvp);
};

struct VSColorIn
{
	DirectX::SimpleMath::Vector3 pos; // : POSITION;
	DirectX::SimpleMath::Vector3 col; // : COLOR;
};