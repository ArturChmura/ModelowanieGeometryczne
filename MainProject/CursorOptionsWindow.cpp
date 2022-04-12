#include "CursorOptionsWindow.h"
#include "ImGui/imgui.h"
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
	if (ImGui::DragFloat("x", &cursor3d->translation.x) ||
		ImGui::DragFloat("y", &cursor3d->translation.y) ||
		ImGui::DragFloat("z", &cursor3d->translation.z)
		)
	{
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
	Vector4 point(cursor3d->translation.x, cursor3d->translation.y, cursor3d->translation.z, 1);

	auto pos = Vector4::Transform(point, viewMatrx);
	pos = Vector4::Transform(pos, perspectiveMatrix);

	float x = pos.x / pos.w;
	float y = pos.y / pos.w;
	x = (x + 1) / 2.0f * windowSize.cx;
	y = (y + 1) / 2.0f * windowSize.cy;
	this->screenCoords = { x,y };
}
