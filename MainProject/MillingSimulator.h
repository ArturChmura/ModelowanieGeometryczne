#pragma once

#include "ToolPaths.h"
#include "BlockModel.h"
#include "SimpleMath.h"


class MillingSimulator
{
public:
	MillingSimulator(std::shared_ptr<ToolPaths> toolPaths, std::shared_ptr<BlockModel> blockModel, std::shared_ptr<ICutter> cutter, float speed);

	void StartMilling();
	bool Mill(float distance, bool & cancel);
	void SetSpeed(float speed);

private:

	std::shared_ptr<ToolPaths> toolPaths;
	std::shared_ptr<BlockModel> blockModel;
	float speed;

	int pathIndex;
	DirectX::SimpleMath::Vector3 lastPosition;
	std::shared_ptr<ICutter> cutter;
};