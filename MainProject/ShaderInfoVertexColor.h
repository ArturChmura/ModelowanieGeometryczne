#pragma once
#include "ShaderInfo.h"
#include "MVPConstantBuffer.h"

class ShaderInfoVertexColor : public ShaderInfo
{
public:
	ShaderInfoVertexColor();
	// Inherited via ShaderInfo
	virtual void SetUpRender() override;
	virtual void SetVertexBuffer(ID3D11Buffer* vertexBuffer) override;
	virtual void CopyConstantBuffers() override;

	MVPConstantBuffer constantBufferStruct;
	D3D11_MAPPED_SUBRESOURCE res;
};