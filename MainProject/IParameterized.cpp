#include "IParameterized.h"
#include "Helpers.h"

DirectX::SimpleMath::Vector3 IParameterized::GetUDerivativeValue(double u, double v)
{
	u = GetInRange(u, 0.0, 1.0);
	v = GetInRange(v, 0.0, 1.0);

	double h = 0.0005;
	double uph = u + h;
	double umh = u - h;
	double mul = 1.0;
	if (uph > 1.0f)
	{
		if (!IsUWrapped())
		{
			uph = u;
			mul = 2.0;
		}
	}

	if (umh < 0.0f)
	{
		if (!IsUWrapped())
		{
			umh = u;
			mul = 2.0;
		}
	}

	auto valueUP = GetValue(uph, v);
	auto valueUM = GetValue(umh, v);


	auto value = (valueUP - valueUM) / (2*h) * mul;

	return value;
}

DirectX::SimpleMath::Vector3 IParameterized::GetVDerivativeValue(double u, double v)
{
	u = GetInRange(u, 0.0, 1.0);
	v = GetInRange(v, 0.0, 1.0);

	double h = 0.0005;
	double vph = v + h;
	double vmh = v - h;
	double mul = 1.0;
	if (vph > 1.0f)
	{
		if (!IsVWrapped())
		{
			vph = v;
			mul = 2.0;
		}
	}

	if (vmh < 0.0f)
	{
		if (!IsVWrapped())
		{
			vmh = v;
			mul = 2.0;
		}
	}

	auto valueUP = GetValue(u, vph);
	auto valueUM = GetValue(u, vmh);


	auto value = (valueUP - valueUM) / (2 * h) * mul;

	return value;
}

void IParameterized::AddFilter(std::shared_ptr<IFilter> filter)
{
	this->filters.emplace(filter->FilterId, filter);
	OnFilterUpdate();
}

void IParameterized::RemoveFilter(int filterId)
{
	filters.erase(filterId);
	OnFilterUpdate();
}

bool IParameterized::IsVisible(float u, float v)
{
	bool visible = true;
	for (auto [_, filter] : filters)
	{
		visible = visible && (int)filter->IsFiltered(u, v) > 0;
	}
	return visible;
}
