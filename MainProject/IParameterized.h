#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include "SimpleMath.h"


class IParameterized
{
public:
	virtual DirectX::SimpleMath::Vector3 GetValue(float u, float v) = 0;
	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(float u, float v) = 0;
	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(float u, float v) = 0;
	virtual bool IsUWrapped() = 0;
	virtual bool IsVWrapped() = 0;
};