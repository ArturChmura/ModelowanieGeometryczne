#pragma once
#include "DirectXMath.h"

class Coursor3d
{
public:
	Coursor3d();
	DirectX::XMFLOAT3 translation;
	void SetTranslation(DirectX::XMFLOAT3 translation);
	void Translate(DirectX::XMFLOAT3 translation);
	DirectX::XMMATRIX GetModelMatrix();
};