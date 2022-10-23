#pragma once
#include "IScene.h"
#include <memory>
#include "ToolPaths.h"
#include "BlockModel.h"
#include "MillingSimulator.h"
#include "Cursor3d.h"

class SimulationScene : public IScene
{
public:
	SimulationScene();
	
	void AddToolPaths(std::shared_ptr<ToolPaths> toolPaths);
	void AddBlockModel(std::shared_ptr<BlockModel> blockModel);
	void AddSimulator(std::shared_ptr<MillingSimulator> millingSimulator);
	void AddCutter(std::shared_ptr<ICutter> cutter);
	void DrawScene() override;

	std::shared_ptr<ToolPaths> toolPaths;
	std::shared_ptr<BlockModel> blockModel;
	std::shared_ptr<MillingSimulator> millingSimulator;
	std::shared_ptr<ICutter> cutter;

	std::shared_ptr<Cursor3d> cursor;

};

