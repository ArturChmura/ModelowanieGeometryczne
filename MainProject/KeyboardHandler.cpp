#include "KeyboardHandler.h"
#include "imgui.h"

KeyboardHandler::KeyboardHandler(std::shared_ptr<ArcCameraModel> camera, std::shared_ptr<Scene> scene)
{
	this->scene = scene;
	this->camera = camera;
}

void KeyboardHandler::HandleKeyboard()
{
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::IsKeyPressed(ImGuiKey_::ImGuiKey_A) && io.KeyCtrl)
	{
		scene->AddPoint();
	}
	if (ImGui::IsKeyPressed(ImGuiKey_::ImGuiKey_Delete))
	{
		scene->RemoveSelectedModels();
	}
	if (ImGui::IsKeyPressed(ImGuiKey_::ImGuiKey_RightArrow))
	{
		camera->Move(1,0,0);
	}
	if (ImGui::IsKeyPressed(ImGuiKey_::ImGuiKey_UpArrow))
	{
		camera->Move(0, 1, 0);
	}
	if (ImGui::IsKeyPressed(ImGuiKey_::ImGuiKey_LeftArrow))
	{
		camera->Move(-1, 0, 0);
	}
	if (ImGui::IsKeyPressed(ImGuiKey_::ImGuiKey_DownArrow))
	{
		camera->Move(0, -1, 0);
	}
	
}
