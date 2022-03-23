#include "CompositeModel.h"
#include "ImGui/imgui.h"
using namespace DirectX;

using namespace DirectX::SimpleMath;

CompositeModel::CompositeModel()
{
	this->centroidCoursor = std::make_shared<Coursor3d>();
	this->imguiTranslation = this->centroidPosition = { 0,0,0 };
	this->rotation = Quaternion::Identity;
	this->imguiScale = this->scale = { 1,1,1 };
}

void CompositeModel::AddModel(std::shared_ptr<IModel> model)
{
	this->modelsMap.insert(std::pair(model->id, model));
	UpdateCentroidPosition();
}

void CompositeModel::RemoveModel(std::shared_ptr<IModel> model)
{
	this->modelsMap.erase(model->id);
	UpdateCentroidPosition();
}

void CompositeModel::UpdateCentroidPosition()
{
	if (modelsMap.size() == 0)
	{
		centroidPosition = { 0,0,0 };
		return;
	}
	int size = modelsMap.size();
	float x = 0, y = 0, z = 0;
	for (const auto& [key, model] : modelsMap)
	{
		auto translation = model->GetTranslation();
		x += translation.x;
		y += translation.y;
		z += translation.z;
	}
	this->imguiTranslation = this->centroidPosition = { x / size, y / size, z / size };
	
}

void CompositeModel::SetScale(float x, float y, float z)
{
	DirectX::XMFLOAT3 diff = { x / scale.x, y / scale.y ,z  / scale.z };
	for (const auto& [key, model] : modelsMap)
	{
		model->ScaleFromPoint(centroidPosition, diff);
	}
	scale = { x,y,z };
}

DirectX::XMFLOAT3 CompositeModel::GetScale()
{
	return scale;
}

void CompositeModel::SetTranslation(float x, float y, float z)
{
	DirectX::XMFLOAT3 diff = { x - centroidPosition.x, y - centroidPosition.y ,z - centroidPosition.z };
	for (const auto& [key, model] : modelsMap)
	{
		model->Translate(diff.x, diff.y, diff.z);
	}
	centroidPosition = { x,y,z };
}

void CompositeModel::Translate(float x, float y, float z)
{
	for (const auto& [key, model] : modelsMap)
	{
		model->Translate(x, y, z);
	}
	centroidPosition = { centroidPosition.x + x,centroidPosition.y + y,centroidPosition.z + z };
}

DirectX::XMFLOAT3 CompositeModel::GetTranslation()
{
	return centroidPosition;
}

void CompositeModel::SetRotation(float pitch, float yaw, float roll)
{
	auto e = rotation.ToEuler();
	DirectX::XMFLOAT3 diff = { pitch - e.x, yaw - e.y , roll - e.z };
	for (const auto& [key, model] : modelsMap)
	{
		model->RotateFromPoint(centroidPosition, diff);
	}
	rotation = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
}

DirectX::SimpleMath::Quaternion CompositeModel::GetRotation()
{
	return rotation;
}

void CompositeModel::ScaleFromPoint(DirectX::XMFLOAT3 point, DirectX::XMFLOAT3 scale)
{
	for (const auto& [key, model] : modelsMap)
	{
		model->ScaleFromPoint(point, scale);
	}
}

void CompositeModel::RotateFromPoint(DirectX::XMFLOAT3 globalPoint, DirectX::XMFLOAT3 ratation)
{
	for (const auto& [key, model] : modelsMap)
	{
		model->RotateFromPoint(globalPoint, ratation);
	}
}

void CompositeModel::Draw(std::shared_ptr<Camera> camera)
{
	this->centroidCoursor->translation = this->centroidPosition;
	this->centroidCoursor->Draw(camera);
}

void CompositeModel::RenderGUI()
{
	ImGui::Text("Scale");
	float minScale = 0.1f;
	if (
		ImGui::DragFloat("x##xScale", &imguiScale.x, 0.1f, minScale)
		|| ImGui::DragFloat("y##yScale", &imguiScale.y, 0.1f, minScale)
		|| ImGui::DragFloat("z##zScale", &imguiScale.z, 0.1f, minScale)
		)
	{
		imguiScale = { max(imguiScale.x, minScale), max(imguiScale.y, minScale), max(imguiScale.z, minScale) };
		SetScale(imguiScale.x, imguiScale.y, imguiScale.z);
	}


	ImGui::Text("Rotation");
	auto rotationEuler = rotation.ToEuler();
	rotationEuler = { XMConvertToDegrees(rotationEuler.x),  XMConvertToDegrees(rotationEuler.y),XMConvertToDegrees(rotationEuler.z) };
	if (
		ImGui::DragFloat("x##xRotation", &rotationEuler.x, 1.0f)
		|| ImGui::DragFloat("y##yRotation", &rotationEuler.y, 1.0f)
		|| ImGui::DragFloat("z##zRotation", &rotationEuler.z, 1.0f)
		)
	{
		SetRotation(
			DirectX::XMConvertToRadians(rotationEuler.x),
			DirectX::XMConvertToRadians(rotationEuler.y),
			DirectX::XMConvertToRadians(rotationEuler.z));
	}

	ImGui::Text("Translation");
	if (
		ImGui::DragFloat("x##xTransation", &imguiTranslation.x)
		|| ImGui::DragFloat("y##yTransation", &imguiTranslation.y)
		|| ImGui::DragFloat("z##zTransation", &imguiTranslation.z)
		)
	{
		SetTranslation(imguiTranslation.x, imguiTranslation.y, imguiTranslation.z);
	}
}

void CompositeModel::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
}
