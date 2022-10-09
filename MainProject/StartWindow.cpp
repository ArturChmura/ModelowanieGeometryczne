#include "StartWindow.h"
#include "imgui.h"
#include <nfd.h>
#include <Serializer.h>
#include "SceneLoader.h"
#include "SerializationVisitor.h"



StartWindow::StartWindow(
    std::vector<std::shared_ptr<IScene>> scenes, 
    std::function<void(std::shared_ptr<IScene>)> changeSceneLambda)
{
	this->changeSceneLambda = changeSceneLambda;
	this->scenes = scenes;
}

void StartWindow::Render()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;
   
    ImGui::Begin("Start", (bool*)0, window_flags);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            for (auto scene : scenes)
            {
                if (ImGui::MenuItem(scene->name.c_str()))
                {
                    changeSceneLambda(scene);
                }
            }
           
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
}
