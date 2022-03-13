#pragma once
#include <DirectXMath.h>
class Model
{
public:
	Model();
	DirectX::XMFLOAT4X4 modelMatrix;

	void SetScale(float x, float y, float z);
	DirectX::XMFLOAT3 scale;

	void SetTranslation(float x, float y, float z);
	DirectX::XMFLOAT3 translation;
private:
	void UpdateModelMatrix();
};