#include "DetailPathGenerator.h"
#include "OffsetParametrized.h"
#include "PathGenerationHelper.h"
#include "Scene.h"
#include "IntersectionFinder.h"
#include "StraightCurveInterpolatingFactory.h"
#include <limits>
#include "BezierSurfacesFactory.h"

using namespace DirectX::SimpleMath;

std::shared_ptr<StraightCurveInterpolating> DetailPathGenerator::GeneralPaths(std::vector<std::shared_ptr<IModel>> models, std::shared_ptr<Scene> scene)
{
	auto itMug = find_if(models.begin(), models.end(), [](const std::shared_ptr<IModel> model) { return model->name == "Kufel"; });
	auto mugBase = std::dynamic_pointer_cast<IParameterized>(*itMug);
	auto itTop = find_if(models.begin(), models.end(), [](const std::shared_ptr<IModel> model) { return model->name == "Nakrycie"; });
	auto mugTop = std::dynamic_pointer_cast<IParameterized>(*itTop);
	auto itHandle = find_if(models.begin(), models.end(), [](const std::shared_ptr<IModel> model) { return model->name == "Raczka C0"; });
	auto mugHandle = std::dynamic_pointer_cast<IParameterized>(*itHandle);
	auto [basePlane, _] = BezierSurfacesFactory::CreateBezierSurfaceC0(1, 1, 15, 15, false, DirectX::XM_PIDIV2, 0, 0);

	auto mugBaseOffset = std::make_shared< OffsetParametrized>(mugBase, drillRadiusP4);
	auto mugTopOffset = std::make_shared< OffsetParametrized>(mugTop, drillRadiusP4);
	auto mugHandleOffset = std::make_shared< OffsetParametrized>(mugHandle, drillRadiusP4);
	auto basePlaneOffset = std::make_shared< OffsetParametrized>(basePlane, drillRadiusP4);

	auto intersectionFinder = std::make_shared<IntersectionFinder>(0.1, 0.01, false, Vector3(0, 0, 0));
	auto topBaseIntersectionPointsPtr = intersectionFinder->FindIntersection(mugTopOffset, mugBaseOffset);
	auto topBaseIntersectionPoints = *topBaseIntersectionPointsPtr;

	intersectionFinder->SetCursorPosition(-3.5, 0.2, -0.7);
	auto handleBaseDownIntersectionPointsPtr = intersectionFinder->FindIntersection(mugHandleOffset, mugBaseOffset);
	auto handleBaseDownIntersectionPoints = *handleBaseDownIntersectionPointsPtr;

	intersectionFinder->SetCursorPosition(2, 1.2, -0.7);
	auto handleBaseUpIntersectionPointsPtr = intersectionFinder->FindIntersection(mugHandleOffset, mugBaseOffset);
	auto handleBaseUpIntersectionPoints = *handleBaseUpIntersectionPointsPtr;

	auto handleBaseUpIntersectionPositions = std::vector<Vector3>();
	auto handleBaseDownIntersectionPositions = std::vector<Vector3>();



	for (auto intersectionPoint : handleBaseUpIntersectionPoints)
	{
		handleBaseUpIntersectionPositions.push_back(intersectionPoint.position);
	}
	for (auto intersectionPoint : handleBaseDownIntersectionPoints)
	{
		handleBaseDownIntersectionPositions.push_back(intersectionPoint.position);
	}

	auto handleBaseUpIntersectionCurve = StraightCurveInterpolatingFactory::CreateCurveFromIntersectionPoints(handleBaseUpIntersectionPoints);
	auto handleBaseDownIntersectionCurve = StraightCurveInterpolatingFactory::CreateCurveFromIntersectionPoints(handleBaseDownIntersectionPoints);

	scene->AddModel(handleBaseUpIntersectionCurve.first);
	scene->AddModel(handleBaseDownIntersectionCurve.first);

	


	auto positions = std::vector<Vector3>();
	positions.push_back(beginPosition);

	auto baseTopPositions = std::vector<Vector3>();
	GenerateBasePath(mugBaseOffset, baseTopPositions, topBaseIntersectionPoints[0].position.x, handleBaseDownIntersectionPositions, handleBaseUpIntersectionPositions);
	GenerateTopPath(mugTopOffset, baseTopPositions, topBaseIntersectionPoints[0].position.x, baseTopPositions[baseTopPositions.size() - 1].z > 0);
	auto firstBasePositions = baseTopPositions[0];
	firstBasePositions.y = beginPosition.y;
	positions.push_back(firstBasePositions);
	positions.insert(positions.end(), baseTopPositions.begin(), baseTopPositions.end());

	auto lastPosition = baseTopPositions[baseTopPositions.size() - 1];
	lastPosition.y = overTheModelHeight;
	positions.push_back(lastPosition);


	auto handlePositions = std::vector<Vector3>();
	GenerateHandlePaths(mugHandleOffset, basePlaneOffset, handlePositions, handleBaseDownIntersectionPositions, handleBaseUpIntersectionPositions);
	auto firstHandlePositions = handlePositions[0];
	firstHandlePositions.y = overTheModelHeight;
	positions.push_back(firstHandlePositions);
	positions.insert(positions.end(), handlePositions.begin(), handlePositions.end());

	auto points = std::vector<std::shared_ptr<Point>>();
	for (auto position : positions)
	{
		auto point = std::make_shared<Point>(position + Vector3(0, baseHeight - drillRadiusP4, 0));
		points.push_back(point);
	}

	auto interpolated = std::make_shared<StraightCurveInterpolating>(points);





	return interpolated;
}


