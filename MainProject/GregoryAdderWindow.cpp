#include "GregoryAdderWindow.h"
#include "imgui.h"

#include <string>

GregoryAdderWindow::GregoryAdderWindow(std::shared_ptr<Scene> scene, bool* open, std::vector<std::vector<std::shared_ptr<PatchSide>>> cycles)
{
	this->open = open;
	this->scene = scene;
	this->cycles = cycles;
	SetPreview();
}

GregoryAdderWindow::~GregoryAdderWindow()
{
	scene->previewModel = nullptr;
}

void GregoryAdderWindow::Render()
{
	ImGui::Begin("Add gregory", open);

	if (ImGui::Button("Next"))
	{
		index = (index + 1) % cycles.size();
		SetPreview();
	}

	if (ImGui::Button("Dodaj model"))
	{
		AddModel();
	}

	ImGui::End();
}

void GregoryAdderWindow::SetPreview()
{
	auto model = this->GetModel();
	this->scene->previewModel = model;

}

void GregoryAdderWindow::AddModel()
{
	*(this->open) = false;
	auto model = this->GetModel();
	this->scene->AddModel(model);
	this->scene->previewModel = nullptr;
	
}

std::shared_ptr<GregoryPatch> GregoryAdderWindow::GetModel()
{

	return std::make_shared< GregoryPatch>(cycles[index]);
}
