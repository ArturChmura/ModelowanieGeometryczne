#include "IntersecionAdderWindow.h"
#include "imgui.h"
#include "IntersectionFinder.h"
#include "BezierCurveInterpolating.h"
#include "IntersectionCurve.h"
#include "Pair.h"

IntersecionAdderWindow::IntersecionAdderWindow(std::shared_ptr<Scene> scene, bool* open, std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2)
{
	this->open = open;
	this->scene = scene;
	this->surface1 = surface1;
	this->surface2 = surface2;
	this->singleSurface = false;
}

IntersecionAdderWindow::IntersecionAdderWindow(std::shared_ptr<Scene> scene, bool* open, std::shared_ptr<IParameterized> surface)
{
	this->open = open;
	this->scene = scene;
	this->surface1 = surface;
	this->singleSurface = true;
}

IntersecionAdderWindow::~IntersecionAdderWindow()
{
}

void IntersecionAdderWindow::Render()
{
	ImGui::Begin("Add intersection", open);


	ImGui::DragFloat("Points distance", &pointsDistance, 0.01, 0.0001, 1, "%.6f");
	ImGui::DragFloat("Precision", &precision, 0.001, 0.00001, 1, "%.6f");

	ImGui::Checkbox("Use cursor", &useCursor);



	if (ImGui::Button("Add intersection"))
	{
		IntersectionFinder intersectionFinder(pointsDistance, precision, useCursor, scene->cursor->GetTranslation());

		auto intersectionPoints = singleSurface ? intersectionFinder.FindSelfIntersection(surface1) : intersectionFinder.FindIntersection(surface1, surface2);
		if (intersectionPoints)
		{
			std::vector<std::shared_ptr<Point>> points;
			std::vector<Pair<double>> USs[2];
			for (auto intersectionPoint : *intersectionPoints)
			{
				auto point = std::make_shared<Point>(intersectionPoint.position);
				points.push_back(point);

				USs[0].push_back({ intersectionPoint.u, intersectionPoint.v });
				USs[1].push_back({ intersectionPoint.s, intersectionPoint.t });
			}

			std::shared_ptr<IParameterized> surfaces[2] = { surface1, singleSurface ? surface1 : surface2 };
			auto interpolatingCurve = std::make_shared<IntersectionCurve>(points, scene, surfaces, USs);
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
