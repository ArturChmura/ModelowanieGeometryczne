#pragma once

#include "SimpleMath.h"



class Parameterized
{
public:
	virtual DirectX::SimpleMath::Vector3 GetValue(float u, float v) = 0;
	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(float u, float v) = 0;
	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(float u, float v) = 0;
	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(float u, float v) = 0;
	virtual bool IsUWrapped() = 0;
	virtual bool IsVWrapped() = 0;
};