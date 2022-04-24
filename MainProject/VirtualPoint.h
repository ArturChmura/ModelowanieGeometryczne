#pragma once
#include "Point.h"
class VirtualPoint : public Point
{
public:
	VirtualPoint(DirectX::SimpleMath::Vector3 location);
	virtual void OnSelect() override;
};