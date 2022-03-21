#pragma once
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 position;
};

struct VertexColor
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
};