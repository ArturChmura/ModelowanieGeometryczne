#pragma once
#include "ShaderInfo.h"
#include "MVPColorConstantBuffer.h"

class ShaderInfoPolynomial : public ShaderInfo
{
public:
	ShaderInfoPolynomial();
	// Inherited via ShaderInfo
	virtual void SetUpRender() override;
	virtual void SetVertexBuffer(ID3D11Buffer* vertexBuffer) override;
	virtual void CopyConstantBuffers() override;

	MVPColorConstantBuffer constantBufferStruct;
	mini::dx_ptr<ID3D11GeometryShader> m_geometryShader;
	mini::dx_ptr<ID3D11Buffer> constantBufferGS;
	mini::dx_ptr<ID3D11Buffer> constantBufferPS;
};