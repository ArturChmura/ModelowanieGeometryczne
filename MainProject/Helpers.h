#pragma once

#include <vector>
#include <algorithm>

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

inline int modulo2(int a, int b)
{
	return (b + (a % b)) % b;
}

#define PI 3.14159265358979323846 

template <typename T>
struct Pair
{
	T a;
	T b;
};



template <typename T>
std::vector<T> SolveTriDiagonalMatrix(std::vector<float> as, std::vector<float> bs, std::vector<float> cs, std::vector<T> ds)
{
	int n = as.size();
	if (n == 0)
	{
		return std::vector<T>(0);
	}
	if (n == 1)
	{
		return std::vector<T>{ ds[0] / bs[0] };
	}
	std::vector<float> cPrim(n - 1);
	cPrim[0] = cs[0] / bs[0];
	for (int i = 1; i <= n - 2; i++)
	{
		cPrim[i] = cs[i] / (bs[i] - as[i] * cPrim[i - 1]);
	}

	std::vector < T> dPrim(n);
	dPrim[0] = ds[0] / bs[0];
	for (int i = 1; i <= n - 1; i++)
	{
		dPrim[i] = (ds[i] - as[i] * dPrim[i - 1]) / (bs[i] - as[i] * cPrim[i - 1]);
	}

	std::vector < T> xs(n);
	xs[n - 1] = dPrim[n - 1];
	for (int i = n - 2; i >= 0; i--)
	{
		xs[i] = dPrim[i] - cPrim[i] * xs[i + 1];
	}


	return xs;
}