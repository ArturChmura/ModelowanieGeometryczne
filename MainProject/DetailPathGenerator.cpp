#include "DetailPathGenerator.h"
#include "OffsetParametrized.h"

using namespace DirectX::SimpleMath;

std::shared_ptr<StraightCurveInterpolating> DetailPathGenerator::GeneralPaths(std::vector<std::shared_ptr<IModel>> models)
{
	auto itMug = find_if(models.begin(), models.end(), [](const std::shared_ptr<IModel> model) { return model->name == "Kufel"; });
	auto mugBase = std::dynamic_pointer_cast<IParameterized>(*itMug);
	auto itTop = find_if(models.begin(), models.end(), [](const std::shared_ptr<IModel> model) { return model->name == "Nakrycie"; });
	auto mugTop = std::dynamic_pointer_cast<IParameterized>(*itTop);
	auto itHandle = find_if(models.begin(), models.end(), [](const std::shared_ptr<IModel> model) { return model->name == "Raczka C0"; });
	auto mugHandle = std::dynamic_pointer_cast<IParameterized>(*itHandle);

	auto mugBaseOffset = std::make_shared< OffsetParametrized>(mugBase, drillRadiusP4);
	auto mugTopOffset = std::make_shared< OffsetParametrized>(mugTop, drillRadiusP4);
	auto mugHandleOffset = std::make_shared< OffsetParametrized>(mugHandle, drillRadiusP4);


	auto positions = std::vector<Vector3>();
	GenerateBasePath(mugBaseOffset, positions);
	GenerateTopPath(mugTopOffset, positions);

	auto points = std::vector<std::shared_ptr<Point>>();
	for (auto position : positions)
	{
		auto point = std::make_shared<Point>(position + Vector3(0, baseHeight - drillRadiusP4 ,0));
		points.push_back(point);
	}

	auto interpolated = std::make_shared<StraightCurveInterpolating>(points);


	return interpolated;
}


void DetailPathGenerator::GenerateBasePath(std::shared_ptr<IParameterized> mugBaseOffset, std::vector<Vector3>& positions)
{
	int bottomToTopPathCount = 200;
	int leftToRightPathCount = 80;
	bool swapDirectionToRight = false;
	float vBegin = 0.900;
	float vEnd = 1.400;

	float vStep = (vEnd - vBegin) / (leftToRightPathCount - 1);

	for (int i = 0; i < bottomToTopPathCount; i++)
	{
		for (float v = vBegin; v < vEnd + vStep / 2; v += vStep)
		{
			float u = (float)i / (bottomToTopPathCount - 1);
			float vDir = swapDirectionToRight ? vEnd - (v - vBegin) : v;

			auto position = mugBaseOffset->GetValue(u, vDir);
			if (position.y < drillRadiusP4)
			{
				continue;
			}
			positions.push_back(position);
		}
		swapDirectionToRight = !swapDirectionToRight;
	}
}


void DetailPathGenerator::GenerateTopPath(std::shared_ptr<IParameterized> mugTopOffset, std::vector<Vector3>& positions)
{
	int bottomToTopPathCount = 100;
	int leftToRightPathCount = 80;
	bool swapDirectionToRight = false;
	float vBegin = 0.900;
	float vEnd = 1.400;

	float vStep = (vEnd - vBegin) / (leftToRightPathCount - 1);

	for (int i = 0; i < bottomToTopPathCount; i++)
	{
		for (float v = vBegin; v < vEnd + vStep / 2; v += vStep)
		{
			float u = (float)i / (bottomToTopPathCount - 1);
			float vDir = swapDirectionToRight ? vEnd - (v - vBegin) : v;

			auto position = mugTopOffset->GetValue(u, vDir);
			if (position.y < drillRadiusP4)
			{
				continue;
			}
			positions.push_back(position);
		}
		swapDirectionToRight = !swapDirectionToRight;
	}
}