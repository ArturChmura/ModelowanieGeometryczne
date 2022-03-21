#pragma once
#include "DirectXMath.h"

struct MVPColorConstantBuffer
{
	DirectX::XMMATRIX mvp;
	DirectX::XMVECTOR color;
};