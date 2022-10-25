#pragma once
#include "ICutter.h"

class FlatCutter : public ICutter
{
public:
	FlatCutter(float radius, float cuttingPartHeight);
	// Inherited via ICutter
	virtual float GetHeight(float x, float y) override;
	virtual bool IsError(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, float maxHeight, std::string& outErrorMessage) override;

	// Inherited via ICutter
	virtual DirectX::SimpleMath::Vector3 GetCuttingPointInDirection(DirectX::SimpleMath::Vector2 radiusPoint, DirectX::SimpleMath::Vector3 direction) override;

};