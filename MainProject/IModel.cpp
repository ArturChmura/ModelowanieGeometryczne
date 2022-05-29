#include "IModel.h"
#include "imgui.h"
#include "imgui_stdlib.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;
IModel::IModel(std::string name)
	:Named(name)
{
}

void IModel::RenderGUI()
{
	ImGui::InputText("Name##IModelName", &name);
}


void IModel::OnSelect()
{
	this->ChangeColor(selectedColor);
}

void IModel::OnDeselect()
{
	this->ChangeColor(defaultColor);
}

void IModel::ChangeDefaultColor(DirectX::SimpleMath::Vector3 color)
{
	this->defaultColor = color;
	this->ChangeColor(defaultColor);
}


void IModel::OnAddedToScene()
{
}

void IModel::OnRemovedFromScene()
{
}

void IModel::SetVisible(bool visible)
{
	this->visible = visible;
}

bool IModel::GetVisible()
{
	return visible;
}

std::shared_ptr<IModel> IModel::SelectFromScreenCoords(float x, float y, Matrix VP)
{
	float delta = 10.0f;
	auto p = GetTranslation();
	Vector4 modelPosition = { p.x,p.y,p.z,1 };
	auto modelPerspectivePosition = Vector4::Transform(modelPosition, VP);
	modelPerspectivePosition /= modelPerspectivePosition.w;
	float modelX = (modelPerspectivePosition.x + 1) / 2.0f * DxDevice::windowSize.cx;
	float modelY = DxDevice::windowSize.cy - (modelPerspectivePosition.y + 1) / 2.0f * DxDevice::windowSize.cy;

	Vector2 modelScreenPos = { modelX, modelY };
	Vector2 clickScreenPos = { x,y };
	auto diff = clickScreenPos - modelScreenPos;
	auto lengthSqrt = diff.LengthSquared();
	if (lengthSqrt < delta * delta)
	{
		return shared_from_this();
	}
	return nullptr;
}

std::vector < std::shared_ptr<IModel>> IModel::GetContainingModels()
{
	return std::vector < std::shared_ptr<IModel>>();
}


