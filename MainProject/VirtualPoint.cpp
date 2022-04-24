#include "VirtualPoint.h"

VirtualPoint::VirtualPoint(DirectX::SimpleMath::Vector3 location)
	: Point(location)
{
	ChangeDefaultColor({ 0, 0.5f, 1.0f });
}

void VirtualPoint::OnSelect()
{
	IModel::OnSelect();
}
