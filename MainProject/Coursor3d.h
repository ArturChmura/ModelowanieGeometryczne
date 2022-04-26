#pragma once
#include  "IModel.h"
#include "SimpleMath.h"
#include "LineColorShaders.h"

class Coursor3d
{
public:
	Coursor3d();
	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	void Draw(std::shared_ptr<Camera> camera);
	DirectX::SimpleMath::Vector4 translation;
	LineColorShaders shaders;
};