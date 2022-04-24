#include "BezierCurveC0.h"
#include <iterator>
#include "Vertex.h"
#include "ImGui/imgui.h"
#include <algorithm>
#include "SimpleMath.h"
#include "Helpers.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
BezierCurveC0::BezierCurveC0(std::vector<std::shared_ptr<Point>> points)
	:IBezierCurve(points, "Bezier Curve C0")
{
}

void BezierCurveC0::Draw(std::shared_ptr<Camera> camera)
{
	IBezierCurve::Draw(camera);
	
	for (int i = 0; i < points.size(); i++)
	{
		points[i]->Draw(camera);
	}
}

void BezierCurveC0::RenderGUI()
{
	IBezierCurve::RenderGUI();
}

std::vector<Vector3> BezierCurveC0::GetBezierPoints()
{
	std::vector<Vector3> bezier(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		bezier[i] = Vector3(points[i]->GetTranslation());
	}
	return bezier;
}


//
//
//void BezierCurveC0::UpdateSlicesCount(std::shared_ptr<Camera> camera)
//{
//	int segmentsCount = (points.size() + 1) / 3;
//	desiredSlices.resize(segmentsCount);
//	currentSlices.resize(segmentsCount);
//	auto viewMatrix = camera->GetViewMatrix();
//	auto perspectiveMatrix = camera->GetPerspectiveMatrix();
//	auto PV = viewMatrix * perspectiveMatrix;
//	std::vector<Vector2> camPositions;
//	bool isLargerNow = false;
//	for (size_t i = 0; i < points.size(); i++)
//	{
//		auto point = points[i];
//		auto translation = point->GetTranslation();
//		auto camPosition = Vector4::Transform(translation, PV);
//		camPosition /= camPosition.w;
//		Vector2 xy(
//			(camPosition.x + 1) * DxDevice::winowSize.cx / 2.0f,
//			(camPosition.y + 1) * DxDevice::winowSize.cy / 2.0f
//		);
//
//		camPositions.push_back(xy);
//	}
//	for (int i = 0; i < segmentsCount; i++)
//	{
//		int pointsCount = min(points.size() - (i * 3), 4);
//		float area = 0.0f;
//		for (int j = 0; j < pointsCount - 1; j++)
//		{
//			auto length = (camPositions[i * 3 + j] - camPositions[i * 3 + j + 1]).Length();
//			length = min(length, DxDevice::winowSize.cx * 2);
//			area += length;
//		}
//		int slices = max((int)area, 1);
//		if (currentSlices[i] < slices)
//		{
//			isLargerNow = true;
//		}
//		desiredSlices[i] = slices;
//	}
//	if (isLargerNow)
//	{
//		ResetDrawing();
//	}
//}
//
//
