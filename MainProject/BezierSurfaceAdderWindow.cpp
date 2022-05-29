#include "BezierSurfaceAdderWindow.h"
#include "imgui.h"

#include <string>

using namespace DirectX::SimpleMath;

BezierSurfaceAdderWindow::BezierSurfaceAdderWindow(std::shared_ptr<Scene> scene, bool* open)
{
	this->open = open;
	this->scene = scene;
}

BezierSurfaceAdderWindow::~BezierSurfaceAdderWindow()
{
	scene->previewModel = nullptr;
}

void BezierSurfaceAdderWindow::Render()
{
	ImGui::Begin("Add surface", open);

	if (
		ImGui::DragInt("Horizontal slices count", &horizontalSlicesCount, 1, 1, INT_MAX) ||
		ImGui::DragInt("Vertical slices count", &verticalSlicesCount, 1, cylinder ? 3:1, INT_MAX) ||
		ImGui::DragFloat("Width", &width, 0.1, 0.1, FLT_MAX) ||
		ImGui::DragFloat(cylinder ? "Radius" : "Height", &height, 0.1, 0.1, FLT_MAX) ||
		ImGui::Checkbox("Cylinder", &cylinder)
		)
	{
		horizontalSlicesCount = max(horizontalSlicesCount, 1);
		verticalSlicesCount = max(verticalSlicesCount, cylinder ? 3:1);
		width = max(width, 0.1);
		height = max(height, 0.1);
		SetPreview();
	}

	if (ImGui::Button("Dodaj model"))
	{
		AddModel();
	}

	ImGui::End();
}

