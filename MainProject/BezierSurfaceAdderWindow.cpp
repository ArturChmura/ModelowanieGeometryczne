#include "BezierSurfaceAdderWindow.h"
#include "ImGui/imgui.h"

#include <string>

using namespace DirectX::SimpleMath;

BezierSurfaceAdderWindow::BezierSurfaceAdderWindow(std::shared_ptr<Scene> scene, bool* open)
{
	this->open = open;
	this->scene = scene;
	SetPreview();

	scene->cursor->OnPositionChangeCallback.Add([this](DirectX::SimpleMath::Vector3 v) {this->SetPreview(); }, "BezierSurfaceAdderWindow");
}

BezierSurfaceAdderWindow::~BezierSurfaceAdderWindow()
{
	scene->cursor->OnPositionChangeCallback.Remove("BezierSurfaceAdderWindow");
	scene->previewModel = nullptr;
}

void BezierSurfaceAdderWindow::Render()
{
	ImGui::Begin("Add surface C0", open);

	if (
		ImGui::DragInt("Horizontal slices count", &horizontalSlicesCount, 1, 1, INT_MAX) ||
		ImGui::DragInt("Vertical slices count", &verticalSlicesCount, 1, 1, INT_MAX) ||
		ImGui::DragFloat("Width", &width, 0.1, 0.1, FLT_MAX) ||
		ImGui::DragFloat(cylinder ? "Radius" : "Height", &height, 0.1, 0.1, FLT_MAX) ||
		ImGui::Checkbox("Cylinder", &cylinder)
		)
	{
		horizontalSlicesCount = max(horizontalSlicesCount, 1);
		verticalSlicesCount = max(verticalSlicesCount, 1);
		width = max(width, 0.1);
		height = max(height, 0.1);
		SetPreview();
	}

	if (ImGui::Button("Dodaj model"))
	{
		*(this->open) = false;
		this->scene->AddModel(surface);
		this->scene->previewModel = nullptr;
		for (auto points : this->surface->GetPoints())
		{
			for (auto point : points)
			{
				this->scene->AddPoint(point);
				point->onRemovedFromSceneCallback.Add([scene = scene, surface = surface](std::shared_ptr<Point> p) {scene->DeleteModel(surface->id); }, surface->id);
			}
		}
	}

	ImGui::End();
}

void BezierSurfaceAdderWindow::SetPreview()
{
	this->surface =  std::make_shared<BezierSurfaceC0>(horizontalSlicesCount, verticalSlicesCount, width, height, cylinder, scene->cursor->GetTranslation());
	this->scene->previewModel = surface;
}
