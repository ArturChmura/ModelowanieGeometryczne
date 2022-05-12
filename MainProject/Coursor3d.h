#pragma once
#include  "IModel.h"
#include <d3d11.h>
#include "SimpleMath.h"

class Coursor3d
{
public:
	Coursor3d();
	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	void Draw(std::shared_ptr<Camera> camera);
	DirectX::SimpleMath::Vector4 translation;
};