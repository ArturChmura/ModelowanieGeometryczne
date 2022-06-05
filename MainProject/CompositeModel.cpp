#include "CompositeModel.h"
#include "imgui.h"
#include <limits>
using namespace DirectX;

using namespace DirectX::SimpleMath;

CompositeModel::CompositeModel()
{
	this->centroidCoursor = std::make_shared<Coursor3d>();
	this->imguiTranslation = this->centroidPosition = { 0,0,0,1 };
	this->rotation = { 0,0,0 };
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
	int size = 0;
	float x = 0, y = 0, z = 0;
	for (const auto& [key, model] : modelsMap)
	{
		if (model->IsMovable())
		{

			auto translation = model->GetTranslation();
			x += translation.x;
			y += translation.y;
			z += translation.z;
			size++;
		}

	}
	if (size == 0)
	{
		centroidPosition = { FLT_MAX,FLT_MAX,FLT_MAX };
		return;
	}
	this->imguiTranslation = this->centroidPosition = { x / size, y / size, z / size };

}

void CompositeModel::SetScale(float x, float y, float z)
{
	Scale(x / scale.x, y / scale.y, z / scale.z);
}

void CompositeModel::Scale(float x, float y, float z)
{
	for (const auto& [key, model] : modelsMap)
	{
		model->ScaleFromPoint(centroidPosition, {x,y,z});
	}
	scale *= { x,y,z };
}

Vector3 CompositeModel::GetScale()
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
	centroidPosition = { x,y,z,1 };
}

void CompositeModel::Translate(float x, float y, float z)
{
	for (const auto& [key, model] : modelsMap)
	{
		model->Translate(x, y, z);
	}
	centroidPosition = { centroidPosition.x + x,centroidPosition.y + y,centroidPosition.z + z , 1 };
}

Vector4 CompositeModel::GetTranslation()
{
	return centroidPosition;
}

void CompositeModel::SetRotation(float x, float y, float z)
{
	Rotate(x - rotation.x, y - rotation.y, z - rotation.z);
}

void CompositeModel::Rotate(float x, float y, float z)
{
	for (const auto& [key, model] : modelsMap)
	{
		model->RotateFromPoint(centroidPosition, { x,y,z });
	}
	rotation = { x + rotation.x, y + rotation.y, z + rotation.z };
}

Vector3 CompositeModel::GetRotation()
{
	return rotation;
}

void CompositeModel::ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale)
{
	for (const auto& [key, model] : modelsMap)
	{
		model->ScaleFromPoint(point, scale);
	}
}

void CompositeModel::RotateFromPoint(Vector4 globalPoint, DirectX::XMFLOAT3 ratation)
{
	for (const auto& [key, model] : modelsMap)
	{
		model->RotateFromPoint(globalPoint, ratation);
	}
}

void CompositeModel::Draw(std::shared_ptr<Camera> camera)
{
	this->centroidCoursor->SetPosition(Vector3(centroidPosition));
	this->centroidCoursor->Draw(camera);
}

void CompositeModel::RenderGUI()
{
	if (modelsMap.size() == 0)
	{
		return;
	}
	if (modelsMap.size() == 1)
	{
		for (auto [_,model]:modelsMap)
		{
			model->RenderGUI();
		}
		return;
	}
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
	Vector3 rotationEuler = { XMConvertToDegrees(rotation.x),  XMConvertToDegrees(rotation.y),XMConvertToDegrees(rotation.z) };
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

std::vector<std::shared_ptr<IModel>> CompositeModel::GetContainingModels()
{
	std::vector<std::shared_ptr<IModel>> modelsVec;
	for (auto [_,model] : modelsMap)
	{
		modelsVec.push_back(model);
	}
	return modelsVec;

}


void CompositeModel::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<CompositeModel>());
}
