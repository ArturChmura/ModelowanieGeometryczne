#include "ICutter.h"

using namespace DirectX::SimpleMath;

ICutter::ICutter(float radius, float cuttingPartHeight)
{
    this->radius = radius;
    this->cuttingPartHeight = cuttingPartHeight;
}

float ICutter::GetRadius()
{
    return radius;
}

void ICutter::SetRadius(float radius)
{
    this->radius = radius;
    this->model->SetRadius(radius);
}
void ICutter::SetCuttingPartHeight(float cuttingPartHeight)
{
    this->cuttingPartHeight = cuttingPartHeight;
}

void ICutter::SetPosition(DirectX::SimpleMath::Vector3 position)
{
    auto distance = Vector3::Distance(position, this->position);
    OutputDebugString((L"x: " + std::to_wstring(position.x) + L" y: " + std::to_wstring(position.y) + L" z: " + std::to_wstring(position.z) + L"\n").c_str());
    this->position = position;
    this->model->SetTranslation(position.x, position.y, position.z);
    this->model->Rotate(0, distance / 10, 0);
}

void ICutter::Draw(std::shared_ptr<Camera> camera)
{
    this->model->Draw(camera);
}

float ICutter::GetCuttingPartHeight()
{
    return cuttingPartHeight;
}

void ICutter::GetCuttingPartHeight(float cuttingPartHeight)
{
    this->cuttingPartHeight = cuttingPartHeight;
}

bool ICutter::IsError(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, float maxHeight, std::string& outErrorMessage)
{
    if (maxHeight > GetCuttingPartHeight())
    {
        outErrorMessage += "Milling with the non-cutting part. \n";
        return true;
    }
    return false;
}
