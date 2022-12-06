#pragma once

#include "IParameterized.h"
#include <tuple>

class PathGenerationHelper
{
public:
	static std::vector<DirectX::SimpleMath::Vector3> RemoveSelfIntersections(std::vector<DirectX::SimpleMath::Vector3> positions, float pointsDistance);



};