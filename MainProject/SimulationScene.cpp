#include "SimulationScene.h"

SimulationScene::SimulationScene()
	: IScene("Simulation Scene")
{
	this->cursor = std::make_shared<Cursor3d>();
}

void SimulationScene::AddToolPaths(std::shared_ptr<ToolPaths> toolPaths)
{
	this->toolPaths = toolPaths;
}

void SimulationScene::AddBlockModel(std::shared_ptr<BlockModel> blockModel)
{
	this->blockModel = blockModel;
}

void SimulationScene::AddSimulator(std::shared_ptr<MillingSimulator> millingSimulator)
{
	this->millingSimulator = millingSimulator;
}

void SimulationScene::AddCutter(std::shared_ptr<ICutter> cutter)
{
	this->cutter = cutter;
}

void SimulationScene::DrawScene()
{
	if (this->millingSimulator)
	{
		millingSimulator->Mill();
		millingSimulator->Draw(activeCamera);
	}
	if (this->toolPaths)
	{
		this->toolPaths->Draw(activeCamera);
	}
	if (this->blockModel)
	{
		this->blockModel->Draw(activeCamera);
	}

	this->cursor->Draw(activeCamera);
}
