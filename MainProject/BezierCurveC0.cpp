#include "BezierCurveC0.h"
#include <algorithm>
#include <d3d11.h>
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

std::vector<DirectX::SimpleMath::Vector3> BezierCurveC0::GetPolygonChainPoints()
{
	return GetBezierPoints();
}

void BezierCurveC0::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<BezierCurveC0>());
}

