#include "ArrayFilter.h"
#include <algorithm>
#include <cmath>
#include <stack>
#include "Helpers.h"

ArrayFilter::ArrayFilter(std::vector<Pair<double>> points)
{
	filterArray = new bool* [ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		filterArray[i] = new bool[ARRAY_SIZE];
		std::fill_n(filterArray[i], ARRAY_SIZE, false);
		
	}
	for (int i = 1; i < points.size(); i++)
	{
		DrawLineBetweenPoints(points[i - 1], points[i]);
	}
	int startX = 0;
	int startY = 0;
	while (filterArray[startX][startY] == true)
	{
		startX++;
		if (startX >= ARRAY_SIZE)
		{
			startX = 0;
			startY++;
		}
	}
	FloodFill(startX, startY);
}




void ArrayFilter::DrawLineBetweenPoints(Pair<double> uv1, Pair<double> uv2)
{
	bool swapXY = false;
	auto setXY = [&](int x, int y, bool value) {
		if (swapXY)
		{
			std::swap(x, y);
		}
		x = GetInRangeInt(x, 0, ARRAY_SIZE - 1);
		y = GetInRangeInt(y, 0, ARRAY_SIZE - 1);
		filterArray[x][y] = value;
	};
	Pair<int> p1;
	p1.a = (int)(uv1.a * ARRAY_SIZE);
	p1.b = (int)(uv1.b * ARRAY_SIZE);

	Pair<int> p2;
	p2.a = (int)std::floor(uv2.a * ARRAY_SIZE);
	p2.b = (int)std::floor(uv2.b * ARRAY_SIZE);

	if (std::abs(p1.a - p2.a) < std::abs(p1.b - p2.b)) // linia musi byæ pozioma a nie pionowa
	{
		std::swap(p1.a, p1.b);
		std::swap(p2.a, p2.b);
		swapXY = true;
	}

	if (p1.a > p2.a) // p1 ma byæ po lewej stronie
	{
		std::swap(p1, p2);
	}

	for (int x = p1.a; x <= p2.a; x++)
	{
		int y = (p2.b - p1.b) / (double)(p2.a - p1.a) * (x - p1.a) + p1.b;
		setXY(x, y, true);
	}
}

bool ArrayFilter::IsFiltered(double u, double v)
{
	int i = u * ARRAY_SIZE;
	int j = v * ARRAY_SIZE;
	return filterArray[i][j];
}

void ArrayFilter::FloodFill(int x, int y)
{
	std::stack<Pair<int>> stack;
	stack.push({ x,y });
	while (!stack.empty())
	{
		auto n = stack.top();
		stack.pop();
		int x = GetInRangeInt(n.a, 0, ARRAY_SIZE - 1);
		int y = GetInRangeInt(n.b, 0, ARRAY_SIZE - 1);
		if (!filterArray[x][y])
		{
			filterArray[x][y] = true;
			stack.push({ x - 1, y  });
			stack.push({ x , y - 1 });
			stack.push({ x + 1, y  });
			stack.push({ x , y + 1 });
		}

	}
}

void ArrayFilter::Swap()
{
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		for (int j = 0; j < ARRAY_SIZE; j++)
		{
			filterArray[i][j] = !filterArray[i][j];
		}
	}
}
