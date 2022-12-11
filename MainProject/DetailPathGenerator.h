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
	std::vector<float> GetBaseUValues(std::shared_ptr<IParameterized> mugBaseOffset);
	void GenerateTopPath(std::shared_ptr<IParameterized> mugTopOffset, std::vector<DirectX::SimpleMath::Vector3>& positions, float intersectionX, bool baseEndedLeft);
	std::vector<float> GetTopUValues(std::shared_ptr<IParameterized> mugTopOffset);

	void GenerateHandlePaths(
		std::shared_ptr<IParameterized> handleOffset,
		std::shared_ptr<IParameterized> baseOffset,
		std::vector<DirectX::SimpleMath::Vector3>& positions,
		std::vector<DirectX::SimpleMath::Vector3> handleBottomIntersection,
		std::vector<DirectX::SimpleMath::Vector3> handleTopIntersection);

	bool GetCurvesIntersection(
		std::vector<DirectX::SimpleMath::Vector3> curve1,
		std::vector<DirectX::SimpleMath::Vector3> curve2,
		int* outIntersectionIndex1
	);

	bool GenerateHolePath(
		std::shared_ptr<IParameterized> handleOffset,
		std::shared_ptr<IParameterized> baseOffset,
		std::vector<DirectX::SimpleMath::Vector3>& positions
	);
private:
	float drillRadiusP4 = 0.4f;
	float baseHeight = 1.6f;
	float overTheModelHeight = 5.0f;
	const DirectX::SimpleMath::Vector3 beginPosition = DirectX::SimpleMath::Vector3(0, 6.6f, 0);
	float pointsDistance = 0.05f;
	float precision = 0.005f;

	float lift = 0.1;
};