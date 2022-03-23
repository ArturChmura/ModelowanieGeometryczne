#pragma once
#include  "IModel.h"
class Coursor3d
{
public:
	Coursor3d();
	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	void Draw(std::shared_ptr<Camera> camera);
	ShaderInfo shaderInfo;
	DirectX::XMFLOAT3 translation;
};