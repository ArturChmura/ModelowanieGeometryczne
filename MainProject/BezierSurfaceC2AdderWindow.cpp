#include "BezierSurfaceC2AdderWindow.h"
#include "BezierSurfaceC2.h"
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

std::tuple<std::shared_ptr<IModel>, std::vector<std::vector<std::shared_ptr<Point>>>> BezierSurfaceC2AdderWindow::GetModel()
{
	float x = width;
	float y = height;
	Vector3 center = scene->cursor->GetTranslation();

	int pointsHorizontalCount = horizontalSlicesCount + 3;
	int pointsVerticalCount = cylinder ? verticalSlicesCount : verticalSlicesCount + 3;
	auto points = std::vector<std::vector<std::shared_ptr<Point>>>(pointsVerticalCount);
	for (int i = 0; i < pointsVerticalCount; i++)
	{
		points[i] = std::vector<std::shared_ptr<Point>>(pointsHorizontalCount);
	}

	for (int i = 0; i < pointsVerticalCount; i++)
	{
		for (int j = 0; j < pointsHorizontalCount; j++)
		{
			Vector3 position;
			if (cylinder)
			{
				float dx = x / (pointsHorizontalCount - 3);
				float lx = -dx * (pointsHorizontalCount - 1) / 2.0f;
				float xx = lx + j * dx;
				float alphaStep = DirectX::XM_2PI / pointsVerticalCount;
				float yy = sinf(alphaStep * i) * y;
				float zz = cosf(alphaStep * i) * y;
				position = Vector3(xx, yy, zz) + center;
			}
			else
			{
				float dx = x / (pointsHorizontalCount - 3);
				float dy = y / (pointsVerticalCount - 3);
				float lx = -dx * (pointsHorizontalCount - 1) / 2.0f;
				float by = -dy * (pointsVerticalCount - 1) / 2.0f;
				position = Vector3(lx + j * dx, by + i * dy, 0);
				position += center;
			}
			auto point = std::make_shared<Point>(position);
			points[i][j] = point;
		}
	}

	std::vector<std::shared_ptr<SingleBezierSurfaceC2>> singleSurfaces;
	for (int i = 0; i < verticalSlicesCount; i++)
	{
		for (int j = 0; j < horizontalSlicesCount; j++)
		{
			const int m = pointsVerticalCount;
			std::array<std::array<std::shared_ptr<Point>, 4>, 4> singleSurfacePoints =
			{ {
				{points[(i + 0) % m][j], points[(i + 0) % m][j + 1], points[(i + 0) % m][j + 2], points[(i + 0) % m][j + 3]},
				{points[(i + 1) % m][j], points[(i + 1) % m][j + 1], points[(i + 1) % m][j + 2], points[(i + 1) % m][j + 3]},
				{points[(i + 2) % m][j], points[(i + 2) % m][j + 1], points[(i + 2) % m][j + 2], points[(i + 2) % m][j + 3]},
				{points[(i + 3) % m][j], points[(i + 3) % m][j + 1], points[(i + 3) % m][j + 2], points[(i + 3) % m][j + 3]}
			} };
			auto singleSurface = std::make_shared<SingleBezierSurfaceC2>(singleSurfacePoints, 10, 10);
			singleSurfaces.push_back(singleSurface);
		}
	}


	auto surface = std::make_shared<BezierSurfaceC2>(singleSurfaces);
	return std::make_tuple(surface,points);
}