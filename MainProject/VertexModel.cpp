#include "VertexModel.h"
#include "MVPColorConstantBuffer.h"
#include "imgui.h"
#include <d3d11.h>
#include "SimpleMath.h"
#include "MatrixExt.h"
#include "ShadersManager.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

VertexModel::VertexModel(std::string name)
	:IModel(name)
{
	this->scale = { 1,1,1 };
	this->translation = { 0,0,0 };
	this->rotation = { 0,0,0 };
	UpdateModelMatrix();
}

Vector3 VertexModel::GetColor()
{
	return meshInfo.color;
}

void VertexModel::SetColor(DirectX::XMFLOAT3 color)
{
	meshInfo.color = color;
}

void VertexModel::Draw(std::shared_ptr<Camera> camera)
{
	meshInfo.SetUpRender();

	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsConstColor->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsConstColor->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(nullptr, nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psConstColor->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsConstColor->SetVertexBuffer(meshInfo.vertexBuffer.get());

	auto m = modelMatrix;
	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp = m * v * p;
	ShadersManager::vsConstColor->SetConstantBuffer(mvp);

	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	DxDevice::instance->context()->DrawIndexed(GetIndicesCount(), 0, 0);
}



void VertexModel::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
	UpdateModelMatrix();
}

void VertexModel::Scale(float x, float y, float z)
{
	scale *= { x,y,z };
	UpdateModelMatrix();
}

Vector3 VertexModel::GetScale()
{
	return scale;
}

void VertexModel::SetTranslation(float x, float y, float z)
{
	translation = { x,y,z };
	UpdateModelMatrix();
}

void VertexModel::Translate(float x, float y, float z)
{
	translation = { translation.x + x, translation.y + y, translation.z + z };
	UpdateModelMatrix();
}

Vector4 VertexModel::GetTranslation()
{
	return translation;
}

void VertexModel::SetRotation(float x, float y, float z)
{
	rotation = {x,y,z};
	UpdateModelMatrix();
}

void VertexModel::Rotate(float x, float y, float z)
{
	rotation += { x,y,z };
	UpdateModelMatrix();
}

Vector3 VertexModel::GetRotation()
{
	return rotation;
}

void VertexModel::ScaleFromPoint(DirectX::SimpleMath::Vector4 globalPoint, XMFLOAT3 scale)
{
	auto scaleMatrix =
		XMMatrixTranslationFromVector(-1.0f * globalPoint) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslationFromVector(1.0 * globalPoint);

	modelMatrix= modelMatrix * scaleMatrix;

	this->translation = MatrixExt::DecomposeTranslation(modelMatrix);
	this->scale = MatrixExt::DecomposeScale(modelMatrix);
	UpdateModelMatrix();
}

void VertexModel::RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, XMFLOAT3 rotation)
{
	auto rotationMatrix =
		XMMatrixTranslationFromVector(-1.0f * globalPoint) *
		XMMatrixRotationY(rotation.y) *
		XMMatrixRotationX(rotation.x) *
		XMMatrixRotationZ(rotation.z) *
		XMMatrixTranslationFromVector(1 * globalPoint);

	this->modelMatrix = this->modelMatrix * rotationMatrix;

	this->rotation = MatrixExt::DecomposeRotation(modelMatrix);
	this->translation = MatrixExt::DecomposeTranslation(modelMatrix);

	UpdateModelMatrix();
}

void VertexModel::UpdateModelMatrix()
{
	Matrix scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	auto rotationMatrix = GetRotationMatrix();
	auto transMatrix = XMMatrixTranslation(translation.x, translation.y, translation.z);
	modelMatrix = scaleMatrix;
	modelMatrix = modelMatrix * rotationMatrix;
	modelMatrix = modelMatrix * transMatrix;

}

Matrix VertexModel::GetRotationMatrix()
{
	return
		Matrix::CreateRotationX(rotation.x) *
		Matrix::CreateRotationY(rotation.y) *
		Matrix::CreateRotationZ(rotation.z);
}

void VertexModel::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	this->SetColor(color);
}

void VertexModel::RenderGUI()
{
	IModel::RenderGUI();
	ImGui::Text("Scale");
	float minScale = 0.0f;
	if (
		ImGui::DragFloat("x##xScale", &scale.x, 0.1f, minScale)
		|| ImGui::DragFloat("y##yScale", &scale.y, 0.1f, minScale)
		|| ImGui::DragFloat("z##zScale", &scale.z, 0.1f, minScale)
		)
	{
		scale = { max(scale.x, minScale), max(scale.y, minScale), max(scale.z, minScale) };
		UpdateModelMatrix();
	}

	ImGui::Text("Rotation");
	Vector3 rotationEuler = { XMConvertToDegrees(rotation.x),  XMConvertToDegrees(rotation.y),XMConvertToDegrees(rotation.z) };
	if (
		ImGui::DragFloat("x##xRotation", &rotationEuler.x, 1.0f)
		|| ImGui::DragFloat("y##yRotation", &rotationEuler.y, 1.0f)
		|| ImGui::DragFloat("z##zRotation", &rotationEuler.z, 1.0f)
		)
	{
		auto x = XMConvertToRadians(rotationEuler.x);
		auto y = XMConvertToRadians(rotationEuler.y);
		auto z = XMConvertToRadians(rotationEuler.z);


		this->SetRotation(x, y, z);
	}

	ImGui::Text("Translation");
	if (
		ImGui::DragFloat("x##xTransation", &translation.x)
		|| ImGui::DragFloat("y##yTransation", &translation.y)
		|| ImGui::DragFloat("z##zTransation", &translation.z)
		)
	{
		UpdateModelMatrix();
	}



	if (ImGui::ColorEdit3("Color", (float*)&meshInfo.color))
	{
	}
}
