#include "OffsetParametrized.h"

OffsetParametrized::OffsetParametrized(std::shared_ptr<IParameterized> parametrized, float offset)
{
    this->parametrized = parametrized;
    this->offset = offset;
}

DirectX::SimpleMath::Vector3 OffsetParametrized::GetValue(double u, double v)
{
    auto UDer = parametrized->GetUDerivativeValue(u, v);
    auto VDer = parametrized->GetVDerivativeValue(u, v);
    auto normal = UDer.Cross(VDer);
    normal.Normalize();
    auto value = parametrized->GetValue(u, v);
    auto offsetValue = value + offset * normal;
    return offsetValue;
}


bool OffsetParametrized::IsUWrapped()
{
    return parametrized->IsUWrapped();
}

bool OffsetParametrized::IsVWrapped()
{
    return parametrized->IsVWrapped();
}

void OffsetParametrized::OnFilterUpdate()
{
    parametrized->OnFilterUpdate();
}
