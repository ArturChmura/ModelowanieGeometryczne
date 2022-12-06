#pragma once

#include "IModel.h"
#include "StraightCurveInterpolating.h"
#include "IParameterized.h"

class DetailPathGenerator
{
public:
	std::shared_ptr< StraightCurveInterpolating> GeneralPaths(std::vector<std::shared_ptr<IModel>> models);
	void GenerateBasePath(std::shared_ptr<IParameterized> mugBaseOffset, std::vector<DirectX::SimpleMath::Vector3>& positions);
	void GenerateTopPath(std::shared_ptr<IParameterized> mugTopOffset, std::vector<DirectX::SimpleMath::Vector3>& positions);
private:
	float drillRadiusP4 = 0.4f;
	float baseHeight = 1.6f;
};