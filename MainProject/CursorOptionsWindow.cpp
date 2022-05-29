#include "CursorOptionsWindow.h"
#include "imgui.h"
using namespace DirectX::SimpleMath;


CursorOptionsWindow::CursorOptionsWindow(std::shared_ptr<Coursor3d> cursor3d, std::shared_ptr<Scene> scene, SIZE windowSize)
{
	this->cursor3d = cursor3d;
	this->scene = scene;
	this->windowSize = windowSize; 
	UpdateScreenCoords();
}

void CursorOptionsWindow::Render()
{
	ImGui::Begin("Cursor Options");

	ImGui::Text("Translation");
	auto translation = cursor3d->GetTranslation();
	if (ImGui::DragFloat("x", &translation.x) ||
		ImGui::DragFloat("y", &translation.y) ||
		ImGui::DragFloat("z", &translation.z)
		)
	{
		cursor3d->SetPosition(translation);
	}

	UpdateScreenCoords();
	ImGui::Text("Screen coordinates");

	
	if (
		ImGui::DragFloat("X##ScreenCoordsX", &screenCoords.x) ||
		ImGui::DragFloat("X##ScreenCoordsY", &screenCoords.y))
	{
		scene->UpdateCursorPositionFromScreenCoords(screenCoords);
	}


	ImGui::End();
}

void CursorOptionsWindow::UpdateScreenCoords()
{
	auto viewMatrx = scene->activeCamera->GetViewMatrix();
	auto perspectiveMatrix = scene->activeCamera->GetPerspectiveMatrix();
	Vector4 point(cursor3d->GetTranslation().x, cursor3d->GetTranslation().y, cursor3d->GetTranslation().z, 1);

	auto pos = Vector4::Transform(point, viewMatrx);
	pos = Vector4::Transform(pos, perspectiveMatrix);

	float x = pos.x / pos.w;
	float y = pos.y / pos.w;
	x = (x + 1) / 2.0f * windowSize.cx;
	y = (y + 1) / 2.0f * windowSize.cy;
	this->screenCoords = { x,y };
}
