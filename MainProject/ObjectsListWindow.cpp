#include "ObjectsListWindow.h"
#include "imgui.h"
#include "CompositeModel.h"
ObjectsListWindow::ObjectsListWindow(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

void ObjectsListWindow::Render()
{
	ImGui::Begin("Objects list");
    if (ImGui::BeginListBox("##ObjectsListBox", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing())))
    {
        ImGuiListClipper clipper;
        clipper.Begin(scene->models.size());
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                auto model = scene->models[i];
                const bool is_selected = scene->IsSelcted(model->id);

                if (ImGui::Selectable((model->name + "##" + std::to_string(model->id)).c_str(), is_selected, ImGuiSelectableFlags_::ImGuiSelectableFlags_AllowDoubleClick))
                {
                    auto io = ImGui::GetIO();
                    if (io.KeyCtrl)
                    {
                        scene->ChangeSelection(model);
                        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                        {
                            scene->Select(model);
                            auto innerModels = model->GetContainingModels();
                            for (auto innerModel : innerModels)
                            {
                                scene->Select(innerModel);
                            }
                        }
                    }
                    else
                    {
                        scene->DeselectAll();
                        scene->Select(model);
                        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                        {
                            auto innerModels = model->GetContainingModels();
                            for (auto innerModel : innerModels)
                            {
                                scene->Select(innerModel);
                            }
                        }
                    }
                    
                   
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
       
        ImGui::EndListBox();
    }
    if (scene->composite->modelsMap.size() > 0)
    {
        if (ImGui::Button("Remove selected"))
        {
            scene->RemoveSelectedModels();
        }
    }



	ImGui::End();


}
