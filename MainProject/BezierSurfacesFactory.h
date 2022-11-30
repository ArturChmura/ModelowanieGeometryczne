#pragma once
#include "Point.h"
#include "BezierSurfaceC0.h"
#include "BezierSurfaceC2.h"

class BezierSurfacesFactory
{
public:
	static std::tuple<std::shared_ptr<BezierSurfaceC0>, std::vector<std::vector<std::shared_ptr<Point>>>> CreateBezierSurfaceC0(
		int horizontalSlicesCount,
		int verticalSlicesCount,
		float width,
		float height,
		bool cylinder,
		float xRotation = 0.0f,
		float yRotation = 0.0f,
		float zRotation = 0.0f
		);

	static std::tuple<std::shared_ptr<BezierSurfaceC2>, std::vector<std::vector<std::shared_ptr<Point>>>> CreateBezierSurfaceC2(
		int horizontalSlicesCount,
		int verticalSlicesCount,
		float width,
		float height,
		bool cylinder,
		float xRotation = 0.0f,
		float yRotation = 0.0f,
		float zRotation = 0.0f);
};