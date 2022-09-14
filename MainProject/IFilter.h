#pragma once

class IFilter
{
public:
	IFilter()
	{
		static int id = 0;
		FilterId = id++;
	}
	virtual bool IsFiltered(double u, double v) = 0;
	int FilterId;
};