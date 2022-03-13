#include "Drawable.h"

Shape::Shape()
{
    DirectX::XMStoreFloat4x4(&translationMatrix, DirectX::XMMatrixIdentity());
}

void Shape::Translate(float x, float y, float z)
{
    DirectX::XMStoreFloat4x4(&translationMatrix, DirectX::XMMatrixTranslation(x,y,z));
}
