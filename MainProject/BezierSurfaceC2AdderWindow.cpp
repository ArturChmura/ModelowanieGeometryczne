#include "BezierSurfaceC2AdderWindow.h"
#include "BezierSurfaceC2.h"

BezierSurfaceC2AdderWindow::BezierSurfaceC2AdderWindow(std::shared_ptr<Scene> scene, bool* open)
	:BezierSurfaceAdderWindow(scene, open)
{
	SetPreview();

	scene->cursor->OnPositionChangeCallback.Add([this](DirectX::SimpleMath::Vector3 v) {this->SetPreview(); }, "BezierSurfaceC2AdderWindow");
}

BezierSurfaceC2AdderWindow::~BezierSurfaceC2AdderWindow()
{
	scene->cursor->OnPositionChangeCallback.Remove("BezierSurfaceC2AdderWindow");
}

void BezierSurfaceC2AdderWindow::AddModel()
{
	*(this->open) = false;
	auto surface = std::make_shared<BezierSurfaceC2>(horizontalSlicesCount, verticalSlicesCount, width, height, cylinder, scene->cursor->GetTranslation());
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

void BezierSurfaceC2AdderWindow::SetPreview()
{
	auto surface = std::make_shared<BezierSurfaceC2>(horizontalSlicesCount, verticalSlicesCount, width, height, cylinder, scene->cursor->GetTranslation());
	this->scene->previewModel = surface;
}
