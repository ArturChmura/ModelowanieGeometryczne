#include "MouseEvents.h"
#include "ImGui/imgui.h"

MouseEvents::MouseEvents(std::shared_ptr<ArcCameraModel> camera)
{
	this->camera = camera;
}

void MouseEvents::HandleMouse()
{
	ImGuiIO& io = ImGui::GetIO();
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
