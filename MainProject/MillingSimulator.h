#pragma once

#include "ToolPaths.h"
#include "BlockModel.h"
#include "SimpleMath.h"
#include "ISumulation.h"
#include <future>

class MillingSimulator: public ISimulation
{
public:
	MillingSimulator(std::shared_ptr<ToolPaths> toolPaths, std::shared_ptr<BlockModel> blockModel, std::shared_ptr<ICutter> cutter, float speed);

	virtual void Update(float dt) override;

	void SetSpeed(float speed);
	virtual void Stop() override;
	virtual void Start() override;
	virtual bool IsRunning() override;

private:

	std::shared_ptr<ToolPaths> toolPaths;
	std::shared_ptr<BlockModel> blockModel;
	std::shared_ptr<ICutter> cutter;

	float speed;
	int pathIndex;
	DirectX::SimpleMath::Vector3 lastPosition;
	float timeDelay;

	bool isRunning = false;
	bool cancelTask = false;
	std::future<bool> futureMill;


	void StartMilling();
	bool Mill(float distance, bool& cancel);

	bool isThreadRunning = false;
};