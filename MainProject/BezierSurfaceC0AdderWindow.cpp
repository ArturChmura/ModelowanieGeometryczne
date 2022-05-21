#include "BezierSurfaceC0AdderWindow.h"
#include "BezierSurfaceC0.h"

BezierSurfaceC0AdderWindow::BezierSurfaceC0AdderWindow(std::shared_ptr<Scene> scene, bool* open)
	:BezierSurfaceAdderWindow(scene, open)
{
	SetPreview();

	scene->cursor->OnPositionChangeCallback.Add([this](DirectX::SimpleMath::Vector3 v) {this->SetPreview(); }, "BezierSurfaceC0AdderWindow");
}

BezierSurfaceC0AdderWindow::~BezierSurfaceC0AdderWindow()
{
	scene->cursor->OnPositionChangeCallback.Remove("BezierSurfaceC0AdderWindow");
}


void BezierSurfaceC0AdderWindow::AddModel()
{
	*(this->open) = false;
	auto surface = std::make_shared<BezierSurfaceC0>(horizontalSlicesCount, verticalSlicesCount, width, height, cylinder, scene->cursor->GetTranslation());
	this->scene->AddModel(surface);
	this->scene->previewModel = nullptr;
	for (auto points : surface->GetPoints())
	{
		for (auto point : points)
		{
			this->scene->AddPoint(point);
			point->onRemovedFromSceneCallback.Add([scene = scene, surface = surface](std::shared_ptr<Point> p) {scene->DeleteModel(surface->id); }, surface->id);
		}
	}
}

void BezierSurfaceC0AdderWindow::SetPreview()
{
	auto surface = std::make_shared<BezierSurfaceC0>(horizontalSlicesCount, verticalSlicesCount, width, height, cylinder, scene->cursor->GetTranslation());
	this->scene->previewModel = surface;
}
