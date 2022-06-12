#include "ObjectAdderWindow.h"
#include "imgui.h"
#include "BezierSurfaceC0AdderWindow.h"
#include "BezierSurfaceC2AdderWindow.h"
#include "PointsMerger.h"
#include "BezierSurfaceC0.h"
#include "GregoryFinder.h"
#include "GregoryPatch.h"
#include "BezierCurveInterpolating.h"
#include "GregoryFactory.h"

ObjectAdderWindow::ObjectAdderWindow(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
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
                for (auto cycle : cycles)
                {
                    std::vector<std::shared_ptr<Point>> pointsFirst;
                    std::vector<std::shared_ptr<Point>> pointsSecond;
                    for (int i = 0; i < cycle.size(); i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            pointsFirst.push_back(cycle[i]->firstLine[j]);
                        }
                        for (int j = 0; j < 4; j++)
                        {
                            pointsSecond.push_back(cycle[i]->secondLine[j]);
                        }
                    }


                    auto curveFirst = std::make_shared<BezierCurveInterpolating>(pointsFirst);
                    scene->AddModel(curveFirst);
                    curveFirst->ChangeColor({ 0,1,0 });

                    auto curveSecond = std::make_shared<BezierCurveInterpolating>(pointsSecond);
                    scene->AddModel(curveSecond);
                    curveSecond->ChangeColor({ 0,0,1 });

                    auto gregory = GregoryFactory::CreateGregoryPatch(cycle);
                    for (auto model : gregory)
                    {
                        scene->AddModel(model);
                    }
                }

               
            }
        }
       
    }

    if (scene->GetSelectedType<Point>().size() >= 2)
    {
        if (ImGui::Button("Merge selected points"))
        {
            PointsMerger pointsMerger;
            pointsMerger.MergePoints(scene);
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
}
