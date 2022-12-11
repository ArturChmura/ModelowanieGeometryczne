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
	auto topBaseIntersectionCurve = PathGenerationHelper::GetPositions(*topBaseIntersectionPointsPtr);

	intersectionFinder->SetCursorPosition(-3.5, 0.2, -0.7);
	auto handleBaseDownIntersectionPointsPtr = intersectionFinder->FindIntersection(mugHandleOffset, mugBaseOffset);

	intersectionFinder->SetCursorPosition(2, 1.2, -0.7);
	auto handleBaseUpIntersectionPointsPtr = intersectionFinder->FindIntersection(mugHandleOffset, mugBaseOffset);

	auto handleBaseUpIntersectionCurve = PathGenerationHelper::GetPositions(*handleBaseUpIntersectionPointsPtr);
	auto handleBaseDownIntersectionCurve = PathGenerationHelper::GetPositions(*handleBaseDownIntersectionPointsPtr);


	auto positions = std::vector<Vector3>();
	positions.push_back(beginPosition);

	auto baseTopPositions = std::vector<Vector3>();
	GenerateBasePath(mugBaseOffset, baseTopPositions, topBaseIntersectionCurve[0].x, handleBaseDownIntersectionCurve, handleBaseUpIntersectionCurve);
	GenerateTopPath(mugTopOffset, baseTopPositions, topBaseIntersectionCurve[0].x, baseTopPositions[baseTopPositions.size() - 1].z > 0);
	JoinPositionsParts(positions, baseTopPositions);



	auto handlePositions = std::vector<Vector3>();
	GenerateHandlePaths(mugHandleOffset, basePlaneOffset, handlePositions, handleBaseDownIntersectionCurve, handleBaseUpIntersectionCurve);
	JoinPositionsParts(positions, handlePositions);

	auto holePositions = std::vector<Vector3>();
	GenerateHolePath(mugHandleOffset, mugBaseOffset, holePositions);
	JoinPositionsParts(positions, holePositions);
	
	auto topBaseIntersectionPositions = std::vector<Vector3>();
	AddBaseIntersectionPoints(topBaseIntersectionCurve, topBaseIntersectionPositions);
	JoinPositionsParts(positions, topBaseIntersectionPositions);

	auto handleBaseUpIntersectionPositions = std::vector<Vector3>();
	AddBaseIntersectionPoints(handleBaseUpIntersectionCurve, handleBaseUpIntersectionPositions);
	JoinPositionsParts(positions, handleBaseUpIntersectionPositions);


	auto handleBaseDownIntersectionPositions = std::vector<Vector3>();
	AddBaseIntersectionPoints(handleBaseDownIntersectionCurve, handleBaseDownIntersectionPositions);
	JoinPositionsParts(positions, handleBaseDownIntersectionPositions);

	auto edgePositions = std::vector<Vector3>();
	GenerateEdgePoints(mugHandle, edgePositions, handleBaseDownIntersectionCurve, handleBaseUpIntersectionCurve);
	JoinPositionsParts(positions, edgePositions);

	auto lastPosition = positions[positions.size() - 1];
	lastPosition.y = overTheModelHeight;
	positions.push_back(lastPosition);

	positions.push_back(beginPosition);
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


		auto intersections = PathGenerationHelper::GetCurvesIntersections(currentPositions, handleBottomIntersection);
		if (intersections.size() == 0)
		{
			intersections = PathGenerationHelper::GetCurvesIntersections(currentPositions, handleTopIntersection);
		}
		int startIndex = 0;
		int endIndex = currentPositions.size() - 1;
		if (intersections.size() >= 1)
		{
			if (swapDirectionToFromRight)
			{
				startIndex = std::get<0>(intersections[0]) + 1;
			}
			else
			{
				endIndex = std::get<0>(intersections[0]);
			}
		}

		auto subVector = std::vector<Vector3>(currentPositions.begin() + startIndex, currentPositions.begin() + endIndex + 1);
		JoinCurvesWithLift(positions, subVector);


		swapDirectionToFromRight = !swapDirectionToFromRight;
	}
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


		JoinCurvesWithLift(positions, currentPositions);


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
		auto intersectionsBottom = PathGenerationHelper::GetCurvesIntersections(currentPositions, handleBottomIntersection);
		auto intersectionsTop = PathGenerationHelper::GetCurvesIntersections(currentPositions, handleTopIntersection);

		int startIndex = 0;
		int endIndex = currentPositions.size() - 1;

		if (intersectionsBottom.size() >= 1)
		{
			startIndex = std::get<0>(intersectionsBottom[0]) + 1;
		}
		if (intersectionsTop.size() >= 1)
		{
			endIndex = std::get<0>(intersectionsTop[0]);
		}
		

		return std::vector<Vector3>(currentPositions.begin() + startIndex, currentPositions.begin() + endIndex + 1);
	};

	float vBegin = 0.44878196268745163;
	float vEnd = 0.051168519;

	float vStep = (vEnd - vBegin) / (leftToRightPathCount - 1);
	float uStep = 1.0f / (topToBottomPointsCount - 1);
	float v = vBegin;
	while (v >= vEnd)
	{
		std::vector<Vector3> currentPositions;
		bool breaked = false;
		float u = 0.0f;
		while (u <= 1.0f)
		{
			auto position = handleOffset->GetValue(u, v);

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
		if (swapDirectionToFromTop)
		{
			std::reverse(cuttedCurve.begin(), cuttedCurve.end());
		}

		JoinCurvesWithLift(positions, cuttedCurve);
		swapDirectionToFromTop = !swapDirectionToFromTop;

		v += vStep;
		if (v < vEnd && v >= vEnd + vStep / 2)

		{
			v = vEnd;
		}
	}

}

