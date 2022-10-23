#pragma once

#include "ToolPaths.h"
#include "BlockModel.h"
#include <chrono>
#include "SimpleMath.h"
#include "IModel.h"


class MillingSimulator
{
public:
	MillingSimulator(std::shared_ptr<ToolPaths> toolPaths, std::shared_ptr<BlockModel> blockModel, std::shared_ptr<ICutter> cutter, float speed);

	void StartMilling();
	bool Mill();
	void SetSpeed(float speed);
	void Draw(std::shared_ptr<Camera> camera);

private:

	std::shared_ptr<ToolPaths> toolPaths;
	std::shared_ptr<BlockModel> blockModel;
	float speed;
	std::chrono::steady_clock::time_point lastFrameTimePoint;

	int pathIndex;
	DirectX::SimpleMath::Vector3 lastPosition;
	std::shared_ptr<IModel> millModel;
	std::shared_ptr<ICutter> cutter;
};