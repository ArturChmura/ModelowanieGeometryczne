#include "IModel.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
using namespace DirectX;
IModel::IModel(std::string name)
{
	this->name = name;
	static int id = 0;
	this->id = ++id;
}

void IModel::ChangeName(std::string name)
{
	this->name = name;
}


void IModel::RenderGUI()
{
	ImGui::InputText("Name##IModelName", &name);
}


