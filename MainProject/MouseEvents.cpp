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

	if (ImGui::IsMouseDown(2))
	{
		float x = io.MouseDelta.x;
		float y = io.MouseDelta.y;
		if (x != 0 || y != 0)
		{
			camera->Rotate(x * -rotationSpeedPerPixel, y * -rotationSpeedPerPixel);
		}
	}
	if (ImGui::IsMouseDown(1))
	{
		float y = io.MouseDelta.y;
		if (y != 0)
		{
			camera->ChangeDistance(y);
		}
	}
}
