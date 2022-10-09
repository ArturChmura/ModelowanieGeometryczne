#pragma once

#include "SimulationScene.h"
#include <memory>
#include "IGuiWindow.h"

class SimulationSceneStartWindow : public IGuiWindow
{
public:
	SimulationSceneStartWindow(std::shared_ptr<SimulationScene> scene);
	void Render();
	std::shared_ptr<SimulationScene> scene;
private:
};