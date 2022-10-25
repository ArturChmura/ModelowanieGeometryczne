#include "FlatCutter.h"

using namespace DirectX::SimpleMath;

FlatCutter::FlatCutter(float radius, float cuttingPartHeight)
    :ICutter(radius, cuttingPartHeight)
{
    auto getHeight = [&](float x, float y) {return GetHeight(x, y); };
    this->model = std::make_shared<CutterModel>(radius, getHeight);
}

float FlatCutter::GetHeight(float x, float y)
{
    return 0.0f;
}

bool FlatCutter::IsError(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, float maxHeight, std::string& outErrorMessage)
{
    bool isError = ICutter::IsError(start, end, maxHeight, outErrorMessage);
    if (maxHeight > 0 && end.y < start.y)
    {
        isError = true;
        outErrorMessage += "Milling with the base of a flat cutter.\n";
    }
    return isError;
}

DirectX::SimpleMath::Vector3 FlatCutter::GetCuttingPointInDirection(DirectX::SimpleMath::Vector2 radiusPoint, DirectX::SimpleMath::Vector3 direction)
{
    auto d2 = radiusPoint.LengthSquared();
    auto r2 = radius * radius;
    
    if (d2 > r2)
    {
        d2 = r2; 
    }
    auto distance = sqrtf(r2 - d2);
    auto x = Vector3(radiusPoint.x, 0, radiusPoint.y) + distance * Vector3(direction.x, 0, direction.z);
    return x;
}
