#include "Cone.h"

std::vector<DirectX::XMFLOAT3> Cone::GetVertices()
{
    return {
        {0.0f, 1.0f, 0.0f},
        { -1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f},
        { 0.0f, -1.0f, 1.0f}
    };
}

std::vector<unsigned int> Cone::GetIndices()
{
    return {
    1,3,2,
    1,4,3,
    1,2,4,
    2,3,4,
    };
}

int Cone::GetSize()
{
    return 12;
}
