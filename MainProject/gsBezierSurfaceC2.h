#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"

__declspec(align(16))
struct GSBezierSurfaceC2ConstantBuffer
{
	DirectX::SimpleMath::Matrix mvp;
	float X[16];
	float Y[16];
	float Z[16];
	int slices;
	int flipped;
	int rowIndex;
	int columnIndex;
	int rowCount;
	int columnCount;
	int filter;
};

class GSBezierSurfaceC2
{
public:
	GSBezierSurfaceC2();

	mini::dx_ptr<ID3D11GeometryShader> m_geometryShader;
	mini::dx_ptr<ID3D11Buffer> constantBufferGS;
	void SetConstantBuffer(GSBezierSurfaceC2ConstantBuffer buffer);
	void SetFilterTexture(ID3D11ShaderResourceView* texture);
	mini::dx_ptr<ID3D11SamplerState> m_samplerWrap;
};
