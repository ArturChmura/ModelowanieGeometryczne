#include "PathGenerationHelper.h"

using namespace DirectX::SimpleMath;

std::vector<DirectX::SimpleMath::Vector3> PathGenerationHelper::RemoveSelfIntersections(std::vector<DirectX::SimpleMath::Vector3> positions, float pointsDistance)
{
	auto newPositions = std::vector<DirectX::SimpleMath::Vector3>();

	auto intersectionPoints = std::vector<std::pair<int, int>>();

	for (int i = 0; i < positions.size(); i++)
	{
		auto currentPosition = positions[i];
		for (int j = i + 4; j < positions.size() - 1; j++)
		{
			auto nextPosition = positions[j];
			auto distanceSquared = Vector3::DistanceSquared(currentPosition, nextPosition);
			if (distanceSquared < pointsDistance * pointsDistance*0.75)
			{
				while (j + 1 < positions.size())
				{
					auto nextPosition = positions[++j];
					auto distanceSquared2 = Vector3::DistanceSquared(currentPosition, nextPosition);
					if (distanceSquared2 > distanceSquared)
					{
						intersectionPoints.push_back(std::make_pair(i, j-1));
						i = j;
						break;
					}
				}
			}
		}
	}

	if (intersectionPoints.size() == 0)
	{
		newPositions = positions;
	}
	else
	{
		for (int i = 0; i < intersectionPoints[0].first; i++)
		{
			newPositions.push_back(positions[i]);
		}
		for (int i = 1; i < intersectionPoints.size(); i++)
		{
			int start = intersectionPoints[i - 1].second;
			int end = intersectionPoints[i].first;
			for (int j = start; j < end; j++)
			{
				newPositions.push_back(positions[j]);
			}
		}
		for (int i = intersectionPoints[intersectionPoints.size() - 1].second; i < positions.size(); i++)
		{
			newPositions.push_back(positions[i]);
		}
	}
	return newPositions;
}
