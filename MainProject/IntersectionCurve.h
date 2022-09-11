#pragma once
#include "BezierCurveInterpolating.h"
#include "SimpleMath.h"
#include <vector>
#include "Scene.h"
class IntersectionCurve : public BezierCurveInterpolating
{
public:
	IntersectionCurve(std::vector<std::shared_ptr<Point>> points, std::shared_ptr<Scene> scene);
	virtual void RenderGUI() override;
	std::shared_ptr<Scene> scene;
};