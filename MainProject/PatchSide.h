#pragma once
#include <vector>
#include <memory>
#include "Point.h"
#include <array>

struct PatchSide
{
	int patchId;
	int pointIds[2];

	std::array<std::shared_ptr<Point>, 4> firstLine;
	std::array<std::shared_ptr<Point>, 4> secondLine;
};