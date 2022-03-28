#include "MatrixExt.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
Vector3 MatrixExt::DecomposeScale(Matrix matrix)
{
    float x = Vector3(matrix._11, matrix._12, matrix._13).Length();
    float y = Vector3(matrix._21, matrix._22, matrix._23).Length();
    float z = Vector3(matrix._31, matrix._32, matrix._33).Length();
    return Vector3(x,y,z);
}

Vector4 MatrixExt::DecomposeTranslation(Matrix matrix)
{
    return Vector4(matrix._41, matrix._42, matrix._43, matrix._43);
}

Vector3 MatrixExt::DecomposeRotation(Matrix matrix)
{
    Vector3 scale = DecomposeScale(matrix);
    XMFLOAT3X3 r(
        matrix._11 / scale.x, matrix._12 / scale.x, matrix._13 / scale.x,
        matrix._21 / scale.y, matrix._22 / scale.y, matrix._23 / scale.y,
        matrix._31 / scale.z, matrix._32 / scale.z, matrix._33 / scale.z
    );
    float x = atan2(r._23, r._33);
    float y = atan2(-r._13, sqrtf(r._23 * r._23 + r._33 * r._33));
    float z = atan2(r._12, r._11);
    return Vector3(x, y, z);
}
