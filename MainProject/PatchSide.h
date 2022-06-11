#pragma once
#include <vector>
#include <memory>
#include "Point.h"


struct PatchSide
{
	int patchId;
	int pointIds[2];
};