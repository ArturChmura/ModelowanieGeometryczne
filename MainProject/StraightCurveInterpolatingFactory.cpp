#include "StraightCurveInterpolatingFactory.h"

std::pair<std::shared_ptr<StraightCurveInterpolating>, std::vector<std::shared_ptr<Point>>> StraightCurveInterpolatingFactory::CreateCurveFromIntersectionPoints(std::vector<IntersectionPoint> intersectionPoints)
{
	auto points = std::vector<std::shared_ptr<Point>>();
	for (int i = 0; i < intersectionPoints.size(); ++i)
	{
		auto point = std::make_shared<Point>(intersectionPoints[i].position);
		points.push_back(point);

	}

	auto interpolated = std::make_shared<StraightCurveInterpolating>(points);
	return std::make_pair(interpolated, points);

}