void DetailPathGenerator::GenerateBasePath(
	std::shared_ptr<IParameterized> mugBaseOffset,
	std::vector<DirectX::SimpleMath::Vector3>& positions,
	float intersectionWithTopX,
	std::vector<DirectX::SimpleMath::Vector3> handleBottomIntersection,
	std::vector<DirectX::SimpleMath::Vector3> handleTopIntersection)
{
	int leftToRightPathCount = 80;
	float vBegin = 0.900;
	float vEnd = 1.400;

	float vStep = (vEnd - vBegin) / (leftToRightPathCount - 1);

	auto uValues = GetBaseUValues(mugBaseOffset);

	bool swapDirectionToFromRight = false;
	for (int i = 0; i < uValues.size(); i++)
	{
		float u = uValues[i];
		std::vector<Vector3> currentPositions;
		auto position = mugBaseOffset->GetValue(u, 0);
		if (position.x > intersectionWithTopX)
		{
			break;
		}
		for (float v = vBegin; v < vEnd + vStep / 2; v += vStep)
		{
			float vDir = swapDirectionToFromRight ? vEnd - (v - vBegin) : v;

			auto position = mugBaseOffset->GetValue(u, vDir);
		
			if (position.y < drillRadiusP4)
			{
				continue;
			}
			currentPositions.push_back(position);
		}
		int intersectionIndex;
		float pointsDistance = Vector3::Distance(currentPositions[0], currentPositions[1]);
		auto intersection1 = GetCurvesIntersection(currentPositions, handleBottomIntersection, &intersectionIndex);
		if (!intersection1)
		{
			intersection1 = GetCurvesIntersection(currentPositions, handleTopIntersection, &intersectionIndex);
		}
		int startIndex = 0;
		int endIndex = currentPositions.size() - 1;
		if (intersection1)
		{
			if (swapDirectionToFromRight)
			{
				startIndex = intersectionIndex + 1;
			}
			else
			{
				endIndex = intersectionIndex;
			}
		}

		auto firstLifted = currentPositions[startIndex] + Vector3(0, lift, 0);
		positions.push_back(firstLifted);
		positions.insert(positions.end(), currentPositions.begin() + startIndex, currentPositions.begin() + endIndex + 1);
		auto lastLifted = currentPositions[endIndex] + Vector3(0, lift, 0);
		positions.push_back(lastLifted);


		swapDirectionToFromRight = !swapDirectionToFromRight;
	}
}

