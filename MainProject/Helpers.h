#pragma once


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

