#include "SimulationScene.h"

SimulationScene::SimulationScene()
	: IScene("Simulation Scene")
{
}

void SimulationScene::AddToolPaths(std::shared_ptr<ToolPaths> toolPaths)
{
	this->toolPaths = toolPaths;
}

void SimulationScene::DrawScene()
{
	if (this->toolPaths)
	{
		this->toolPaths->Draw(activeCamera);
	}
}
