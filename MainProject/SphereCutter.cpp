#include "SphereCutter.h"
#include <cmath>
SphereCutter::SphereCutter(float radius)
    :ICutter(radius)
{
}
float SphereCutter::GetHeight(float x, float y)
{
    float d2 = x * x + y * y;
    float h2 = radius * radius - d2;
    return -1*std::sqrtf(h2);
}
