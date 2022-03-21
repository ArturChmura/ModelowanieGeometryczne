#include "PerspectiveCameraOptionsWindow.h"
#include "ImGui/imgui.h"

PerspectiveCameraOptionsWindow::PerspectiveCameraOptionsWindow(std::shared_ptr<PerspectiveCamera> perspectiveCamera)
{
	this->perspectiveCamera = perspectiveCamera;
	this->aspectRatio = perspectiveCamera->GetAspectRatio();
	this->fieldOfView = perspectiveCamera->GetFieldOfView();
	this->nearZ = perspectiveCamera->GetNearZ();
	this->farZ = perspectiveCamera->GetFarZ();
}

void PerspectiveCameraOptionsWindow::Render()
{
	ImGui::Begin("Perspective Camera Options");
	if (ImGui::SliderFloat("Field of View", &fieldOfView, 0.1f, 3.1415))
	{
		perspectiveCamera->SetFieldOfView(fieldOfView);
	}
	if (ImGui::SliderFloat("AspectRatio", &aspectRatio, 0.1, 10.0f))
	{
		perspectiveCamera->SetAspectRatio(aspectRatio);
	}
	if (ImGui::SliderFloat("Near Z", &nearZ, 1.0f, farZ))
	{
		perspectiveCamera->SetNearZ(nearZ);
	}
	if (ImGui::SliderFloat("Far Z", &farZ, 1.0f, 1000.0f))
	{
		perspectiveCamera->SetFarZ(farZ);
	}
	ImGui::End();
}

