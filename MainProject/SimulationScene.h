#pragma once
#include "IScene.h"
#include <memory>
#include "ToolPaths.h"

class SimulationScene : public IScene
{
public:
	SimulationScene();
	
	void AddToolPaths(std::shared_ptr<ToolPaths> toolPaths);
	void DrawScene() override;

	std::shared_ptr<ToolPaths> toolPaths;
};

