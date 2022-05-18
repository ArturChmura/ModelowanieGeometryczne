#include "ObjectsListWindow.h"
#include "ImGui/imgui.h"
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

                if (ImGui::Selectable((model->name + "##" + std::to_string(model->id)).c_str(), is_selected))
                {
                    auto io = ImGui::GetIO();
                    if (io.KeyCtrl)
                    {
                        scene->ChangeSelection(model);
                    }
                    else
                    {
                        scene->Select(model);
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
