#include "Coursor3d.h"

Coursor3d::Coursor3d()
{
	this->translation = { 0,0,0 };
}

void Coursor3d::SetTranslation(DirectX::XMFLOAT3 translation)
{
	this->translation = translation;
}

void Coursor3d::Translate(DirectX::XMFLOAT3 translation)
{
	DirectX::XMStoreFloat3(&this->translation,
		DirectX::XMVectorAdd(
			DirectX::XMLoadFloat3(&this->translation),
			DirectX::XMLoadFloat3(&translation)));
}

DirectX::XMMATRIX Coursor3d::GetModelMatrix()
{
	return DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&translation));
}
