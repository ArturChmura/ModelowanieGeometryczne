#include "DebugWindow.h"
#include "imgui.h"

void DebugWindow::Render()
{
	ImGui::Begin("Debug Window");

	ImGui::Text(debugMessage.c_str());
	
	ImGui::End();
}
