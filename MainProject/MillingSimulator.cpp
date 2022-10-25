#include "MillingSimulator.h"
#include "Torus.h"

using namespace DirectX::SimpleMath;

MillingSimulator::MillingSimulator(std::shared_ptr<ToolPaths> toolPaths, std::shared_ptr<BlockModel> blockModel, std::shared_ptr<ICutter> cutter, float speed)
{
	this->toolPaths = toolPaths;
	this->blockModel = blockModel;
	this->speed = speed;
	this->cutter = cutter;
}

void MillingSimulator::StartMilling()
{
	pathIndex = 0;
	auto [x, y, z] = toolPaths->points[pathIndex];
	lastPosition = Vector3(x, y, z);

	blockModel->SetCutter(cutter);
}

bool MillingSimulator::Mill(float distance, bool& cancel)
{
	if (pathIndex >= toolPaths->points.size() - 1)
	{
		return true;
	}
	

	auto [x, y, z] = toolPaths->points[pathIndex + 1];
	auto endPoint = Vector3(x, y, z);

	auto toEndVector = endPoint - lastPosition;


	Vector3 toPoint;
	while (distance * distance > toEndVector.LengthSquared())
	{
		toPoint = endPoint;
		blockModel->DrawLine(lastPosition, toPoint);
		pathIndex++;
		if (pathIndex >= toolPaths->points.size() - 1)
		{
			return true;
		}
		distance -= toEndVector.Length();
		lastPosition = endPoint;
		auto [x, y, z] = toolPaths->points[pathIndex + 1];
		endPoint = Vector3(x, y, z);
		toEndVector = endPoint - lastPosition;
	}

	Vector3 direction = toEndVector;
	direction.Normalize();

	toPoint = lastPosition + direction * distance;

	blockModel->DrawLine(lastPosition, toPoint);

	lastPosition = toPoint;

	this->cutter->SetPosition(lastPosition);

	return false;
}

void MillingSimulator::SetSpeed(float speed)
{
	this->speed = speed;
}
