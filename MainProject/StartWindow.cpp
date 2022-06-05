#include "StartWindow.h"
#include "imgui.h"
#include <nfd.h>
#include <Serializer.h>
#include "SceneLoader.h"
#include "SerializationVisitor.h"

StartWindow::StartWindow(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

void StartWindow::Render()
{
	ImGui::Begin("Start");
	
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
			catch (const std::exception&)
			{
				ImGui::OpenPopup("Wrong input file popup");
			}

		}
		else {
			ImGui::OpenPopup("Error popup");
		}
		free(outPath);
		
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
