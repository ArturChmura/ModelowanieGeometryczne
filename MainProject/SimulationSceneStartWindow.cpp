#include "SimulationSceneStartWindow.h"
#include "imgui.h"
#include <nfd.h>
#include "ToolPathsReader.h"

SimulationSceneStartWindow::SimulationSceneStartWindow(std::shared_ptr<SimulationScene> scene)
{
	this->scene = scene;
}

void SimulationSceneStartWindow::Render()
{
	ImGui::Begin("Simulation Scene");

	if (ImGui::Button("Load Paths"))
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
		if (result == NFD_OKAY) {
			try
			{
				auto toolPaths = ToolPathsReader::ReadFromFile(outPath);
				scene->AddToolPaths(toolPaths);
				ImGui::OpenPopup("Success paths");
			}
			catch (const std::exception& e)
			{
				auto a = e.what();
				ImGui::OpenPopup(a);
			}

		}
		free(outPath);

	}

	
	ImGui::End();
}
