#pragma once

class IFilter
{
public:
	IFilter()
	{
		static int id = 0;
		FilterId = id++;
	}
	virtual unsigned char IsFiltered(double u, double v) = 0;
	int FilterId;
};