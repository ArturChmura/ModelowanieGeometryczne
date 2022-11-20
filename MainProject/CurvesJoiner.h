#pragma once
#include "IBezierCurve.h"
#include "Scene.h"

class CurvesJoiner
{
public:
	static void JoinCurves(std::shared_ptr<IBezierCurve> curve1, std::shared_ptr<IBezierCurve> curve2, std::shared_ptr<Scene> scene);
	static void JoinPoint(std::shared_ptr<IBezierCurve> curve, std::shared_ptr<Point> point);
};