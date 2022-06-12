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
	DirectX::SimpleMath::Vector3 pos; // : POSITION;
};


__declspec(align(16))
struct VSGregoryConstantBuffer
{
	DirectX::SimpleMath::Matrix mvp;

	std::array<DirectX::SimpleMath::Vector3 ,4> p;
	std::array<DirectX::SimpleMath::Vector3 ,4> dU;
	std::array<DirectX::SimpleMath::Vector3 ,4> dV;
	std::array<DirectX::SimpleMath::Vector3 ,4> dUV;
	std::array<DirectX::SimpleMath::Vector3 ,4> dVU;

};

class VSGregory
{
public:
	VSGregory();

	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;
	mini::dx_ptr<ID3D11Buffer> constantBufferVS;

	void SetVertexBuffer(ID3D11Buffer* vertexBuffer);

	void SetConstantBuffer(VSGregoryConstantBuffer buffer);
};
