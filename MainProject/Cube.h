#pragma once
#include "Drawable.h"

class Cube : public Shape
{
public:
	std::vector<DirectX::XMFLOAT3> GetVertices();
	virtual std::vector<unsigned int> GetIndices();
	virtual int GetSize();
	
};