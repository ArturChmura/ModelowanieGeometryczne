#include "StartWindow.h"
#include "imgui.h"
#include <nfd.h>
#include <Serializer.h>

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
			puts("Success!");
			puts(outPath);

			SerializationVisitor serializer;

			MG1::Scene::Get().Clear();
			for (auto model : scene->models)
			{
				serializer.Visit(*model);
			}
			

			MG1::SceneSerializer sceneSerializer;

			sceneSerializer.SaveScene(outPath);
		}
		else if (result == NFD_CANCEL) {
			puts("User pressed cancel.");
		}
		else {
			printf("Error: %s\n", NFD_GetError());
		}
		free(outPath);
	}

	if (ImGui::Button("Load Scene"))
	{

	}

	ImGui::End();
}
