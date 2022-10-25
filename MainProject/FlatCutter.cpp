#include "FlatCutter.h"

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
