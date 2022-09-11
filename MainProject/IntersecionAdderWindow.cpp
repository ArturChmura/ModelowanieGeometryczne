#include "IntersecionAdderWindow.h"
#include "imgui.h"
#include "IntersectionFinder.h"
#include "BezierCurveInterpolating.h"

IntersecionAdderWindow::IntersecionAdderWindow(std::shared_ptr<Scene> scene, bool* open, std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2)
{
	this->open = open;
	this->scene = scene;
	this->surface1 = surface1;
	this->surface2 = surface2;

	IntersectionFinder intersectionFinder;
	auto model = intersectionFinder.FindIntersection(surface1, surface2);
	scene->AddModel(model);

}

IntersecionAdderWindow::~IntersecionAdderWindow()
{
}

void IntersecionAdderWindow::Render()
{
	ImGui::Begin("Add intersection", open);

	if (ImGui::Button("Ok"))
	{
		*(this->open) = false;
	}

	ImGui::End();
}
