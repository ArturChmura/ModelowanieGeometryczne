#include "BezierSurfaceC0AdderWindow.h"
#include "BezierSurfaceC0.h"
#include "BezierSurfacesFactory.h"

using namespace DirectX::SimpleMath;

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

std::tuple<std::shared_ptr<IBezierSurface>, std::vector<std::vector<std::shared_ptr<Point>>>> BezierSurfaceC0AdderWindow::GetModel()
{
	return BezierSurfacesFactory::CreateBezierSurfaceC0(horizontalSlicesCount, verticalSlicesCount, width, height, cylinder);
}
