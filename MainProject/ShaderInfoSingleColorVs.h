#pragma once
#include "ShaderInfo.h"
#include "MVPColorConstantBuffer.h"

class ShaderInfoSingleColorVs : public ShaderInfo
{
public:
	ShaderInfoSingleColorVs();
	// Inherited via ShaderInfo
	virtual void SetUpRender() override;
	virtual void SetVertexBuffer(ID3D11Buffer* vertexBuffer) override;
	virtual void CopyConstantBuffers() override;

	MVPColorConstantBuffer constantBufferStruct;
	D3D11_MAPPED_SUBRESOURCE res;
};