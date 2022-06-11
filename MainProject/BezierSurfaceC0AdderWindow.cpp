#include "BezierSurfaceC0AdderWindow.h"
#include "BezierSurfaceC0.h"
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

std::tuple<std::shared_ptr<IModel>, std::vector<std::vector<std::shared_ptr<Point>>>> BezierSurfaceC0AdderWindow::GetModel()
{
	float x = width;
	float y = height;
	Vector3 center = scene->cursor->GetTranslation();

	int pointsHorizontalCount = horizontalSlicesCount * 3 + 1;
	int pointsVerticalCount = cylinder ? verticalSlicesCount * 3 : verticalSlicesCount * 3 + 1;

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
				float xx = -x / 2.0f + j * (x / (pointsHorizontalCount - 1));
				float alphaStep = DirectX::XM_2PI / pointsVerticalCount;
				float yy = sinf(alphaStep * i) * y;
				float zz = cosf(alphaStep * i) * y;
				position = Vector3(xx, yy, zz) + center;
			}
			else
			{
				Vector3 leftBottom = Vector3(-x / 2.0f + j * (x / (pointsHorizontalCount - 1)), -y / 2.0f + i * (y / (pointsVerticalCount - 1)), 0);
				leftBottom += center;
				position = leftBottom;
			}
			auto point = std::make_shared<Point>(position);
			points[i][j] = point;
		}
	}
	std::vector<std::shared_ptr<SingleBezierSurfaceC0>> singleSurfaces;

	for (int i = 0; i < pointsVerticalCount - 1; i += 3)
	{
		for (int j = 0; j < pointsHorizontalCount - 1; j += 3)
		{
			std::array<std::array<std::shared_ptr<Point>, 4>, 4> singleSurfacePoints =
			{ {
				{points[i + 0][j], points[i + 0][j + 1], points[i + 0][j + 2], points[i + 0][j + 3]},
				{points[i + 1][j], points[i + 1][j + 1], points[i + 1][j + 2], points[i + 1][j + 3]},
				{points[i + 2][j], points[i + 2][j + 1], points[i + 2][j + 2], points[i + 2][j + 3]},
				{points[(i + 3) % pointsVerticalCount][j], points[(i + 3) % pointsVerticalCount][j + 1], points[(i + 3) % pointsVerticalCount][j + 2], points[(i + 3) % pointsVerticalCount][j + 3]}
			} };
			auto singleSurface = std::make_shared<SingleBezierSurfaceC0>(singleSurfacePoints, 10, 10);
			singleSurfaces.push_back(singleSurface);
		}
	}

	auto surface = std::make_shared<BezierSurfaceC0>(singleSurfaces, horizontalSlicesCount,verticalSlicesCount, cylinder);
	
	return std::make_tuple(surface, points);
}
