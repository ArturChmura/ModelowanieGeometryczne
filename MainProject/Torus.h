#pragma once
#include "VertexModel.h"

class Torus : public VertexModel
{
public:
	Torus(float R, float r, unsigned int largeSlices, unsigned int smallSlices);

	float GetBigRadius() { return R; }
	float GetSmallRadius() { return r; }
	int GetLargeSlices() { return largeSlices; }
	int GetSmallSlices() { return smallSlices; }


	void SetBigRadius(float R);
	void SetSmallRadius(float r);
	void SetLargeSlices(int ls);
	void SetSmallSlices(int ss);


	virtual void RenderGUI() override;
private:

	float R;
	float r;
	int largeSlices;
	int smallSlices;


	void UpdateSlicesCount();

	// Inherited via VertexModel
	virtual void Accept(AbstractModelVisitor& visitor) override;

};