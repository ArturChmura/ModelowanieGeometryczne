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
    this->model->SetRadius(radius);
}

void ICutter::SetPosition(DirectX::SimpleMath::Vector3 position)
{
    this->position = position;
    this->model->SetTranslation(position.x, position.y, position.z);
}

void ICutter::Draw(std::shared_ptr<Camera> camera)
{
    this->model->Draw(camera);
}
