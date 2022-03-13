#include "OptionsWindow.h"
#include "ImGui/imgui.h"

OptionsWindow::OptionsWindow(std::shared_ptr<Application> application)
{
	this->application = application;
	this->scale = application->torusModel->scale;
	this->torusColor = application->torusModel->color;
}

void OptionsWindow::Render()
{
	ImGui::Begin("Options");
	ImGui::Text("Torus size");
	if (
		ImGui::SliderFloat("R", &application->torusModel->R, 0.1f, 100.0f) ||
		ImGui::SliderFloat("r", &application->torusModel->r, 0.1f, application->torusModel->R))
	{
		application->UpdateBuffers();
	}

	ImGui::Text("Torus slices");
	if (
		ImGui::SliderInt("Large", &application->torusModel->largeSlices, 2, 100)
		|| ImGui::SliderInt("Small", &application->torusModel->smallSlices, 2, 100)
		) 
	{
		application->UpdateBuffers();
	}

	ImGui::Text("Torus scale");
	if (
		ImGui::SliderFloat("x", &scale.x, 0.1f, 10.0f)
		|| ImGui::SliderFloat("y", &scale.y, 0.1f, 10.0f)
		|| ImGui::SliderFloat("z", &scale.z, 0.1f, 10.0f)
		) {
		application->torusModel->SetScale(scale.x, scale.y, scale.z);
	}

	ImGui::Text("Torus translation");
	if (
		ImGui::SliderFloat("x##xTransation", &translation.x, -10.0f, 10.0f)
		|| ImGui::SliderFloat("y##yTransation", &translation.y, -10.0f, 10.0f)
		|| ImGui::SliderFloat("z##zTransation", &translation.z, -10.0f, 10.0f)
		) {
		application->torusModel->SetTranslation(translation.x, translation.y, translation.z);
	}



	if (ImGui::ColorEdit3("Torus color", (float*)&torusColor))
	{
		application->torusModel->ChangeColor(torusColor);
		application->UpdateBuffers();
	}

	ImGui::ColorEdit3("Background color", (float*)&application->backgroundColor);


	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

