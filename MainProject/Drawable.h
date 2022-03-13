#pragma once
#include <vector>
#include <DirectXMath.h>

class Shape
{
public:
	Shape();
	virtual std::vector<DirectX::XMFLOAT3> GetVertices() = 0;
	virtual std::vector<unsigned int> GetIndices() = 0;
	virtual int GetSize() = 0;
	virtual void Translate(float x, float y, float z);
	DirectX::XMFLOAT4X4 translationMatrix;
};