void DetailPathGenerator::GenerateHolePath(std::shared_ptr<IParameterized> handleOffset, std::shared_ptr<IParameterized> baseOffset, std::vector<DirectX::SimpleMath::Vector3>& positions)
{
	float vHandleBase = 0.051168519;

	auto handleBorder = std::vector<Vector3>();
	float borderUStep = 1.0f / 100;
	for (float u = 0; u < 1.0f; u += borderUStep)
	{
		auto position = handleOffset->GetValue(u, vHandleBase);
		handleBorder.push_back(Vector3(position.x, drillRadiusP4, position.z));
	}


	auto [basePlane, _] = BezierSurfacesFactory::CreateBezierSurfaceC0(1, 1, 15, 15, false, DirectX::XM_PIDIV2, 0, 0);
	auto basePlaneOffset = std::make_shared<OffsetParametrized>(basePlane, drillRadiusP4);
	auto intersectionFinder = std::make_shared<IntersectionFinder>(0.005, 0.001, true, Vector3(-0.906, drillRadiusP4, -1.572));
	auto intersectionPointsPtr = intersectionFinder->FindIntersection(basePlaneOffset, baseOffset);
	auto intersectionPoints = *intersectionPointsPtr;
	auto baseBorder = std::vector<Vector3>();
	for (int i = 0; i < intersectionPoints.size(); ++i)
	{
		auto intersectionPoint = intersectionPoints[i];
		baseBorder.push_back(Vector3(intersectionPoint.position.x, drillRadiusP4, intersectionPoint.position.z));
	}

	float offset = 0.0f;
	float offsetStep = 0.1f;
	bool swapToFromTop = false;
	while (true)
	{
		offset += offsetStep;
		auto offsetBorder = baseBorder;
		for (int i = 0; i < offsetBorder.size(); i++)
		{
			offsetBorder[i].z -= offset;
		}

		auto intersections = PathGenerationHelper::GetCurvesIntersections(offsetBorder, handleBorder);
		std::vector<Vector3> path;
		if (intersections.size() == 2)
		{
			int start = std::get<0>(intersections[0]) + 1;
			int end = std::get<0>(intersections[1]);
			path.insert(path.begin(), offsetBorder.begin() + start, offsetBorder.begin() + end + 1);
		}
		else
		{
			break;
		}

		if (!swapToFromTop)
		{
			std::reverse(path.begin(), path.end());
		}

		JoinCurvesWithLift(positions, path);

		swapToFromTop = !swapToFromTop;
	}

	auto offsetBorderHandle = handleBorder;
	for (int i = 0; i < offsetBorderHandle.size(); i++)
	{
		offsetBorderHandle[i].z += offsetStep;
	}
	auto intersections = PathGenerationHelper::GetCurvesIntersections(offsetBorderHandle, baseBorder);
	std::vector<Vector3> path;
	if (intersections.size() == 2)
	{
		int start = std::get<0>(intersections[0]) + 1;
		int end = std::get<0>(intersections[1]);
		path.insert(path.begin(), offsetBorderHandle.begin() + start, offsetBorderHandle.begin() + end + 1);
		if (swapToFromTop)
		{
			std::reverse(path.begin(), path.end());
		}
		path = PathGenerationHelper::RemoveSelfIntersections(path);
		JoinCurvesWithLift(positions, path);
	}

}

