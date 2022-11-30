#include "ArrayFilter.h"
#include <algorithm>
#include <cmath>
#include <stack>
#include "Helpers.h"
#include <tuple>

ArrayFilter::ArrayFilter(std::vector<Pair<double>> points, bool uWrapped, bool vWrapped)
{
	this->uWrapped = uWrapped;
	this->vWrapped = vWrapped;
	filterArray = new unsigned char* [ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		filterArray[i] = new unsigned char[ARRAY_SIZE];
		std::fill_n(filterArray[i], ARRAY_SIZE, (unsigned char)0);
		
	}
	for (int i = 1; i < points.size(); i++)
	{
		DrawLineBetweenPoints(points[i - 1], points[i]);
	}
	int startX = 0;
	int startY = 0;
	while (filterArray[startX][startY] > 0)
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
	auto setXY = [&](int x, int y, unsigned char value) {
		if (swapXY)
		{
			std::swap(x, y);
		}
		x = GetInRangeInt(x, 0, ARRAY_SIZE - 1);
		y = GetInRangeInt(y, 0, ARRAY_SIZE - 1);
		filterArray[x][y] = value;
	};
	Pair<int> p1;
	p1.a = (int)std::floor(uv1.a * ARRAY_SIZE); 
	p1.b = (int)std::floor(uv1.b * ARRAY_SIZE);

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
		if (p1.a == p2.a && p1.b == p2.b)
		{
			continue;
		}
		int y = (p2.b - p1.b) / (double)(p2.a - p1.a) * (x - p1.a) + p1.b;
		setXY(x, y, (unsigned char)127);
	}
}

unsigned char ArrayFilter::IsFiltered(double u, double v)
{
	int i = u * ARRAY_SIZE;
	int j = v * ARRAY_SIZE;
	return filterArray[i][j];
}

void ArrayFilter::FloodFill(int x, int y)
{
	auto range = [=](int& x, int& y)
	{
		x = uWrapped ? GetInRangeInt(x, 0, ARRAY_SIZE - 1) : std::clamp(x, 0, ARRAY_SIZE - 1);
		y = vWrapped ? GetInRangeInt(y, 0, ARRAY_SIZE - 1) : std::clamp(y, 0, ARRAY_SIZE - 1);
	};
	std::stack<std::tuple<int,int>> stack;
	stack.push({ x,y });
	while (!stack.empty())
	{
		auto [x,y] = stack.top();
		stack.pop();
		range(x, y);
		if (filterArray[x][y]==(unsigned char)0)
		{
			filterArray[x][y] = (unsigned char)255;
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
			if (filterArray[i][j] == (unsigned char)0)
			{
				filterArray[i][j] = (unsigned char)255;
			}
			else if (filterArray[i][j] == (unsigned char)255)
			{
				filterArray[i][j] = (unsigned char)0;
			}
		}
	}
}

void ArrayFilter::FloodFill2(int x, int y)
{
	auto inside = [&](int x, int y)
	{
		x = uWrapped ? GetInRangeInt(x, 0, ARRAY_SIZE - 1) : std::clamp(x, 0, ARRAY_SIZE - 1);
		y = vWrapped ? GetInRangeInt(y, 0, ARRAY_SIZE - 1) : std::clamp(y, 0, ARRAY_SIZE - 1);
		return filterArray[x][y] == (unsigned char)0;
	};
	auto set = [&](int x, int y)
	{
		x = uWrapped ? GetInRangeInt(x, 0, ARRAY_SIZE - 1) : std::clamp(x, 0, ARRAY_SIZE - 1);
		y = vWrapped ? GetInRangeInt(y, 0, ARRAY_SIZE - 1) : std::clamp(y, 0, ARRAY_SIZE - 1);
		filterArray[x][y] = (unsigned char)255;
	};

	std::stack<std::tuple<int,int,int,int>> stack;
	stack.push(std::make_tuple(x, x, y, 1));
	stack.push(std::make_tuple(x, x, y-1, -1));

	while (!stack.empty())
	{

		auto [x1, x2, y, dy] = stack.top();
		stack.pop();
		int x = x1;
		if (inside(x, y))
		{
			while (inside(x - 1, y))
			{
				set(x - 1, y);
				x--;
			}
		}
		if (x < x1)
		{
			stack.push(std::make_tuple(x, x1 - 1, y - dy, -dy));
		}
		while (x1 <= x2)
		{
			while (inside(x1, y))
			{
				set(x1, y);
				x1++;
				stack.push(std::make_tuple(x, x1 - 1, y + dy, dy));
				if (x1 - 1 > x2)
				{
					stack.push(std::make_tuple(x2 + 1, x1 - 1, y - dy, -dy));
				}
			}
			x1++;
			while (x1 < x2 && !inside(x1, y))
			{
				x1 ++;
			}
			x = x1;
		}

		

	}
}