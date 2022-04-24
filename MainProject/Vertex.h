#pragma once
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 position;
};

struct Coef
{
	DirectX::XMFLOAT4 A;
	DirectX::XMFLOAT3 B;
	DirectX::XMFLOAT3 C;
	DirectX::XMFLOAT3 D;
};

struct VertexColor
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
};