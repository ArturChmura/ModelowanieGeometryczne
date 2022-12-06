#pragma once
#include "IntersectionPoint.h"
#include "StraightCurveInterpolating.h"

class StraightCurveInterpolatingFactory
{
public:
	static std::pair<std::shared_ptr<StraightCurveInterpolating>, std::vector<std::shared_ptr<Point>>> CreateCurveFromIntersectionPoints(
		std::vector<IntersectionPoint> intersectionPoints
	);

	
};