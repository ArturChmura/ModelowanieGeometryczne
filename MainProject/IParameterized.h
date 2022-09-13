#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include "SimpleMath.h"
#include "dxptr.h"
#include "IFilter.h"
#include <map>


class IParameterized
{
public:
	virtual DirectX::SimpleMath::Vector3 GetValue(float u, float v) = 0;
	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(float u, float v) = 0;
	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(float u, float v) = 0;
	virtual bool IsUWrapped() = 0;
	virtual bool IsVWrapped() = 0;

	void AddFilter(std::shared_ptr<IFilter> filter);
	void RemoveFilter(int filterId);
	bool IsVisible(float u, float v);
	virtual void OnFilterUpdate() = 0;

protected:
	std::map<int, std::shared_ptr<IFilter>> filters;
};