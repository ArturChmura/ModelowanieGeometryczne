#pragma once
#include "IBezierCurve.h"
#include <filesystem>

class CurveToToolPathsSerializer
{
public:
	static bool Serialize(std::vector<std::shared_ptr<Point>> points, std::filesystem::path filePath);
};