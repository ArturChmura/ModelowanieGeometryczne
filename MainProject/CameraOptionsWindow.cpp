#include "CameraOptionsWindow.h"
#include "ImGui/imgui.h"


void CameraOptionsWindow::Render()
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

CameraOptionsWindow::CameraOptionsWindow(std::shared_ptr<Scene> scene)
{
    this->scene = scene;
}
