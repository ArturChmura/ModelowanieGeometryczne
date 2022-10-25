#include "SphereCutter.h"
#include <cmath>

using namespace DirectX::SimpleMath;
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

DirectX::SimpleMath::Vector3 SphereCutter::GetCuttingPointInDirection(DirectX::SimpleMath::Vector2 radiusPoint, DirectX::SimpleMath::Vector3 direction)
{
    auto d2 = radiusPoint.LengthSquared();
    auto r2 = radius * radius;

    if (d2 > r2)
    {
        d2 = r2;
    }

    auto perpendicular2d = Vector3(direction.z, 0, -direction.x);
    perpendicular2d.Normalize();
    auto perpendicularDown = perpendicular2d.Cross(direction);
    perpendicularDown.Normalize();

    auto distance = sqrtf(r2 - d2);

    auto x = Vector3(radiusPoint.x, 0, radiusPoint.y) + distance * perpendicularDown + Vector3(0,radius,0);
    return x;
}
