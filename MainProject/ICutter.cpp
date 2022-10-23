#include "ICutter.h"

ICutter::ICutter(float radius)
{
    this->radius = radius;
}

float ICutter::GetRadius()
{
    return radius;
}

void ICutter::SetRadius(float radius)
{
    this->radius = radius;
}
