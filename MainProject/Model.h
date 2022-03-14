#pragma once
#include <DirectXMath.h>
class Model
{
public:
	Model();
	DirectX::XMFLOAT4X4 GetModelMatrix();

	void SetScale(float x, float y, float z);
	DirectX::XMFLOAT3 GetScale();

	void SetTranslation(float x, float y, float z);
	DirectX::XMFLOAT3 GetTranslation();

	void SetRotation(float x, float y, float z);
	DirectX::XMFLOAT3 GetRotation();
private:
	DirectX::XMFLOAT4X4 modelMatrix;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 rotation;
	void UpdateModelMatrix();
	DirectX::XMMATRIX GetRotationMatrix();
};