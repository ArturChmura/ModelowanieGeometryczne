#pragma once
#include <vector>
#include "Triangulation.h"
#include "VertexModel.h"

class Torus : public VertexModel
{
public:
	Torus(float R, float r, unsigned int largeSlices, unsigned int smallSlices);

	float GetBigRadius() { return R; }
	float GetSmallRadius() { return r; }
	int GetLargeSlices() { return largeSlices; }
	int GetSmallSlices() { return smallSlices; }
	DirectX::XMFLOAT3 GetColor() { return color; }

	void SetBigRadius(float R);
	void SetSmallRadius(float r);
	void SetLargeSlices(int ls);
	void SetSmallSlices(int ss);
	void SetColor(DirectX::XMFLOAT3 color);
private:

	float R;
	float r;
	int largeSlices;
	int smallSlices;

	DirectX::XMFLOAT3 color;

	void UpdateVertices();
};