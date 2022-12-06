#pragma once

#include "IModel.h"
#include "Scene.h"
#include "StraightCurveInterpolating.h"
#include "IParameterized.h"

class DetailPathGenerator
{
public:
	std::shared_ptr< StraightCurveInterpolating> GeneralPaths(std::vector<std::shared_ptr<IModel>> models, std::shared_ptr<Scene> scene);

	void GenerateBasePath(
		std::shared_ptr<IParameterized> mugBaseOffset, 
		std::vector<DirectX::SimpleMath::Vector3>& positions, 
		float intersectionWithTopX, 
		std::vector<DirectX::SimpleMath::Vector3> handleBottomIntersection,
		std::vector<DirectX::SimpleMath::Vector3> handleTopIntersection
	);
	bool GetCurvesIntersection(
		std::vector<DirectX::SimpleMath::Vector3> curve1,
		std::vector<DirectX::SimpleMath::Vector3> curve2,
		int* outIntersectionIndex1,
		float minDistance
	);
	std::vector<float> GetBaseUValues(std::shared_ptr<IParameterized> mugBaseOffset);
	void GenerateTopPath(std::shared_ptr<IParameterized> mugTopOffset, std::vector<DirectX::SimpleMath::Vector3>& positions, float intersectionX, bool baseEndedLeft);
	std::vector<float> GetTopUValues(std::shared_ptr<IParameterized> mugTopOffset);
private:
	float drillRadiusP4 = 0.4f;
	float baseHeight = 1.6f;
	float lift = 0.1;
};