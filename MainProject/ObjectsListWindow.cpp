#include "ObjectsListWindow.h"
#include "ImGui/imgui.h"
#include <unordered_set>
#include "CompositeModel.h"
ObjectsListWindow::ObjectsListWindow(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

void ObjectsListWindow::Render()
{
	ImGui::Begin("Objects list");
    if (ImGui::BeginListBox("##ObjectsListBox", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (int i = 0; i < scene->models.size(); i++)
        {
            auto model = scene->models[i];
            const bool is_selected = scene->IsSelcted(model->id);

            if (ImGui::Selectable((model->name + "##" + std::to_string(model->id)).c_str(), is_selected))
            {
                scene->ChangeSelection(model->id);
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    if (scene->composite->modelsMap.size() > 0)
    {
        if (ImGui::Button("Remove selected"))
        {
            for (auto [id, model] : scene->composite->modelsMap)
            {
                scene->DeleteModel(id);
            }
            scene->selectedModel = nullptr;
            scene->composite = std::make_shared<CompositeModel>();
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
	ImGui::End();
}
