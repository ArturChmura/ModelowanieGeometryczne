#include "Point.h"
#include "MVPColorConstantBuffer.h"
#include "ImGui/imgui.h"
#include "ShadersManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Point::Point(DirectX::SimpleMath::Vector3 location)
	: IModel("Point")
{
	this->SetTranslation(location.x, location.y, location.z);
	float halfSideLength = 0.1f;
	std::vector<VSConstColorIn> vertices = {
		{ Vector3(-halfSideLength, -halfSideLength, -halfSideLength)},
		{ Vector3(-halfSideLength, halfSideLength, -halfSideLength)},
		{ Vector3(halfSideLength, halfSideLength, -halfSideLength)},
		{ Vector3(halfSideLength, -halfSideLength, -halfSideLength)},
		{ Vector3(-halfSideLength, -halfSideLength, halfSideLength)},
		{ Vector3(-halfSideLength, halfSideLength, halfSideLength)},
		{ Vector3(halfSideLength, halfSideLength, halfSideLength)},
		{ Vector3(halfSideLength, -halfSideLength, halfSideLength)}
			};
	std::vector<int> indices = {
		0,1,2,
		0,2,3, // front

		6,5,4,
		7,6,4, // back

		1,5,2,
		5,6,2, // top

		0,3,4,
		3,7,4,// bottom

		0,4,1,
		4,5,1, // left

		3,2,6,
		3,6,7 //right

	};
	this->verticesCount = vertices.size();
	this->indicesCount = indices.size();
	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
	this->meshInfo.indexBuffer = DxDevice::instance->CreateVertexBuffer(indices);
	this->meshInfo.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

}

void Point::SetScale(float x, float y, float z)
{
}

void Point::Scale(float x, float y, float z)
{
}

Vector3 Point::GetScale()
{
	return { 1,1,1 };
}

void Point::SetTranslation(float x, float y, float z)
{
	this->translation = { x,y,z, 1 };
	OnModelChange();
}

void Point::Translate(float x, float y, float z)
{
	translation = { translation.x + x, translation.y + y, translation.z + z, 1 };
	OnModelChange();
}

Vector4 Point::GetTranslation()
{
	return translation;
}

void Point::SetRotation(float pitch, float yaw, float roll)
{
}

void Point::Rotate(float x, float y, float z)
{
}

Vector3 Point::GetRotation()
{
	return { 0,0,0 };
}

void Point::OnSelect()
{
	IModel::OnSelect();
	for (auto f : onSelectCallback)
		f(IModel::downcasted_shared_from_this<Point>());
}

void Point::OnAddedToScene()
{
	IModel::OnAddedToScene();
	for (auto f : onAddedToSceneCallback)
		f(IModel::downcasted_shared_from_this<Point>());
}

void Point::OnRemovedFromScene()
{
	IModel::OnRemovedFromScene();
	for (auto f : onRemovedFromSceneCallback)
		std::get<1>(f)(IModel::downcasted_shared_from_this<Point>());
}

Matrix Point::GetModelMatrix()
{
	return Matrix::CreateTranslation(translation.x, translation.y, translation.z);
}

void Point::ScaleFromPoint(Vector4 globalPoint, DirectX::XMFLOAT3 scale)
{
	auto scaleMatrix =
		XMMatrixTranslationFromVector(-1.0f * globalPoint) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslationFromVector(1.0 * globalPoint);

	auto m = GetModelMatrix() * scaleMatrix;
	this->translation = { m._41, m._42, m._43 };
	OnModelChange();
}

void Point::RotateFromPoint(Vector4 globalPoint, DirectX::XMFLOAT3 rotation)
{
	auto rotationMatrix =
		XMMatrixTranslationFromVector(-1.0f * globalPoint) *
		XMMatrixRotationY(rotation.y) *
		XMMatrixRotationX(rotation.x) *
		XMMatrixRotationZ(rotation.z) *
		XMMatrixTranslationFromVector(1 * globalPoint);

	auto modelMatrix = GetModelMatrix();
	auto m = modelMatrix * rotationMatrix;
	this->translation = { m._41, m._42, m._43, 1 };
	OnModelChange();
}

void Point::Draw(std::shared_ptr<Camera> camera)
{
	meshInfo.SetUpRender();
	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsConstColor->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsConstColor->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(nullptr, nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psConstColor->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsConstColor->SetVertexBuffer(meshInfo.vertexBuffer.get());

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp = GetModelMatrix() * v * p;

	ShadersManager::vsConstColor->SetConstantBuffer(mvp);

	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	DxDevice::instance->context()->DrawIndexed(indicesCount, 0, 0);
}


void Point::RenderGUI()
{
	IModel::RenderGUI();


	ImGui::Text("Translation");
	if (
		ImGui::DragFloat("x##xTransation", &translation.x)
		|| ImGui::DragFloat("y##yTransation", &translation.y)
		|| ImGui::DragFloat("z##zTransation", &translation.z)
		)
	{
		OnModelChange();
	}
}

void Point::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	this->meshInfo.color = color;
}

void Point::OnModelChange()
{
	for (auto f : onModelChangeCallback) std::get<1>(f)(IModel::downcasted_shared_from_this<Point>());
}