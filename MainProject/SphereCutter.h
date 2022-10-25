#pragma once
#include "ICutter.h"

class SphereCutter : public ICutter
{
public:
	SphereCutter(float radius, float cuttingPartHeight);
	// Inherited via ICutter
	virtual float GetHeight(float x, float y) override;


	// Inherited via ICutter
	virtual DirectX::SimpleMath::Vector3 GetCuttingPointInDirection(DirectX::SimpleMath::Vector2 radiusPoint, DirectX::SimpleMath::Vector3 direction) override;


};