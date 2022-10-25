#pragma once

#include "SimulationScene.h"
#include <memory>
#include "IGuiWindow.h"
#include <chrono>
#include <future>

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
	bool showPaths = true;
	std::string errorMessage;
	int selectedCutterIndex = 0;
	bool isMilling = false;

	std::shared_ptr< MillingSimulator> millingSimulator;

	std::chrono::steady_clock::time_point lastFrameTimePoint;

	bool cancelTask = false;
	std::future<bool> futureMill;
};