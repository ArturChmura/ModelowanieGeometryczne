#include "PointsMerger.h"
#include "IBezierCurve.h"
#include "IModel.h"
#include "Point.h"
#include "Torus.h"
#include "BezierCurveC0.h"
#include "BezierCurveC2.h"
#include "BezierCurveInterpolating.h"
#include "SingleBezierSurfaceC0.h"
#include "BezierSurfaceC0.h"
#include "SingleBezierSurfaceC2.h"
#include "BezierSurfaceC2.h"
#include "CompositeModel.h"

using namespace DirectX::SimpleMath;

void PointsMerger::MergePoints(std::shared_ptr<Scene> scene)
{
	auto points = scene->GetSelectedPoints();
	Vector3 pos;
	for (auto point : points)
	{
		pos += Vector3(point->GetTranslation());
	}
	pos /= points.size();
	auto mergedPoint = std::make_shared<Point>(pos);

	this->pointsIds.clear();
	for (auto point : points)
	{
		mergedPoint->onAddedToSceneCallback.Add(point->onAddedToSceneCallback);
		mergedPoint->onModelChangeCallback.Add(point->onModelChangeCallback);
		mergedPoint->onRemovedFromSceneCallback.Add(point->onRemovedFromSceneCallback);
		mergedPoint->onSelectCallback.Add(point->onSelectCallback);
		point->onRemovedFromSceneCallback.Clear();
		this->pointsIds.insert(point->id);
	}
	this->mergedPoint = mergedPoint;

	auto allModels = scene->models;
	for (auto model : allModels)
	{
		model->Accept(*this);
	}
	
	for (auto point : points)
	{
		scene->RemoveModel(point->id);
	}

	scene->AddPoint(mergedPoint);
}

void PointsMerger::Accept(Point& point)
{
}

void PointsMerger::Accept(Torus& torus)
{
}


void PointsMerger::Accept(BezierCurveC0& curve)
{
	AcceptBezierCurve(curve);
}

void PointsMerger::Accept(BezierCurveC2& curve)
{
	AcceptBezierCurve(curve);
}

void PointsMerger::Accept(BezierCurveInterpolating& curve)
{
	AcceptBezierCurve(curve);
}

void PointsMerger::AcceptBezierCurve(IBezierCurve& curve)
{
	for (size_t i = 0; i < curve.points.size(); i++)
	{
		auto point = curve.points[i];
		if (pointsIds.contains(point->id))
		{
			curve.points[i] = mergedPoint;
		}
	}
	curve.ResetDrawing();
}

void PointsMerger::Accept(SingleBezierSurfaceC0& patch)
{
	for (size_t i = 0; i < patch.points.size(); i++)
	{
		for (size_t j = 0; j < patch.points[i].size(); j++)
		{
			auto point = patch.points[i][j];
			if (pointsIds.contains(point->id))
			{
				patch.points[i][j] = mergedPoint;
			}
		}
	
	}
	patch.UpdateVertices();
}

void PointsMerger::Accept(BezierSurfaceC0& surface)
{
	for (auto single : surface.singleSurfaces)
	{
		single->Accept(*this);
	}
}

void PointsMerger::Accept(SingleBezierSurfaceC2& patch)
{
	for (size_t i = 0; i < patch.points.size(); i++)
	{
		for (size_t j = 0; j < patch.points[i].size(); j++)
		{
			auto point = patch.points[i][j];
			if (pointsIds.contains(point->id))
			{
				patch.points[i][j] = mergedPoint;
			}
		}

	}
	patch.UpdateVertices();
}

void PointsMerger::Accept(BezierSurfaceC2& surface)
{
	for (auto single : surface.singleSurfaces)
	{
		single->Accept(*this);
	}
}

void PointsMerger::Accept(CompositeModel& surface)
{
}
