#pragma once
#include  "IModel.h"
#include "ShaderInfoVertexColor.h"
class Coursor3d
{
public:
	Coursor3d();
	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	void Draw(std::shared_ptr<Camera> camera);
	DirectX::XMFLOAT3 translation;
	std::shared_ptr<ShaderInfoVertexColor> shaderInfoVertexColor;
};