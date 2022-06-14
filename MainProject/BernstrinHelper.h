#pragma once
#include "SimpleMath.h"

class BernstrinHelper
{
public:
	static DirectX::SimpleMath::Vector3 DeCasteljeu(DirectX::SimpleMath::Vector3 coef[4], float t, int size)
	{
		for (int i = 0; i < size - 1; i++)
		{
			int currentSize = size - i;
			for (int j = 0; j < currentSize - 1; j++)
			{
				coef[j] = coef[j] * (1.0f - t) + coef[j + 1] * t;
			}
		}
		DirectX::SimpleMath::Vector3 value = coef[0];

		return value;
	}

	static DirectX::SimpleMath::Vector3 dU(DirectX::SimpleMath::Vector3 coef[4], float t)
	{
		DirectX::SimpleMath::Vector3 der[4]
		{
			3 * (coef[1] - coef[0]),
			3 * (coef[2] - coef[1]),
			3 * (coef[3] - coef[2]),
			DirectX::SimpleMath::Vector3()
		};
		return DeCasteljeu(der, t, 3);
	}


	static DirectX::SimpleMath::Vector3 dUV(DirectX::SimpleMath::Vector3 firstLine[4], DirectX::SimpleMath::Vector3 secondLine[4], float t)
	{
		auto p1 = dU(firstLine, t);
		auto p2 = dU(secondLine, t);
		return 3 * (p1 - p2);
	}
};