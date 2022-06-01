#include "IUnmovableModel.h"

IUnmovableModel::IUnmovableModel(std::string name):
IModel(name)
{
}

bool IUnmovableModel::IsMovable()
{
    return false;
}

void IUnmovableModel::SetScale(float x, float y, float z)
{
}

void IUnmovableModel::Scale(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 IUnmovableModel::GetScale()
{
    return DirectX::SimpleMath::Vector3();
}

void IUnmovableModel::ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale)
{
}

void IUnmovableModel::SetTranslation(float x, float y, float z)
{
}

void IUnmovableModel::Translate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector4 IUnmovableModel::GetTranslation()
{
    return DirectX::SimpleMath::Vector4();
}

void IUnmovableModel::SetRotation(float x, float y, float z)
{
}

void IUnmovableModel::Rotate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 IUnmovableModel::GetRotation()
{
    return DirectX::SimpleMath::Vector3();
}

void IUnmovableModel::RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation)
{
}