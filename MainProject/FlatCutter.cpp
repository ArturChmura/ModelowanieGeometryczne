#include "FlatCutter.h"

FlatCutter::FlatCutter(float radius)
    :ICutter(radius)
{
    auto getHeight = [&](float x, float y) {return GetHeight(x, y); };
    this->model = std::make_shared<CutterModel>(radius, getHeight);
}

float FlatCutter::GetHeight(float x, float y)
{
    return 0.0f;
}
