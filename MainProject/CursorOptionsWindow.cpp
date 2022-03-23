#include "CursorOptionsWindow.h"
#include "ImGui/imgui.h"
using namespace DirectX::SimpleMath;


CursorOptionsWindow::CursorOptionsWindow(std::shared_ptr<Coursor3d> cursor3d, std::shared_ptr<Scene> scene, SIZE windowSize)
{
	this->cursor3d = cursor3d;
	this->translation = cursor3d->translation;
	this->scene = scene;
	this->windowSize = windowSize; 
	UpdateScreenCoords();
}

void CursorOptionsWindow::Render()
{
	ImGui::Begin("Cursor Options");

	ImGui::Text("Translation");
	if (ImGui::DragFloat("x", &translation.x) ||
		ImGui::DragFloat("y", &translation.y) ||
		ImGui::DragFloat("z", &translation.z)
		)
	{
		cursor3d->translation = { translation.x, translation.y, translation.z };
	}

	UpdateScreenCoords();
	ImGui::Text("Screen coordinates");

	
	if (
		ImGui::DragFloat("X##ScreenCoordsX", &screenCoords.x) ||
		ImGui::DragFloat("X##ScreenCoordsY", &screenCoords.y))
	{
		UpdateCursorPositionFromScreenCoords();
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

void CursorOptionsWindow::UpdateCursorPositionFromScreenCoords()
{
	Matrix viewMatrx = scene->activeCamera->GetViewMatrix();
	Matrix perspectiveMatrix = scene->activeCamera->GetPerspectiveMatrix();
	auto perspectiveMatrixInverted = perspectiveMatrix.Invert();
	auto viewMatrxInverted = viewMatrx.Invert();

	Vector4 currentGlobalPosition(cursor3d->translation.x, cursor3d->translation.y, cursor3d->translation.z, 1);
	auto currentCameraPosition =  Vector4::Transform(currentGlobalPosition, viewMatrx);
	auto currentPerspectivePosition = Vector4::Transform(currentCameraPosition, perspectiveMatrix);
	float w = currentPerspectivePosition.w;
	float z = currentCameraPosition.z;
	currentPerspectivePosition = currentPerspectivePosition / currentPerspectivePosition.w;

	Vector2 normalizedScreenCoords = { screenCoords.x * 2.0f / windowSize.cx - 1,screenCoords.y * 2.0f / windowSize.cy - 1 };

	Vector4 newPerspectivePostion = { normalizedScreenCoords.x, normalizedScreenCoords.y, 1 ,1};
	newPerspectivePostion = newPerspectivePostion * w;

	auto newCameraPosition = Vector4::Transform(newPerspectivePostion, perspectiveMatrixInverted);
	newCameraPosition.w = 1;
	auto newWorld = Vector4::Transform(newCameraPosition, viewMatrxInverted);

	this->translation =  this->cursor3d->translation = { newWorld.x, newWorld.y, newWorld.z };
}
