#include "BezierSurfaceC2AdderWindow.h"
#include "BezierSurfaceC2.h"
#include "BezierSurfacesFactory.h"
using namespace DirectX::SimpleMath;
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

std::tuple<std::shared_ptr<IBezierSurface>, std::vector<std::vector<std::shared_ptr<Point>>>> BezierSurfaceC2AdderWindow::GetModel()
{
	return BezierSurfacesFactory::CreateBezierSurfaceC2(horizontalSlicesCount, verticalSlicesCount, width, height, cylinder);
}