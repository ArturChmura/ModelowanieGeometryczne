#pragma once

#include "SimulationScene.h"
#include <memory>
#include "IGuiWindow.h"
#include "MillingSimulator.h"

class SimulationSceneStartWindow : public IGuiWindow
{
public:
	SimulationSceneStartWindow(std::shared_ptr<SimulationScene> scene, std::function<void(std::shared_ptr<ISimulation>)> setSimulation);
	void Render();
	std::shared_ptr<SimulationScene> scene;
private:

	int gridWidthCount = 1000;
	int gridLengthCount = 1000;
	float widthSize = 200;
	float lengthSize = 200;
	float heightSize = 40;
	float minimumBaseHeight = 5;

	float cutterRadius = 40;
	float cutterCuttingPartHeight = 40;
	float cutterSpeed = 30;
	bool showPaths = true;
	std::string inputFileErrorMessage;
	int selectedCutterIndex = 0;

	std::shared_ptr<MillingSimulator> millingSimulator;
	std::function<void(std::shared_ptr<ISimulation>)> setSimulation;

};