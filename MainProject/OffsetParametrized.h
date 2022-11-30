#pragma once
#include "IParameterized.h"
#include "IModel.h"

class OffsetParametrized : public IParameterized
{
public:
	OffsetParametrized(std::shared_ptr<IParameterized> parametrized, float offset);
	std::shared_ptr<IParameterized> parametrized;
	float offset;

	// Inherited via IParameterized
	virtual DirectX::SimpleMath::Vector3 GetValue(double u, double v) override;


	virtual bool IsUWrapped() override;


	virtual bool IsVWrapped() override;


	virtual void OnFilterUpdate() override;


};