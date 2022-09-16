#pragma once
#include "IParameterized.h"
#include <tuple>

class HalfSurface : public IParameterized
{
public:
	HalfSurface(std::shared_ptr<IParameterized> surface, bool topLeft, int halfIndex);

	std::shared_ptr<IParameterized> surface;
	bool topLeft;
	int halfIndex;

	// Inherited via IParameterized
	virtual DirectX::SimpleMath::Vector3 GetValue(double u, double v) override;

	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(double u, double v) override;


	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(double u, double v) override;


	virtual bool IsUWrapped() override;


	virtual bool IsVWrapped() override;


	virtual void OnFilterUpdate() override;

	std::tuple<double, double> GetSurfaceUV(double u, double v);




};