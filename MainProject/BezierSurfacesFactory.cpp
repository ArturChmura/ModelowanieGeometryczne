#include "BezierSurfacesFactory.h"
#include "CompositeModel.h"

using namespace DirectX::SimpleMath;

std::tuple<std::shared_ptr<BezierSurfaceC0>, std::vector<std::vector<std::shared_ptr<Point>>>> BezierSurfacesFactory::CreateBezierSurfaceC0(
	int horizontalSlicesCount, 
	int verticalSlicesCount, 
	float width, 
	float height, 
	bool cylinder,
	float xRotation,
	float yRotation,
	float zRotation
)
{
	float x = width;
	float y = height;
	auto center = Vector3(0, 0, 0);
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
			auto singleSurface = std::make_shared<SingleBezierSurfaceC0>(singleSurfacePoints, 20, 20, i / 3, j / 3, verticalSlicesCount, horizontalSlicesCount);
			singleSurfaces.push_back(singleSurface);
		}
	}

	auto surface = std::make_shared<BezierSurfaceC0>(singleSurfaces, horizontalSlicesCount, verticalSlicesCount, false, cylinder);

	auto composite = std::make_shared<CompositeModel>();
	for (auto pointsRow : points)
	{
		for (auto point : pointsRow)
		{
			composite->AddModel(point);
		}
	}
	composite->SetRotation(xRotation, yRotation, zRotation);

	return std::make_tuple(surface, points);
}

std::tuple<std::shared_ptr<BezierSurfaceC2>, std::vector<std::vector<std::shared_ptr<Point>>>> BezierSurfacesFactory::CreateBezierSurfaceC2(
	int horizontalSlicesCount, 
	int verticalSlicesCount, 
	float width, 
	float height, 
	bool cylinder,
	float xRotation,
	float yRotation,
	float zRotation)
{
	float x = width;
	float y = height;
	auto center = Vector3(0, 0, 0);

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
			auto singleSurface = std::make_shared<SingleBezierSurfaceC2>(singleSurfacePoints, 20, 20, i, j, verticalSlicesCount, horizontalSlicesCount);
			singleSurfaces.push_back(singleSurface);
		}
	}


	auto surface = std::make_shared<BezierSurfaceC2>(singleSurfaces, horizontalSlicesCount, verticalSlicesCount, false, cylinder);

	auto composite = std::make_shared<CompositeModel>();
	for (auto pointsRow : points)
	{
		for (auto point : pointsRow)
		{
			composite->AddModel(point);
		}
	}
	composite->SetRotation(xRotation, yRotation, zRotation);

	return std::make_tuple(surface, points);
}
