#include "ObjectAdderWindow.h"
#include "imgui.h"
#include "BezierSurfaceC0AdderWindow.h"
#include "BezierSurfaceC2AdderWindow.h"
#include "PointsMerger.h"
#include "BezierSurfaceC0.h"
#include "BezierSurfaceC2.h"
#include "GregoryFinder.h"
#include "GregoryPatch.h"
#include "BezierCurveInterpolating.h"

ObjectAdderWindow::ObjectAdderWindow(std::shared_ptr<Scene> scene, bool* renderGui)
{
	this->scene = scene;
    this->renderGui = renderGui;
}

void ObjectAdderWindow::Render()
{
    ImGui::Begin("Add object");

    auto selectedBezierSurfaceC0 = scene->GetSelectedType<BezierSurfaceC0>();
    if (selectedBezierSurfaceC0.size() >= 1)
    {
        if (ImGui::Button("Join selected patches"))
        {
            auto cycles = GregoryFinder::FindFill(selectedBezierSurfaceC0);
            if (cycles.size() > 0)
            {
                gregoryAdderOpen = true;
                this->gregoryAdder = std::make_shared<GregoryAdderWindow>(scene, &gregoryAdderOpen, cycles);
            }
        }
       
    }

    auto selectedToruses = scene->GetSelectedType<Torus>();
    auto selectedC0Surface = scene->GetSelectedType<BezierSurfaceC0>();
    auto selectedC2Surface = scene->GetSelectedType<BezierSurfaceC2>();

    std::vector<std::shared_ptr<IParameterized>> parametrized;
    parametrized.insert(parametrized.end(), selectedToruses.begin(), selectedToruses.end());
    parametrized.insert(parametrized.end(), selectedC0Surface.begin(), selectedC0Surface.end());
    parametrized.insert(parametrized.end(), selectedC2Surface.begin(), selectedC2Surface.end());
    if (parametrized.size() == 2 || parametrized.size() == 1)
    {
        if (ImGui::Button("Find intersection"))
        {
            intersectionAdderOpen = true;
            if (parametrized.size() == 2)
            {
                this->intersecionAdder = std::make_shared<IntersecionAdderWindow>(scene, &intersectionAdderOpen, parametrized[0], parametrized[1]);
            }
            else
            {
                this->intersecionAdder = std::make_shared<IntersecionAdderWindow>(scene, &intersectionAdderOpen, parametrized[0]);
            }
        }

    }

    auto selectedPoints = scene->GetSelectedType<Point>();
    if (selectedPoints.size() >= 2)
    {
        if (ImGui::Button("Merge selected points"))
        {
            PointsMerger pointsMerger;
            pointsMerger.MergePoints(scene, selectedPoints);
        }
    }
    if (ImGui::Button("Add Torus"))
    {
        scene->AddTorus();
    }
    if (ImGui::Button("Add Point"))
    {
        scene->AddPoint();
    }
    if (scene->GetSelectedType<Point>().size() > 0)
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
    if (gregoryAdder)
    {
        if (gregoryAdderOpen)
        {
            gregoryAdder->Render();
        }
        else
        {
            gregoryAdder = nullptr;
        }
    }

    if (intersecionAdder)
    {
        if (intersectionAdderOpen)
        {
            intersecionAdder->Render();
        }
        else
        {
            intersecionAdder = nullptr;
        }
    }
}
