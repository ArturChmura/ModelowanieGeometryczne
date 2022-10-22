#include "SimulationScene.h"

SimulationScene::SimulationScene()
	: IScene("Simulation Scene")
{
}

void SimulationScene::AddToolPaths(std::shared_ptr<ToolPaths> toolPaths)
{
	this->toolPaths = toolPaths;
}

void SimulationScene::AddBlockModel(std::shared_ptr<BlockModel> blockModel)
{
	this->blockModel = blockModel;
}

void SimulationScene::DrawScene()
{
	if (this->toolPaths)
	{
		this->toolPaths->Draw(activeCamera);
	}
	if (this->blockModel)
	{
		this->blockModel->Draw(activeCamera);
	}
}
