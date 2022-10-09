#include "SimulationSceneStartWindow.h"
#include "imgui.h"
#include <nfd.h>

SimulationSceneStartWindow::SimulationSceneStartWindow(std::shared_ptr<SimulationScene> scene)
{
	this->scene = scene;
}

void SimulationSceneStartWindow::Render()
{
	ImGui::Begin("Simulation Scene");

	if (ImGui::Button("Load Paths"))
	{
		auto paths = std::make_shared<ToolPaths>();
		scene->AddToolPaths(paths);
	}

	
	ImGui::End();
}
