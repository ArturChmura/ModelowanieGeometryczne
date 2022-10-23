#include "SimulationSceneStartWindow.h"
#include "imgui.h"
#include <nfd.h>
#include "ToolPathsReader.h"
#include "FlatCutter.h"
#include "SphereCutter.h"
#include <regex>

SimulationSceneStartWindow::SimulationSceneStartWindow(std::shared_ptr<SimulationScene> scene)
{
	this->scene = scene;

	if (selectedCutterIndex == 0)
	{
		auto cutter = std::make_shared<FlatCutter>(cutterRadius);
		this->scene->AddCutter(cutter);
	}
	else if (selectedCutterIndex == 1)
	{
		auto cutter = std::make_shared<SphereCutter>(cutterRadius);
		this->scene->AddCutter(cutter);
	}
}

void SimulationSceneStartWindow::Render()
{
	ImGui::Begin("Simulation Scene");

	if (isMilling)
		ImGui::BeginDisabled();

	if (!errorMessage.empty())
	{
		ImGui::Text(errorMessage.c_str());
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
				errorMessage = "";

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
							auto cutter = std::make_shared<FlatCutter>(radius);
							this->scene->AddCutter(cutter);
							selectedCutterIndex = 0;
						}
						else if (type == 'k')
						{
							auto cutter = std::make_shared<SphereCutter>(radius);
							this->scene->AddCutter(cutter);
							selectedCutterIndex = 1;
						}

					}
				}
			}
			catch (const std::exception& e)
			{
				auto a = e.what();
				errorMessage = a;
				ImGui::OpenPopup(a);
			}

		}
		free(outPath);

	}

	if (ImGui::DragInt("Grid width count", &gridWidthCount, 1, 2, INT_MAX))
	{

	}
	if (ImGui::DragInt("Grid length count", &gridLengthCount, 1,  2, INT_MAX))
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

	if (ImGui::Button("New block"))
	{
		auto blockModel = std::make_shared<BlockModel>(widthSize,lengthSize,heightSize,gridWidthCount, gridLengthCount);
		scene->AddBlockModel(blockModel);
	}


	const char* items[] = { "Flat cutter", "Sphere cutter"};
	if (ImGui::Combo("Cutter type", &selectedCutterIndex, items, IM_ARRAYSIZE(items)))
	{
		if (selectedCutterIndex == 0)
		{
			auto cutter = std::make_shared<FlatCutter>(cutterRadius);
			this->scene->AddCutter(cutter);
		}
		else if(selectedCutterIndex == 1)
		{
			auto cutter = std::make_shared<SphereCutter>(cutterRadius);
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

	if (isMilling)
		ImGui::EndDisabled();

	if (ImGui::Button(isMilling ? "End" : "Start"))
	{
		if (isMilling)
		{
			this->millingSimulator = nullptr;
			isMilling = false;
		}
		else
		{
			if (scene->toolPaths && scene->blockModel && scene->cutter)
			{
				auto millingSimulator = std::make_shared<MillingSimulator>(scene->toolPaths, scene->blockModel, scene->cutter, cutterSpeed);
				this->millingSimulator = millingSimulator;
				millingSimulator->StartMilling();
				isMilling = true;
			}
		}
	}

	if (isMilling && this->millingSimulator)
	{
		millingSimulator->Mill();
	}

	if (ImGui::DragFloat("Cutter speed", &cutterSpeed, 1, 0, FLT_MAX))
	{
		if (millingSimulator)
		{
			millingSimulator->SetSpeed(cutterSpeed);
		}
	}

	ImGui::End();
}
