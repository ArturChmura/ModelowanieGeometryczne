#include "CurvesJoiner.h"
#include<iterator> 

using namespace DirectX::SimpleMath;

void CurvesJoiner::JoinCurves(std::shared_ptr<IBezierCurve> curve1, std::shared_ptr<IBezierCurve> curve2, std::shared_ptr<Scene> scene)
{
	if (curve1->points.size() == 0)
	{
		scene->RemoveModel(curve1->id);
		return;
	}
	if (curve2->points.size() == 0)
	{
		scene->RemoveModel(curve2->id);
		return;
	}

	if (curve1->points.size() == 1 && curve2->points.size() == 1)
	{
		scene->RemoveModel(curve2->id);
		curve1->AddPoint(curve2->points[0]);
		return;
	}

	if (curve1->points.size() == 1)
	{
		JoinPoint(curve2, curve1->points[0]);
		scene->RemoveModel(curve1->id);
		return;
	}

	if (curve2->points.size() == 1)
	{
		JoinPoint(curve1, curve2->points[0]);
		scene->RemoveModel(curve2->id);
		return;
	}

	auto points1 = curve1->points;
	auto points2 = curve2->points;

	auto begin1Location = Vector3(points1[0]->GetTranslation());
	auto end1Location = Vector3(points1[points1.size() - 1]->GetTranslation());
	auto begin2Location = Vector3(points2[0]->GetTranslation());
	auto end2Location = Vector3(points2[points2.size() - 1]->GetTranslation());

	auto beginToBegin = (begin1Location - begin2Location).LengthSquared();
	auto begin1ToEnd2 = (begin1Location - end2Location).LengthSquared();
	auto begin2ToEnd1 = (begin2Location - end1Location).LengthSquared();
	auto endToEnd = (end1Location - end2Location).LengthSquared();

	auto minValue = min(beginToBegin, min(begin1ToEnd2, min(begin2ToEnd1, endToEnd)));

	if (minValue == beginToBegin)
	{
		for (auto point : points2)
		{
			curve1->AddPointToBeggining(point);
		}
		scene->RemoveModel(curve2->id);
	}
	else if (minValue == begin1ToEnd2)
	{
		for (auto point : points1)
		{
			curve2->AddPoint(point);
		}
		scene->RemoveModel(curve1->id);
	}
	else if (minValue == begin2ToEnd1)
	{
		for (auto point : points2)
		{
			curve1->AddPoint(point);
		}
		scene->RemoveModel(curve2->id);
	}
	else if (minValue == endToEnd)
	{
		for (int i = points2.size() - 1; i >= 0 ; i--)
		{
			curve1->AddPoint(points2[i]);
		}
		scene->RemoveModel(curve2->id);

	}
}

void CurvesJoiner::JoinPoint(std::shared_ptr<IBezierCurve> curve, std::shared_ptr<Point> point)
{
	auto beginPointLocation = Vector3(curve->points[0]->GetTranslation());
	auto endPointLocation = Vector3(curve->points[curve->points.size()-1]->GetTranslation());
	auto pointLocation = Vector3(point->GetTranslation());
	auto distanceToBegin = (pointLocation - beginPointLocation).LengthSquared();
	auto distanceToEnd = (pointLocation - endPointLocation).LengthSquared();

	if (distanceToBegin < distanceToEnd)
	{
		curve->AddPointToBeggining(point);
	}
	else
	{
		curve->AddPoint(point);
	}

}