#pragma once

class IFilter
{
public:
	IFilter()
	{
		static int id = 0;
		FilterId = id++;
	}
	virtual bool IsFiltered(float u, float v) = 0;
	int FilterId;
};