#pragma once
#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <SimpleMath.h>

__declspec(align(16))
struct  PSNormalConstantBuffer
{
	int gridWidthCount;
	int gridLengthCount;

	float widthSize;
	float lengthSize;
};

class PSNormal
{
public:
	PSNormal();

	void SetTexture(ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* colorTexture);

	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;

	void SetConstantBuffer(PSNormalConstantBuffer buffer);
private:
	mini::dx_ptr<ID3D11SamplerState> m_samplerWrap;
	mini::dx_ptr<ID3D11Buffer> constantBufferPS;
};