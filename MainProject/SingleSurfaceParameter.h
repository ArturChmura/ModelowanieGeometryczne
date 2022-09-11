#pragma once

struct SingleSurfaceParameter
{
	std::shared_ptr<ISingleBezierSurface> singleSurface;

	float u;
	float v;
};