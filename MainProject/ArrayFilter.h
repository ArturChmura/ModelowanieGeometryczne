#pragma once
#include "IFilter.h"
#include <array>

#define ARRAY_SIZE 1024
class ArrayFilter : public IFilter
{
public:
	ArrayFilter(int mode = 0);
	// Inherited via IFilter
	virtual bool IsFiltered(float u, float v) override;

	std::array<std::array<bool, ARRAY_SIZE>, ARRAY_SIZE> filterArray;
};