#include "MouseEvents.h"
#include "ImGui/imgui.h"


MouseEvents::MouseEvents(std::shared_ptr<ArcCameraModel> camera, std::shared_ptr<Scene> scene)
{
	this->camera = camera;
	this->scene = scene;
}

void MouseEvents::HandleMouse()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse == true)
	{
		return;
	}
	float x = io.MouseDelta.x;
	if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_R))
	{
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_X))
		{
			scene->selectedModel->Rotate(x * -objectRotationSpeedPerPixel, 0, 0);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_Y))
		{
			scene->selectedModel->Rotate(0, x * -objectRotationSpeedPerPixel, 0);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_Z))
		{
			scene->selectedModel->Rotate(0, 0, x * -objectRotationSpeedPerPixel);
		}
	}
	if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_S))
	{
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_X))
		{
			scene->selectedModel->Scale(1.0f + x * objectScaleSpeedPerPixel, 1, 1);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_Y))
		{
			scene->selectedModel->Scale(1, 1.0f + x * objectScaleSpeedPerPixel, 1);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_Z))
		{
			scene->selectedModel->Scale(1, 1, 1.0f + x * objectScaleSpeedPerPixel);
		}
	}
	if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_T))
	{
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_X))
		{
			scene->selectedModel->Translate(x * objectTranslateSpeedPerPixel, 0, 0);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_Y))
		{
			scene->selectedModel->Translate(0, x * objectTranslateSpeedPerPixel, 0);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_Z))
		{
			scene->selectedModel->Translate(0, 0, x * objectTranslateSpeedPerPixel);
		}
	}
	if (ImGui::IsMouseDown(2))
	{
		float x = io.MouseDelta.x;
		float y = io.MouseDelta.y;
		if (x != 0 || y != 0)
		{
			camera->Rotate(x * -cameraRotationSpeedPerPixel, y * -cameraRotationSpeedPerPixel);
		}
	}
	if (ImGui::IsMouseDown(1))
	{
		float y = io.MouseDelta.y;
		if (y != 0)
		{
			camera->ChangeDistance(y* cameraMoveSpeedPerPixel);
		}
	}
}
