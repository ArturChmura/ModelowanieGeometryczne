#pragma once
#include "Camera.h"

class ToolPaths
{
public:
	ToolPaths(std::vector<std::tuple<double, double, double>> points);
	void Draw(std::shared_ptr<Camera> camera);

	std::vector<std::tuple<double, double, double>> points;
};