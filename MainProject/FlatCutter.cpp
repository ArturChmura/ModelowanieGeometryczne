#include "FlatCutter.h"

FlatCutter::FlatCutter(float radius)
    :ICutter(radius)
{
}

float FlatCutter::GetHeight(float x, float y)
{
    return 0.0f;
}
