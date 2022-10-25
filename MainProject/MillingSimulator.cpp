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
		isThreadRunning = false;
		return true;
	}

	auto drawLine = [&](Vector3 from, Vector3 to)
	{
		auto drawResult = blockModel->DrawLine(from, to);
		this->cutter->SetPosition(to);
		lastPosition = to;
		std::string errorMessage;
		bool error = cutter->IsError(from, to, drawResult.maxHeightChange, errorMessage);
		error |= blockModel->IsError(drawResult.minHeightCutted, errorMessage);
		if (error)
		{
			errorMessages += "Error on path " + std::to_string(pathIndex) + "\n" + errorMessage;
		}
		
	};

	auto [x, y, z] = toolPaths->points[pathIndex + 1];
	auto endPoint = Vector3(x, y, z);

	auto toEndVector = endPoint - lastPosition;


	Vector3 toPoint;
	while (distance * distance > toEndVector.LengthSquared() && !cancel)
	{
		toPoint = endPoint;

		drawLine(lastPosition, toPoint);

		pathIndex++;
		if (pathIndex >= toolPaths->points.size() - 1)
		{
			isThreadRunning = false;
			return true;
		}
		distance -= toEndVector.Length();
		auto [x, y, z] = toolPaths->points[pathIndex + 1];
		endPoint = Vector3(x, y, z);
		toEndVector = endPoint - lastPosition;
	}

	if (cancel)
	{
		isThreadRunning = false;
		return false;
	}

	Vector3 direction = toEndVector;
	direction.Normalize();
	toPoint = lastPosition + direction * distance;

	drawLine(lastPosition, toPoint);

	isThreadRunning = false;
	return false;
}

void MillingSimulator::SetSpeed(float speed)
{
	this->speed = speed;
}

void MillingSimulator::Stop()
{
	if (isRunning)
	{
		cancelTask = true;
		if (futureMill.valid())
		{
			futureMill.wait();
			auto result = futureMill.get();
		}

		isRunning = false;
	}
}

void MillingSimulator::Start()
{
	if (!isRunning)
	{
		timeDelay = 0.0f;
		cancelTask = false;
		StartMilling();
		isRunning = true;

	}
}

bool MillingSimulator::IsRunning()
{
	return isRunning;
}

std::string MillingSimulator::GetErrorMessages()
{
	return errorMessages;
}

void MillingSimulator::Update(float dt)
{
	if (isRunning)
	{
		timeDelay += dt;
		if (this->isThreadRunning)
		{

		}
		else
		{
			auto done = futureMill.valid() && futureMill.get();
			if (done)
			{
				isRunning = false;
			}
			else
			{
				auto distance = timeDelay * speed;

				timeDelay = 0.0f;

				isThreadRunning = true;
				futureMill = std::async(std::launch::async, &MillingSimulator::Mill, this, distance, std::ref(cancelTask));
			}
		}
	}
}
