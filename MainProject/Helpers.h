#pragma once

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

inline int modulo2(int a, int b)
{
	return (b + (a % b)) % b;
}

#define PI 3.14159265358979323846 


inline double GetInRange(double value, double min, double max)
{
	double d = max - min;
	while (value > max)
	{
		value -= d;
	}
	while (value < min)
	{
		value += d;
	}
	return value;
}


inline int GetInRangeInt(int value, int min, int max)
{
	int d = max - min + 1;
	while (value > max)
	{
		value -= d;
	}
	while (value < min)
	{
		value += d;
	}
	return value;
}