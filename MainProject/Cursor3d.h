#pragma once
#include  "IModel.h"
#include <d3d11.h>
#include "SimpleMath.h"
#include "Callback.h"

class Cursor3d
{
public:
	Cursor3d();
	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	void Draw(std::shared_ptr<Camera> camera);
	DirectX::SimpleMath::Vector3 GetTranslation();
	void SetPosition(DirectX::SimpleMath::Vector3 position);
	Callback<void(DirectX::SimpleMath::Vector3)> OnPositionChangeCallback;
private:
	DirectX::SimpleMath::Vector4 translation;
};