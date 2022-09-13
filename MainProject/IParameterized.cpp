#include "IParameterized.h"

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
		visible = visible && !filter->IsFiltered(u, v);
	}
	return visible;
}
