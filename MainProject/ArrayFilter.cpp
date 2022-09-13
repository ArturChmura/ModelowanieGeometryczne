#include "ArrayFilter.h"

ArrayFilter::ArrayFilter(int mode)
{
	if (mode)
	{
		for (int i = 0; i < ARRAY_SIZE; i++)
		{
			for (int j = 0; j < ARRAY_SIZE; j++)
			{
				filterArray[i][j] = i + j < ARRAY_SIZE;
			}
		}
	}
	else
	{
		for (int i = 0; i < ARRAY_SIZE; i++)
		{
			for (int j = 0; j < ARRAY_SIZE; j++)
			{
				filterArray[i][j] = i > ARRAY_SIZE / 2 && j > ARRAY_SIZE / 2;
			}
		}
	}

}

bool ArrayFilter::IsFiltered(float u, float v)
{
	int i = u * ARRAY_SIZE;
	int j = v * ARRAY_SIZE;
	return filterArray[i][j];
}
