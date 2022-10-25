#pragma once
#include "ICutter.h"

class FlatCutter : public ICutter
{
public:
	FlatCutter(float radius, float cuttingPartHeight);
	// Inherited via ICutter
	virtual float GetHeight(float x, float y) override;
	virtual bool IsError(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, float maxHeight, std::string& outErrorMessage) override;
};