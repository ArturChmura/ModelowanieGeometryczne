#include "SphereCutter.h"
#include <cmath>
SphereCutter::SphereCutter(float radius, float cuttingPartHeight)
    :ICutter(radius, cuttingPartHeight)
{
    auto getHeight = [&](float x, float y) {return GetHeight(x, y); };
    this->model = std::make_shared<CutterModel>(radius, getHeight);
}
float SphereCutter::GetHeight(float x, float y)
{
    float d2 = x * x + y * y;
    float h2 = radius * radius - d2;
    return -1*std::sqrtf(h2) + radius;
}
