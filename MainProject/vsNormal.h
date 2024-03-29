#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"

__declspec(align(16))
struct  VSNormalConstantBuffer
{
	DirectX::SimpleMath::Matrix modelMatrix;
	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix inversedViewMatrix;
	DirectX::SimpleMath::Matrix perspectiveMatrix;
	int gridWidthCount;
	int gridLengthCount;

	float widthSize;
	float lengthSize;
};

class VSNormal
{
public:
	VSNormal();

	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;
	mini::dx_ptr<ID3D11Buffer> constantBufferVS;

	void SetVertexBuffer(ID3D11Buffer* vertexBuffer);

	void SetTexture(ID3D11ShaderResourceView* resourceView);

	void SetConstantBuffer(VSNormalConstantBuffer buffer);
private:
	mini::dx_ptr<ID3D11SamplerState> m_samplerWrap;
};

struct VSNormalIn
{
	DirectX::SimpleMath::Vector3 pos; // : POSITION;
	DirectX::SimpleMath::Vector3 normal; // : NORMAL;
};