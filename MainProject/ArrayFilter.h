#pragma once
#include "IFilter.h"
#include <array>
#include <vector>
#include "Pair.h"

#define ARRAY_SIZE 512
class ArrayFilter : public IFilter
{
public:
	ArrayFilter(std::vector<Pair<double>> points, bool uWrapped, bool vWrapped);
	void DrawLineBetweenPoints(Pair<double> uv1, Pair<double> uv2);
	// Inherited via IFilter
	virtual unsigned char IsFiltered(double u, double v) override;

	unsigned char** filterArray;

	void FloodFill(int x, int y);

	void Swap();

	void FloodFill2(int x, int y);

	bool uWrapped;
	bool vWrapped;
};