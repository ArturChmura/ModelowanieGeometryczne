#pragma once
#include "IFilter.h"
#include <array>
#include <vector>
#include "Pair.h"

#define ARRAY_SIZE 1024
class ArrayFilter : public IFilter
{
public:
	ArrayFilter(std::vector<Pair<double>> points);
	void DrawLineBetweenPoints(Pair<double> uv1, Pair<double> uv2);
	// Inherited via IFilter
	virtual bool IsFiltered(double u, double v) override;

	bool** filterArray;

	void FloodFill(int x, int y);

	void Swap();
};