#pragma once
#include "DirectXMath.h"

struct ConstantBuffer
{
	DirectX::XMMATRIX mvp;
	DirectX::XMVECTOR color;
};