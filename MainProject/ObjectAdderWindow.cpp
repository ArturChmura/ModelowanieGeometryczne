#include "ObjectAdderWindow.h"
#include "ImGui/imgui.h"
#include "BezierSurfaceC0AdderWindow.h"
#include "BezierSurfaceC2AdderWindow.h"

ObjectAdderWindow::ObjectAdderWindow(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

void ObjectAdderWindow::Render()
{
    ImGui::Begin("Add object");
    if (ImGui::Button("Add Torus"))
    {
        scene->AddTorus();
    }
    if (ImGui::Button("Add Point"))
    {
        scene->AddPoint();
    }
    if (scene->GetSelectedPoints().size() > 0)
    {
        if (ImGui::Button("Add Bezier Curve C0"))
        {
            scene->AddBezierCurveC0FromSelectedPoints();
        }
        if (ImGui::Button("Add Bezier Curve C2"))
        {
            scene->AddBezierCurveC2FromSelectedPoints();
        }
        if (ImGui::Button("Add Bezier Curve C2 Interpolating"))
        {
            scene->AddBezierCurveInterpolatingFromSelectedPoints();
        }
    }
    if (ImGui::Button("Add Bezier Surface C0"))
    {
        if (!surfaceAdderOpen)
        {
            surfaceAdderOpen = true;
            this->adder = std::make_shared<BezierSurfaceC0AdderWindow>(scene, &surfaceAdderOpen);
        }
    }
    if (ImGui::Button("Add Bezier Surface C2"))
    {
        if (!surfaceAdderOpen)
        {
            surfaceAdderOpen = true;
            this->adder = std::make_shared<BezierSurfaceC2AdderWindow>(scene, &surfaceAdderOpen);
        }
    }
    ImGui::End();
    if (adder)
    {
        if (surfaceAdderOpen)
        {
            adder->Render();
        }
        else
        {
            adder = nullptr;
        }
    }
}
