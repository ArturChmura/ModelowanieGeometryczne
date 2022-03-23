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
    static std::shared_ptr< CompositeModel> composite = std::make_shared<CompositeModel>();
    if (ImGui::BeginListBox("##ObjectsListBox", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (int i = 0; i < scene->models.size(); i++)
        {
            auto model = scene->models[i];
            const bool is_selected = (composite->modelsMap.contains(model->id));

            if (ImGui::Selectable((model->name + "##" + std::to_string(model->id)).c_str(), is_selected))
            {
                if (is_selected)
                {
                    composite->RemoveModel(model);
                }
                else
                {
                    composite->AddModel(model);
                }
                
                if (composite->modelsMap.size() == 0)
                {
                    scene->selectedModel = nullptr;
                }
                else if (composite->modelsMap.size() == 1)
                {
                    scene->selectedModel = composite->modelsMap.begin()->second;
                }
                else
                {
                    scene->selectedModel = composite;
                }
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    if (composite->modelsMap.size() > 0)
    {
        if (ImGui::Button("Remove selected"))
        {
            for (auto [id, model] : composite->modelsMap)
            {
                scene->DeleteModel(id);
            }
            scene->selectedModel = nullptr;
            composite = std::make_shared<CompositeModel>();
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
