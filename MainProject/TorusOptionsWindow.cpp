#include "TorusOptionsWindow.h"
#include "ImGui/imgui.h"

TorusOptionsWindow::TorusOptionsWindow(std::shared_ptr<Torus> torus)
{
	this->torus = torus;
	this->scale = torus->GetScale();
	this->translation = torus->GetTranslation();
	this->rotation = torus->GetRotation();
	this->R = torus->GetBigRadius();
	this->r = torus->GetSmallRadius();

	this->largeSlices = torus->GetLargeSlices();
	this->smallSlices = torus->GetSmallSlices();

	this->torusColor = torus->GetColor();
}

void TorusOptionsWindow::Render()
{
	ImGui::Begin("Options");
	ImGui::Text("Torus size");
	if (ImGui::SliderFloat("R", &R, 0.1f, 100.0f))
	{
		torus->SetBigRadius(R);
	}
	if (ImGui::SliderFloat("r", &r, 0.1f, R))
	{
		torus->SetSmallRadius(r);
	}

	ImGui::Text("Torus slices");
	if (ImGui::SliderInt("Large", &largeSlices, 2, 100))
	{
		torus->SetLargeSlices(largeSlices);
	}
	if (ImGui::SliderInt("Small", &smallSlices, 2, 100))
	{
		torus->SetSmallSlices(smallSlices);
	}

	ImGui::Text("Torus scale");
	if (
		ImGui::SliderFloat("x", &scale.x, 0.1f, 10.0f)
		|| ImGui::SliderFloat("y", &scale.y, 0.1f, 10.0f)
		|| ImGui::SliderFloat("z", &scale.z, 0.1f, 10.0f)
		)
	{
		torus->SetScale(scale.x, scale.y, scale.z);
	}

	ImGui::Text("Torus rotation");
	if (
		ImGui::SliderFloat("x##xRotation", &rotation.x, -3.14, 3.14)
		|| ImGui::SliderFloat("y##yRotation", &rotation.y, -3.14, 3.14)
		|| ImGui::SliderFloat("z##zRotation", &rotation.z, -3.14, 3.14)
		)
	{
		torus->SetRotation(rotation.x, rotation.y, rotation.z);
	}

	ImGui::Text("Torus translation");
	if (
		ImGui::SliderFloat("x##xTransation", &translation.x, -10.0f, 10.0f)
		|| ImGui::SliderFloat("y##yTransation", &translation.y, -10.0f, 10.0f)
		|| ImGui::SliderFloat("z##zTransation", &translation.z, -10.0f, 10.0f)
		)
	{
		torus->SetTranslation(translation.x, translation.y, translation.z);
	}



	if (ImGui::ColorEdit3("Torus color", (float*)&torusColor))
	{
		torus->SetColor(torusColor);
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