bool DetailPathGenerator::GetCurvesIntersection(std::vector<Vector3> curve1, std::vector<Vector3> curve2, int* outIntersectionIndex1)
{
	float minDistanceSquared = FLT_MAX;
	int minDistanceIndex1 = -1;
	int minDistanceIndex2 = -1;
	float distanceBetweenPointsOnCurve1 = Vector3::Distance(curve1[0], curve1[1]);
	for (int i = 0; i < curve1.size(); i++)
	{
		if (i + 1 < curve1.size())
		{
			distanceBetweenPointsOnCurve1 = Vector3::Distance(curve1[i], curve1[i+1]);
		}
		float distanceBetweenPointsOnCurve2 = Vector3::Distance(curve2[0], curve2[1]);
		for (int j = 0; j < curve2.size(); j++)
		{
			if (j + 1 < curve2.size())
			{
				distanceBetweenPointsOnCurve2 = Vector3::Distance(curve2[j], curve2[j + 1]);
			}
			auto point1 = curve1[i];
			auto point2 = curve2[j];
			auto distanceSquared = Vector3::DistanceSquared(point1, point2);
			float minDistance = max(distanceBetweenPointsOnCurve2, distanceBetweenPointsOnCurve1);
			if (distanceSquared < minDistance * minDistance && distanceSquared < minDistanceSquared)
			{
				minDistanceSquared = distanceSquared;
				minDistanceIndex1 = i;
				minDistanceIndex2 = j;
			}

		}
	}

	if (minDistanceIndex1 == -1)
	{
		return false;
	}

	auto minDistancePosition1 = curve1[minDistanceIndex1];
	auto minDistancePosition2 = curve2[minDistanceIndex2];

	int l1Index;
	int l2Index;
	int r1Index;
	int r2Index;

	if (minDistanceIndex1 > 0)
	{
		if (minDistanceIndex1 < curve1.size() - 1)
		{
			auto prev1 = curve1[minDistanceIndex1 - 1];
			auto next1 = curve1[minDistanceIndex1 + 1];
			if (Vector3::DistanceSquared(prev1, minDistancePosition2) < Vector3::DistanceSquared(next1, minDistancePosition2))
			{
				l1Index = minDistanceIndex1 - 1;
				r1Index = minDistanceIndex1;
			}
			else
			{

				l1Index = minDistanceIndex1;
				r1Index = minDistanceIndex1 + 1;
			}
		}
		else
		{
			l1Index = curve1.size() - 2;
			r1Index = curve1.size() - 1;
		}
	}
	else
	{
		l1Index = 0;
		r1Index = 1;
	}

	if (minDistanceIndex2 > 0)
	{
		if (minDistanceIndex2 < curve2.size() - 1)
		{
			auto prev2 = curve2[minDistanceIndex2 - 1];
			auto next2 = curve2[minDistanceIndex2 + 1];
			if (Vector3::DistanceSquared(prev2, minDistancePosition1) < Vector3::DistanceSquared(next2, minDistancePosition1))
			{
				l2Index = minDistanceIndex2 - 1;
				r2Index = minDistanceIndex2;
			}
			else
			{

				l2Index = minDistanceIndex2;
				r2Index = minDistanceIndex2 + 1;
			}
		}
		else
		{
			l2Index = curve2.size() - 2;
			r2Index = curve2.size() - 1;
		}
	}
	else
	{
		l2Index = 0;
		r2Index = 1;
	}

	*outIntersectionIndex1 = l1Index;
	
	return true;
}

bool DetailPathGenerator::GenerateHolePath(std::shared_ptr<IParameterized> handleOffset, std::shared_ptr<IParameterized> baseOffset, std::vector<DirectX::SimpleMath::Vector3>& positions)
{
	return false;
}

std::vector<float> DetailPathGenerator::GetBaseUValues(std::shared_ptr<IParameterized> mugBaseOffset)
{
	int resolution = 10000;
	float step = 1.0f / resolution;
	auto Us = std::vector<float>();
	auto lastPosition = Vector3(-100, -100, -100);
	float distanceBetweenLines = 0.1;
	for (float u = 0; u < 1.0f; u += step)
	{
		auto currentPosition = mugBaseOffset->GetValue(u, 0.15);
		if (Vector3::Distance(lastPosition, currentPosition) > distanceBetweenLines)
		{
			Us.push_back(u);
			lastPosition = currentPosition;
		}
	}

	return Us;
}


void DetailPathGenerator::GenerateTopPath(std::shared_ptr<IParameterized> mugTopOffset, std::vector<Vector3>& positions, float intersectionX, bool baseEndedLeft)
{
	int leftToRightPathCount = 80;
	float vBegin = 0.900;
	float vEnd = 1.400;

	auto uValues = GetTopUValues(mugTopOffset);
	float vStep = (vEnd - vBegin) / (leftToRightPathCount - 1);

	bool swapDirectionToRight = !baseEndedLeft;
	for (int i = 0; i < uValues.size(); i++)
	{
		float u = uValues[i];
		std::vector<Vector3> currentPositions;
		auto position = mugTopOffset->GetValue(u, 0);
		if (position.x < intersectionX)
		{
			continue;
		}
		for (float v = vBegin; v < vEnd + vStep / 2; v += vStep)
		{
			float u = uValues[i];
			float vDir = swapDirectionToRight ? vEnd - (v - vBegin) : v;

			auto position = mugTopOffset->GetValue(u, vDir);
			
			if (position.y < drillRadiusP4)
			{
				continue;
			}
			currentPositions.push_back(position);
		}

		auto firstLifted = currentPositions[0] + Vector3(0, lift, 0);
		positions.push_back(firstLifted);
		positions.insert(positions.end(), currentPositions.begin(), currentPositions.end());
		auto lastLifted = currentPositions[currentPositions.size()-1] + Vector3(0, lift, 0);
		positions.push_back(lastLifted);

		swapDirectionToRight = !swapDirectionToRight;
	}
}


