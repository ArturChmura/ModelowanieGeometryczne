#pragma once
#include "VertexModel.h"
#include "IParameterized.h"

class Torus : public VertexModel, public IParameterized
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

	virtual DirectX::SimpleMath::Vector3 GetValue(float u, float v) override;
	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(float u, float v) override;
	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(float u, float v) override;
	virtual bool IsUWrapped() override;
	virtual bool IsVWrapped() override;

private:

	float R;
	float r;
	int largeSlices;
	int smallSlices;


	void UpdateSlicesCount();

	// Inherited via VertexModel
	virtual void Accept(AbstractModelVisitor& visitor) override;





};