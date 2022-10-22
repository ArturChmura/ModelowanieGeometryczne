#pragma once
#include "IScene.h"
#include <memory>
#include "ToolPaths.h"
#include "BlockModel.h"

class SimulationScene : public IScene
{
public:
	SimulationScene();
	
	void AddToolPaths(std::shared_ptr<ToolPaths> toolPaths);
	void AddBlockModel(std::shared_ptr<BlockModel> blockModel);
	void DrawScene() override;

	std::shared_ptr<ToolPaths> toolPaths;
	std::shared_ptr<BlockModel> blockModel;
};