std::vector<float> DetailPathGenerator::GetTopUValues(std::shared_ptr<IParameterized> mugTopOffset)
{
	int resolution = 100000;
	float step = 1.0f / resolution;
	auto lastPosition = Vector3(-100, -100, -100);
	float distanceBetweenLines = 0.1;

	auto Us = std::vector<float>();
	auto positions = std::vector<Vector3>();

	for (float u = 0; u < 1.0f; u += step)
	{
		auto currentPosition = mugTopOffset->GetValue(u, 0.15);
		if (Vector3::Distance(lastPosition, currentPosition) > distanceBetweenLines)
		{
			Us.push_back(u);
			positions.push_back(currentPosition);
			lastPosition = currentPosition;
		}
	}

	auto positionsWithoutIntersections = PathGenerationHelper::RemoveSelfIntersections(positions);

	auto UsWithoutIntersections = std::vector<float>();
	int j = 0;
	for (int i = 0; i < Us.size(); i++)
	{
		if (positions[i] == positionsWithoutIntersections[j])
		{
			UsWithoutIntersections.push_back(Us[i]);
			j++;
		}
	}

	return UsWithoutIntersections;
}


void DetailPathGenerator::GenerateHandlePaths(
	std::shared_ptr<IParameterized> handleOffset,
	std::shared_ptr<IParameterized> baseOffset,
	std::vector<DirectX::SimpleMath::Vector3>& positions,
	std::vector<DirectX::SimpleMath::Vector3> handleBottomIntersection,
	std::vector<DirectX::SimpleMath::Vector3> handleTopIntersection)
{
	int topToBottomPointsCount = 100;
	int leftToRightPathCount = 60;


	bool swapDirectionToFromTop = true;
	auto cutCurveToIntersections = [&](std::vector<Vector3>& currentPositions)
	{
		int intersectionIndexBottom;
		int intersectionIndexTop;
		float pointsDistance = Vector3::Distance(currentPositions[0], currentPositions[1]);
		auto intersectionBottom = GetCurvesIntersection(currentPositions, handleBottomIntersection, &intersectionIndexBottom);
		auto intersectionTop = GetCurvesIntersection(currentPositions, handleTopIntersection, &intersectionIndexTop);
		int startIndex = 0;
		int endIndex = currentPositions.size() - 1;
		if (intersectionBottom)
		{
			if (swapDirectionToFromTop)
			{
				endIndex = intersectionIndexBottom;
			}
			else
			{
				startIndex = intersectionIndexBottom + 1;
			}
		}
		if (intersectionTop)
		{
			if (swapDirectionToFromTop)
			{
				startIndex = intersectionIndexTop + 1;

			}
			else
			{
				endIndex = intersectionIndexTop;
			}
		}

		return std::vector<Vector3>(currentPositions.begin() + startIndex, currentPositions.begin() + endIndex + 1);
	};

	auto addCurveToPositions = [&](std::vector<Vector3>& currentPositions)
	{

		auto firstLifted = currentPositions[0] + Vector3(0, lift, 0);
		positions.push_back(firstLifted);
		positions.insert(positions.end(), currentPositions.begin(), currentPositions.end());
		auto lastLifted = currentPositions[currentPositions.size() - 1] + Vector3(0, lift, 0);
		positions.push_back(lastLifted);
	};



	float vBegin = 0.44878196268745163;
	float vEnd = 0.051168519;
	/*float vBegin = 0.151168519;
	float vEnd = 0.151168519;*/
	float vStep = (vEnd - vBegin) / (leftToRightPathCount - 1);
	float uStep = 1.0f / (topToBottomPointsCount - 1);
	float v = vBegin;
	while(v >= vEnd)
	{
		std::vector<Vector3> currentPositions;
		bool breaked = false;
		float u = 0.0f;
		while (u <= 1.0f)
		{

			float uDir = swapDirectionToFromTop ?1.0f-u : u;

			auto position = handleOffset->GetValue(uDir, v);

			if (position.y < drillRadiusP4)
			{
				position.y = drillRadiusP4;
			}
			currentPositions.push_back(position);

			u += uStep;
			if (u > 1.0f && u <= 1.0f + uStep / 2)
			{
				u = 1.0f;
			}
		}
		if (breaked)
		{
			continue;
		}
		
		currentPositions = PathGenerationHelper::RemoveSelfIntersections(currentPositions);
		auto cuttedCurve = cutCurveToIntersections(currentPositions);
		addCurveToPositions(cuttedCurve);
		swapDirectionToFromTop = !swapDirectionToFromTop;

		v += vStep;
		if (v < vEnd && v >= vEnd + vStep / 2)

		{
			v = vEnd;
		}
	}

}