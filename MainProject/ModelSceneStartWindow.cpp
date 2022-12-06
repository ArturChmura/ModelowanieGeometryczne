#include "ModelSceneStartWindow.h"
#include "imgui.h"
#include <nfd.h>
#include <Serializer.h>
#include "SceneLoader.h"
#include "SerializationVisitor.h"
#include "GeneralPathsGenerator.h"
#include "ModelFilterSelectorVisitor.h"
#include "DetailPathGenerator.h"

ModelSceneStartWindow::ModelSceneStartWindow(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

void ModelSceneStartWindow::Render()
{
	ImGui::Begin("Model Scene");

	if (ImGui::Button("Save scene"))
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_SaveDialog(NULL, NULL, &outPath);
		if (result == NFD_OKAY) {
			SerializationVisitor serializer;

			MG1::Scene::Get().Clear();
			for (auto model : scene->models)
			{
				model->Accept(serializer);
			}

			MG1::SceneSerializer sceneSerializer;
			sceneSerializer.SaveScene(outPath);

			ImGui::OpenPopup("Success save popup");
		}
		else {
			ImGui::OpenPopup("Error popup");
		}
		free(outPath);
	}

	if (ImGui::Button("Load Scene"))
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
		if (result == NFD_OKAY) {
			try
			{
				SceneLoader::LoadScene(scene, outPath);
				ImGui::OpenPopup("Success load popup");
			}
			catch (const std::exception& e)
			{
				auto a = e.what();
				ImGui::OpenPopup("Wrong input file popup");
			}

		}
		else {
			ImGui::OpenPopup("Error popup");
		}
		free(outPath);

	}
	
	if (ImGui::Button("Generate general tool paths"))
	{
		ModelFilterSelectorVisitor<Point> visitor;
		auto modelsList = visitor.GetList(scene->models);
		GeneralPathsGenerator generator;
		auto curve = generator.GeneralPaths(modelsList);
		scene->AddModel(curve);
	}

	if (ImGui::Button("Generate base tool paths"))
	{
		ModelFilterSelectorVisitor<Point> visitor;
		auto modelsList = visitor.GetList(scene->models);
		GeneralPathsGenerator generator;
		auto curve = generator.PlanePaths(modelsList);
		scene->AddModel(curve);
	}

	if (ImGui::Button("Generate border tool paths"))
	{
		ModelFilterSelectorVisitor<Point> visitor;
		auto modelsList = visitor.GetList(scene->models);
		GeneralPathsGenerator generator;
		auto curve = generator.BorderPath(modelsList, scene);
		scene->AddModel(curve);
	}

	if (ImGui::Button("Generate detail tool paths"))
	{
		ModelFilterSelectorVisitor<Point> visitor;
		auto modelsList = visitor.GetList(scene->models);
		DetailPathGenerator generator;
		auto curve = generator.GeneralPaths(modelsList, this->scene);
		scene->AddModel(curve);
	}

	if (ImGui::BeginPopup("Success load popup"))
	{
		ImGui::Text("Successfully loaded scene");
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopup("Error popup"))
	{
		ImGui::Text("Something went wrong");
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopup("Wrong input file popup"))
	{
		ImGui::Text("Input file is corrupted");
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopup("Success save popup"))
	{
		ImGui::Text("Saved to file");
		ImGui::EndPopup();
	}
	ImGui::End();
}
