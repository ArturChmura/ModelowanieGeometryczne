#pragma once
#include "Drawable.h"

class Cone : public Shape
{
public:
	virtual std::vector<DirectX::XMFLOAT3> GetVertices();
	virtual std::vector<unsigned int> GetIndices();
	virtual int GetSize();
};