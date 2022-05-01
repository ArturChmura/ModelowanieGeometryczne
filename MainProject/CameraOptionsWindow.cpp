#include "CameraOptionsWindow.h"
#include "ImGui/imgui.h"


void CameraOptionsWindow::Render(std::shared_ptr<Scene> scene)
{
	ImGui::Begin("Camera Options");

    if (ImGui::BeginListBox("##CamerasListBox", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (int i = 0; i < scene->cameras.size(); i++)
        {
            auto camera = scene->cameras[i];
            const bool is_selected = scene->activeCamera->id == camera->id;

            if (ImGui::Selectable((camera->name + "##" + std::to_string(camera->id)).c_str(), is_selected))
            {
                scene->activeCamera = camera;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    scene->activeCamera->DrawGUI();
	ImGui::End();
}

