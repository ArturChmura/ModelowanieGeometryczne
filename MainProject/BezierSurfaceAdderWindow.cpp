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

void BezierSurfaceAdderWindow::SetPreview()
{
	auto [model, _] = this->GetModel();
	this->scene->previewModel = model;
}

void BezierSurfaceAdderWindow::AddModel()
{
	*(this->open) = false;
	auto [model, points] = GetModel();
	this->scene->AddModel(model);
	this->scene->previewModel = nullptr;
	for (auto ps : points)
	{
		for (auto point : ps)
		{
			this->scene->AddPoint(point);
			point->onRemovedFromSceneCallback.Add([scene = scene, model = model](std::shared_ptr<Point> p) {scene->RemoveModel(model->id); }, model->id);
		}
	}
}

