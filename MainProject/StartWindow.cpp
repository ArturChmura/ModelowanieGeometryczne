#include "StartWindow.h"
#include "imgui.h"
#include <nfd.h>

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
		nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
		if (result == NFD_OKAY) {
			puts("Success!");
			puts(outPath);
			free(outPath);
		}
		else if (result == NFD_CANCEL) {
			puts("User pressed cancel.");
		}
		else {
			printf("Error: %s\n", NFD_GetError());
		}
	}

	if (ImGui::Button("Load Scene"))
	{

	}

	ImGui::End();
}