void DetailPathGenerator::AddBaseIntersectionPoints(const std::vector<DirectX::SimpleMath::Vector3>& baseTopIntersections, std::vector<DirectX::SimpleMath::Vector3>& positions)
{
	int startIndex = -1;
	for (size_t i = 0; i < baseTopIntersections.size(); i++)
	{
		if (baseTopIntersections[i].y < drillRadiusP4)
		{
			startIndex = i;
		}
	}
	
	int index = startIndex;
	do
	{
		if (baseTopIntersections[index].y >= drillRadiusP4)
		{
			positions.push_back(baseTopIntersections[index]);
		}

		index = (index + 1) % baseTopIntersections.size();
	} while (index != startIndex);
}

void DetailPathGenerator::GenerateEdgePoints(
	std::shared_ptr<IParameterized> handle, 
	std::vector<DirectX::SimpleMath::Vector3>& positions,
	std::vector<DirectX::SimpleMath::Vector3> handleBottomIntersection, 
	std::vector<DirectX::SimpleMath::Vector3> handleTopIntersection)
{
	float edgeV = 0.25;
	float uStep = 1.0f / 100;
	float u = 0.0f;
	auto currentPositions = std::vector<Vector3>();
	while (u <= 1.0f)
	{
		auto position = handle->GetValue(u, edgeV);

		
		currentPositions.push_back(position + Vector3(0,drillRadiusP4,0));

		u += uStep;
		if (u > 1.0f && u <= 1.0f + uStep / 2)
		{
			u = 1.0f;
		}
	}

	auto intersectionsBottom = PathGenerationHelper::GetCurvesIntersections(currentPositions, handleBottomIntersection);
	auto intersectionsTop = PathGenerationHelper::GetCurvesIntersections(currentPositions, handleTopIntersection);

	int startIndex = 0;
	int endIndex = currentPositions.size() - 1;

	if (intersectionsBottom.size() >= 1)
	{
		startIndex = std::get<0>(intersectionsBottom[0]) + 1;
	}
	if (intersectionsTop.size() >= 1)
	{
		endIndex = std::get<0>(intersectionsTop[0]);
	}

	currentPositions =  std::vector<Vector3>(currentPositions.begin() + startIndex, currentPositions.begin() + endIndex + 1);
	positions.insert(positions.end(), currentPositions.begin(), currentPositions.end());

	return;
}

void DetailPathGenerator::JoinPositionsParts(std::vector<Vector3>& positionsDestination, std::vector<Vector3>& positionsSource)
{
	auto lastPosition = positionsDestination[positionsDestination.size() - 1];
	lastPosition.y = overTheModelHeight;
	positionsDestination.push_back(lastPosition);

	auto firstSrcPositions = positionsSource[0];
	firstSrcPositions.y = overTheModelHeight;
	positionsDestination.push_back(firstSrcPositions);
	positionsDestination.insert(positionsDestination.end(), positionsSource.begin(), positionsSource.end());
}

void DetailPathGenerator::JoinCurvesWithLift(std::vector<Vector3>& positionsDestination, std::vector<Vector3>& positionsSource)
{
	if (positionsDestination.size() > 0)
	{
		auto lastPosition = positionsDestination[positionsDestination.size() - 1];
		lastPosition.y += lift;
		positionsDestination.push_back(lastPosition);
	}

	auto firstSrcPositions = positionsSource[0];
	firstSrcPositions.y += lift;
	positionsDestination.push_back(firstSrcPositions);
	positionsDestination.insert(positionsDestination.end(), positionsSource.begin(), positionsSource.end());
}