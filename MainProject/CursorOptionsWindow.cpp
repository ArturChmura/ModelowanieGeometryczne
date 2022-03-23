#include "CursorOptionsWindow.h"
#include "ImGui/imgui.h"


CursorOptionsWindow::CursorOptionsWindow(std::shared_ptr<Coursor3d> cursor3d)
{
	this->cursor3d = cursor3d;
	this->translation = cursor3d->translation;
}

void CursorOptionsWindow::Render()
{
	ImGui::Begin("Cursor Options");
	ImGui::Text("Translation");
	if (ImGui::DragFloat("x", &translation.x)||
		ImGui::DragFloat("y", &translation.y)||
		ImGui::DragFloat("z", &translation.z)
		)
	{
		cursor3d->translation = { translation.x, translation.y, translation.z };
	}
	
	ImGui::End();
}
