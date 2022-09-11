#pragma once
#include "SimpleMath.h"

class BernstainHelper
{
public:
	static DirectX::SimpleMath::Vector3 DeCasteljeu(std::array<DirectX::SimpleMath::Vector3, 4> coef, float t, int size)
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

	static DirectX::SimpleMath::Vector3 dU(std::array<DirectX::SimpleMath::Vector3, 4> coef, float t)
	{
		std::array<DirectX::SimpleMath::Vector3, 4> der
		{
			3 * (coef[1] - coef[0]),
			3 * (coef[2] - coef[1]),
			3 * (coef[3] - coef[2]),
			DirectX::SimpleMath::Vector3()
		};
		return DeCasteljeu(der, t, 3);
	}


	static DirectX::SimpleMath::Vector3 dUV(
		std::array<DirectX::SimpleMath::Vector3, 4> firstLine, 
		std::array<DirectX::SimpleMath::Vector3, 4> secondLine, 
		float t)
	{
		auto p1 = dU(firstLine, t);
		auto p2 = dU(secondLine, t);
		return 3 * (p1 - p2);
	}

	static DirectX::SimpleMath::Vector3 DeBoor(std::array<DirectX::SimpleMath::Vector3, 4> coef, float x)
	{
		int k = 3;
		float t[8] = { -3,-2,-1, 0, 1, 2,3,4 };
		int p = 3;

		std::array<DirectX::SimpleMath::Vector3, 4> d = { coef[0], coef[1], coef[2], coef[3] };
		for (int r = 1; r <= p; r++) // r in <1,4>
		{
			for (int j = p; j >= r; j--) // j in <3,0> <3,1> <3,2> <3,3>
			{
				float alpha = (x - t[j + k - p]) / (t[j + 1 + k - r] - t[j + k - p]);
				d[j] = (1.0f - alpha) * d[j - 1] + alpha * d[j];
			}

		}
		return d[3];
	}

	static DirectX::SimpleMath::Vector3 DeBoorDerivative(std::array<DirectX::SimpleMath::Vector3, 4> coef, float x)
	{
		int k = 3;
		float t[8] = { -3,-2,-1, 0, 1, 2,3,4 };
		int p = 3;

		std::array<DirectX::SimpleMath::Vector3, 3> q;
		for (int j = 0; j < p; j++)
		{
			q[j] = p * (coef[j + k - p + 1] - coef[j + k - p]) / (t[j + k + 1] - t[j + k - p + 1]);
		}

		std::array<DirectX::SimpleMath::Vector3, 4> d = { coef[0], coef[1], coef[2], coef[3] };
		for (int r = 1; r <= p-1; r++) // r in <1,4>
		{
			for (int j = p-1; j >= r; j--) // j in <3,0> <3,1> <3,2> <3,3>
			{
				int right = j + 1 + k - r;
				int	left = j + k - (p - 1);
				float alpha = (x - t[left]) / (t[right] - t[left]);
				q[j] = (1.0 - alpha) * q[j - 1] + alpha * q[j];
			}

		}
		return q[p-1];
	}
};