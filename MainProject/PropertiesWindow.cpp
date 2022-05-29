#include "PropertiesWindow.h"
#include "imgui.h"

PropertiesWindow::PropertiesWindow(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

void PropertiesWindow::Render()
{
	ImGui::Begin("Properties");
	if (scene->composite)
	{
		scene->composite->RenderGUI();
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
