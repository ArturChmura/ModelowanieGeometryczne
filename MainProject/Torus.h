#pragma once
#include <vector>
#include "Triangulation.h"
#include "Model.h"

class Torus : public Model
{
public:
	Torus(float R, float r, unsigned int largeSlices, unsigned int smallSlices);
	Triangulation GetVertices();

	float R;
	float r;
	int largeSlices;
	int smallSlices;

	DirectX::XMFLOAT3 color;
	void ChangeColor(DirectX::XMFLOAT3 color);
};