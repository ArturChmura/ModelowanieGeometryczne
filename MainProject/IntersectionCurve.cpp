#include "IntersectionCurve.h"
#include "imgui.h"

IntersectionCurve::IntersectionCurve(std::vector<std::shared_ptr<Point>> points, std::shared_ptr<Scene> scene)
	:BezierCurveInterpolating(points)
{
	this->name = "Intersection Curve";
	this->scene = scene;
}

void IntersectionCurve::RenderGUI()
{

	if (ImGui::Button("Transform to interpolating curve"))
	{
		auto pointsCopy = this->points;
		scene->RemoveModel(this->id);
		auto interpolatingCurve = std::make_shared< BezierCurveInterpolating>(pointsCopy);
		for (auto point : pointsCopy)
		{
			scene->AddModel(point);
		}
		scene->AddModel(interpolatingCurve);
		scene->Select(interpolatingCurve);
	}
}
