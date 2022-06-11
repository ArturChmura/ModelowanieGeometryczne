#pragma once
#include <memory>
#include "Point.h"

struct PointSurfacePair
{
	std::shared_ptr<Point> point;
	int singleSurfaceId;
};