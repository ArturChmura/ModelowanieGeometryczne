#pragma once

#include "IParameterized.h"
#include <tuple>

class PathGenerationHelper
{
public:
	static std::vector<DirectX::SimpleMath::Vector3> RemoveSelfIntersections(std::vector<DirectX::SimpleMath::Vector3> positions);
	static std::vector<DirectX::SimpleMath::Vector3> RemoveIntersectionFromCurve(std::vector<DirectX::SimpleMath::Vector3> curve, std::vector<DirectX::SimpleMath::Vector3> intersectingCurve);

	static std::tuple< DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3,float> GetDistanceBetweenSegments(DirectX::SimpleMath::Vector3 a0, DirectX::SimpleMath::Vector3 a1, DirectX::SimpleMath::Vector3 b0, DirectX::SimpleMath::Vector3 b1);

};