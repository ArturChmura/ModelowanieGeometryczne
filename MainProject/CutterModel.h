#pragma once
#include "VertexModel.h"
#include "IParameterized.h"

class CutterModel : public VertexModel
{
public:

	CutterModel(float radius, std::function<float(float, float)> getHeight);
	float GetRadius() { return radius; }
	void SetRadius(float radius);

	virtual void SetTranslation(float x, float y, float z)  override;
private:
	std::function<float(float, float)> getHeight;
	void UpdateSlicesCount();
	float radius;

	int ringPointsCount = 20;
	int radiusPointsCount = 20;

	// Inherited via VertexModel
	virtual void Accept(AbstractModelVisitor& visitor) override;

};