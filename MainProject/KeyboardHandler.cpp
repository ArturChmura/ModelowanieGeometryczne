#include "KeyboardHandler.h"
#include "ImGui/imgui.h"

KeyboardHandler::KeyboardHandler(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
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
	
}
