#include "VirtualPoint.h"

VirtualPoint::VirtualPoint(DirectX::SimpleMath::Vector3 location)
	: Point(location)
{
}

void VirtualPoint::OnSelect()
{
	IModel::OnSelect();
}
