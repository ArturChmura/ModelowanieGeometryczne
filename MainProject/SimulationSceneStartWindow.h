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

	int gridWidthCount = 1000;
	int gridLengthCount = 1000;
	float widthSize = 200;
	float lengthSize = 200;
	float heightSize = 40;
	float cutterRadius = 40;
	float cutterSpeed = 30;
	std::string errorMessage;
	int selectedCutterIndex = 0;
};