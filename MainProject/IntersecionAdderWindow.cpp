#include "IntersecionAdderWindow.h"
#include "imgui.h"
#include "IntersectionFinder.h"
#include "BezierCurveInterpolating.h"
#include "IntersectionCurve.h"

IntersecionAdderWindow::IntersecionAdderWindow(std::shared_ptr<Scene> scene, bool* open, std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2)
{
	this->open = open;
	this->scene = scene;
	this->surface1 = surface1;
	this->surface2 = surface2;


}

IntersecionAdderWindow::~IntersecionAdderWindow()
{
}

void IntersecionAdderWindow::Render()
{
	ImGui::Begin("Add intersection", open);


	ImGui::DragFloat("Points distance", &pointsDistance, 0.01, 0.0001);
	ImGui::DragFloat("Precision", &precision, 0.001, 0.00001);
	ImGui::Checkbox("Use cursor", &useCursor);


	if (ImGui::Button("Add intersection"))
	{
		IntersectionFinder intersectionFinder(pointsDistance, precision, useCursor, scene->cursor->GetTranslation());
		auto intersectionPoints = intersectionFinder.FindIntersection(surface1, surface2);
		if (intersectionPoints)
		{
			std::vector<std::shared_ptr<Point>> points;
			for (auto intersectionPoint : *intersectionPoints)
			{
				auto point = std::make_shared<Point>(intersectionPoint.position);
				points.push_back(point);
			}
			auto interpolatingCurve = std::make_shared<IntersectionCurve>(points, scene);
			scene->AddModel(interpolatingCurve);
			*(this->open) = false;
		}
		else
		{
			textNotFound = true;
		}
	}

	if (textNotFound)
	{
		ImGui::Text("Intersection not found");
	}

	ImGui::End();
}
