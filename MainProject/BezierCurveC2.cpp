#include "BezierCurveC2.h"
#include "imgui.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

BezierCurveC2::BezierCurveC2(std::vector<std::shared_ptr<Point>> points)
	:IBezierCurve(points, "Bezier Curve C2")
{

}

void BezierCurveC2::Draw(std::shared_ptr<Camera> camera)
{
	if (resetDrawing)
	{
		UpdateBezierPoints();
	}
	
	IBezierCurve::Draw(camera);

	if (bezierRepresentation)
	{
		for (int i = 0; i < bezierPoints.size(); i++)
		{
			bezierPoints[i]->Draw(camera);
		}
	}
	else
	{
		for (int i = 0; i < points.size(); i++)
		{
			points[i]->Draw(camera);
		}
	}
}

void BezierCurveC2::SetRepresentation(bool bezier)
{
	this->bezierRepresentation = bezier;
	for (auto pointPtr : points)
	{
		pointPtr->SetVisible(!bezier);
	}
	ResetDrawing();
}

void BezierCurveC2::RenderGUI()
{
	IBezierCurve::RenderGUI();

	bool rep = bezierRepresentation;
	if (ImGui::Checkbox("Bezier Representation", &rep))
	{
		SetRepresentation(rep);
	}
}

inline Vector3 GetMidPoint(Vector3 a, Vector3 b)
{
	return (a + b) / 2.0f;
}

inline Vector3 GetOneThirdPoint(Vector3 a, Vector3 b)
{
	return a + (b - a) / 3.0f;
}

inline Vector3 GetTwoThirdPoint(Vector3 a, Vector3 b)
{
	return GetOneThirdPoint(b, a);
}

void BezierCurveC2::UpdateBezierPoints()
{
	if (points.size() < 4)
	{
		bezierPoints.resize(0);
		return;
	}
	int bezierPointsCount = 1 + (points.size() - 3) * 3;
	bezierPoints.resize(bezierPointsCount);
	std::vector<Vector3> deBoorePoints(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		auto p = points[i]->GetTranslation();
		deBoorePoints[i] = Vector3(p);
	}
	auto v1 = GetTwoThirdPoint(deBoorePoints[0], deBoorePoints[1]);
	auto v3 = GetOneThirdPoint(deBoorePoints[1], deBoorePoints[2]);
	auto v2 = GetMidPoint(v1, v3);

	bezierPoints[0] = std::make_shared<VirtualPoint>(v2);
	int j = 1;
	for (int i = 3; i < deBoorePoints.size(); i++)
	{
		bezierPoints[j++] = std::make_shared<VirtualPoint>(v3);
		v1 = GetTwoThirdPoint(deBoorePoints[i - 2], deBoorePoints[i - 1]);
		v3 = GetOneThirdPoint(deBoorePoints[i - 1], deBoorePoints[i]);
		v2 = GetMidPoint(v1, v3);
		bezierPoints[j++] = std::make_shared<VirtualPoint>(v1);
		bezierPoints[j++] = std::make_shared<VirtualPoint>(v2);
	}
	for (int i = 0; i < bezierPoints.size(); i++)
	{
		auto point = bezierPoints[i];
		point->onModelChangeCallback.Add(
			[this, i](std::shared_ptr<Point> point)
			{this->OnBezierPointChange(i, point->GetTranslation()); } ,id );
	}
}

void BezierCurveC2::OnBezierPointChange(int pointIndex, Vector4 translation)
{
	int closestDeBooreIndex = (pointIndex + 1) / 3 + 1;
	int delta = 0;
	if ((pointIndex % 3) == 1) // po prawo od bli¿szego 
	{
		delta = 1;	}
	else if ((pointIndex % 3) == 2) // po lewo od bli¿szego 
	{
		delta = -1;
	}
	else // punkt œrodkowy
	{
		auto A = Vector3(points[closestDeBooreIndex - 1]->GetTranslation());
		auto B = Vector3(points[closestDeBooreIndex + 1]->GetTranslation());
		auto X = Vector3(translation);
		auto S = -B / 4.0f - A / 4.0f + 3 * X / 2;

		points[closestDeBooreIndex]->SetTranslation(S.x, S.y, S.z);
		return;
	}
	int farDeBooreIndex = closestDeBooreIndex + delta;
	auto farPos = Vector3(points[farDeBooreIndex]->GetTranslation());
	auto fromFarToNew = Vector3(translation) - farPos;
	auto closeDeBooreTrans = farPos + fromFarToNew * 3.0f / 2.0f;
	points[closestDeBooreIndex]->SetTranslation(closeDeBooreTrans.x, closeDeBooreTrans.y, closeDeBooreTrans.z);
}


std::shared_ptr<IModel> BezierCurveC2::SelectFromScreenCoords(float x, float y, DirectX::SimpleMath::Matrix VP)
{
	if (bezierRepresentation)
	{
		for (auto point : bezierPoints)
		{
			auto modelPtr = point->SelectFromScreenCoords(x, y, VP);
			if (modelPtr)
			{
				return modelPtr;
			}
		}
	}

	return nullptr;
}

std::vector<DirectX::SimpleMath::Vector3> BezierCurveC2::GetBezierPoints()
{
	std::vector<Vector3> bezier(bezierPoints.size());
	for (int i = 0; i < bezierPoints.size(); i++)
	{
		bezier[i] = Vector3(bezierPoints[i]->GetTranslation());
	}
	return bezier;
}

std::vector<DirectX::SimpleMath::Vector3> BezierCurveC2::GetPolygonChainPoints()
{
	if (bezierRepresentation)
	{
		return GetBezierPoints();
	}
	else
	{
		std::vector<Vector3> polyPoints(points.size());
		for (int i = 0; i < points.size(); i++)
		{
			polyPoints[i] = Vector3(points[i]->GetTranslation());
		}
		return polyPoints;
	}
}

void BezierCurveC2::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<BezierCurveC2>());
}

