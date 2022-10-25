#include "SimulationSceneStartWindow.h"
#include "imgui.h"
#include <nfd.h>
#include "ToolPathsReader.h"
#include "FlatCutter.h"
#include "SphereCutter.h"
#include <regex>
#include <future>
#include "FutureExtensions.h"

SimulationSceneStartWindow::SimulationSceneStartWindow(std::shared_ptr<SimulationScene> scene, std::function<void(std::shared_ptr<ISimulation>)> setSimulation)
{
	this->scene = scene;
	this->setSimulation = setSimulation;

	if (selectedCutterIndex == 0)
	{
		auto cutter = std::make_shared<FlatCutter>(cutterRadius, cutterCuttingPartHeight);
		this->scene->AddCutter(cutter);
	}
	else if (selectedCutterIndex == 1)
	{
		auto cutter = std::make_shared<SphereCutter>(cutterRadius, cutterCuttingPartHeight);
		this->scene->AddCutter(cutter);
	}
}

void SimulationSceneStartWindow::Render()
{
	ImGui::Begin("Simulation Scene");

	bool isRunning = millingSimulator && millingSimulator->IsRunning();
	if (isRunning)
		ImGui::BeginDisabled();

	if (!inputFileErrorMessage.empty())
	{
		ImGui::Text(inputFileErrorMessage.c_str());
	}

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
				inputFileErrorMessage = "";

				std::string fileName = outPath;
				auto dotIndex = fileName.find_last_of('.');
				if (dotIndex >= 0)
				{
					auto prefix = fileName.substr(dotIndex + 1);
					std::smatch sm;
					std::regex x_expr("[kf]\\d{2,}");
					if (std::regex_match(prefix, sm, x_expr))
					{
						char type = prefix[0];
						std::string radiusString = prefix.substr(1);
						auto radius = std::stoi(radiusString);
						this->cutterRadius = radius;
						if (type == 'f')
						{
							auto cutter = std::make_shared<FlatCutter>(radius, cutterCuttingPartHeight);
							this->scene->AddCutter(cutter);
							selectedCutterIndex = 0;
						}
						else if (type == 'k')
						{
							auto cutter = std::make_shared<SphereCutter>(radius, cutterCuttingPartHeight);
							this->scene->AddCutter(cutter);
							selectedCutterIndex = 1;
						}

					}
				}
			}
			catch (const std::exception& e)
			{
				auto a = e.what();
				inputFileErrorMessage = a;
				ImGui::OpenPopup(a);
			}

		}
		free(outPath);

	}

	if (ImGui::DragInt("Grid width count", &gridWidthCount, 1, 2, INT_MAX))
	{

	}
	if (ImGui::DragInt("Grid length count", &gridLengthCount, 1, 2, INT_MAX))
	{

	}
	if (ImGui::DragFloat("Width", &widthSize, 1, 1, FLT_MAX))
	{

	}
	if (ImGui::DragFloat("Length", &lengthSize, 1, 1, FLT_MAX))
	{

	}
	if (ImGui::DragFloat("Height", &heightSize, 1, 1, FLT_MAX))
	{

	}
	if (ImGui::DragFloat("Minimum base height", &minimumBaseHeight, 1, 0.01, FLT_MAX))
	{
		if (scene->blockModel)
		{
			scene->blockModel->SetMinHeight(minimumBaseHeight);
		}
	}

	if (ImGui::Button("New block"))
	{
		auto blockModel = std::make_shared<BlockModel>(widthSize, lengthSize, heightSize, gridWidthCount, gridLengthCount, minimumBaseHeight);
		scene->AddBlockModel(blockModel);
	}


	const char* items[] = { "Flat cutter", "Sphere cutter" };
	if (ImGui::Combo("Cutter type", &selectedCutterIndex, items, IM_ARRAYSIZE(items)))
	{
		if (selectedCutterIndex == 0)
		{
			auto cutter = std::make_shared<FlatCutter>(cutterRadius, cutterCuttingPartHeight);
			this->scene->AddCutter(cutter);
		}
		else if (selectedCutterIndex == 1)
		{
			auto cutter = std::make_shared<SphereCutter>(cutterRadius, cutterCuttingPartHeight);
			this->scene->AddCutter(cutter);
		}

	}

	if (ImGui::DragFloat("Cutter radius", &cutterRadius, 1, 0.01, FLT_MAX))
	{
		if (scene->cutter)
		{
			scene->cutter->SetRadius(cutterRadius);
		}
	}
	if (ImGui::DragFloat("Cutter cutting part height", &cutterCuttingPartHeight, 1, 0.01, FLT_MAX))
	{
		scene->cutter->SetCuttingPartHeight(cutterCuttingPartHeight);
	}

	if (isRunning)
		ImGui::EndDisabled();

	if (isRunning)
	{
		if (ImGui::Button("Cancel"))
		{
			if (this->millingSimulator)
			{
				millingSimulator->Stop();
			}
		}
	}
	else
	{
		if (ImGui::Button("Start"))
		{
			if (scene->toolPaths && scene->blockModel && scene->cutter)
			{
				auto millingSimulation = std::make_shared<MillingSimulator>(scene->toolPaths, scene->blockModel, scene->cutter, cutterSpeed);
				this->millingSimulator = millingSimulation;
				setSimulation(millingSimulation);
				millingSimulator->Start();
			}
		}
	}




	if (ImGui::DragFloat("Cutter speed", &cutterSpeed, 1, 0, FLT_MAX))
	{
		if (millingSimulator)
		{
			millingSimulator->SetSpeed(cutterSpeed);
		}
	}
	if (ImGui::Button("Skip"))
	{
		millingSimulator->SetSpeed(FLT_MAX);

	}


	if (ImGui::Checkbox("Show paths", &showPaths))
	{
		this->scene->SetShowPaths(showPaths);
	}

	if (millingSimulator)
	{
		auto errors = millingSimulator->GetErrorMessages();
		if (!errors.empty())
		{
			ImGui::Text(errors.c_str());
		}
	}


	ImGui::End();
}
