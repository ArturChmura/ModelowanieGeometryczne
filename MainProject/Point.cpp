#include "Point.h"
#include "Vertex.h"
#include "MVPColorConstantBuffer.h"
#include "ImGui/imgui.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Point::Point(DirectX::XMFLOAT3 translation)
	: IModel("Point")
{
	this->shaderInfoSingleColorVs = std::make_shared< ShaderInfoSingleColorVs>();
	this->SetTranslation(translation.x, translation.y, translation.z);
	float halfSideLength = 0.1f;
	std::vector<Vertex> vertices = {
		{{-halfSideLength, -halfSideLength, -halfSideLength}},
		{{-halfSideLength, halfSideLength, -halfSideLength}},
		{{halfSideLength, halfSideLength, -halfSideLength}},
		{{halfSideLength, -halfSideLength, -halfSideLength}},
		{{-halfSideLength, -halfSideLength, halfSideLength}},
		{{-halfSideLength, halfSideLength, halfSideLength}},
		{{halfSideLength, halfSideLength, halfSideLength}},
		{{halfSideLength, -halfSideLength, halfSideLength}}
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

Vector3 Point::GetScale()
{
	return { 1,1,1 };
}

void Point::SetTranslation(float x, float y, float z)
{
	this->translation = { x,y,z };
}

void Point::Translate(float x, float y, float z)
{
	translation = { translation.x + x, translation.y + y, translation.z + z };
}

Vector3 Point::GetTranslation()
{
	return translation;
}

void Point::SetRotation(float pitch, float yaw, float roll)
{
}

Vector3 Point::GetRotation()
{
	return {0,0,0};
}

Matrix Point::GetModelMatrix()
{
	return Matrix::CreateTranslation(translation.x, translation.y, translation.z);
}

void Point::ScaleFromPoint(DirectX::XMFLOAT3 globalPoint, DirectX::XMFLOAT3 scale)
{
	auto scaleMatrix =
		XMMatrixTranslationFromVector(-1.0f * globalPoint) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslationFromVector(1.0 * globalPoint);

	auto m = GetModelMatrix() * scaleMatrix;
	this->translation = { m._41, m._42, m._43 };
}

void Point::RotateFromPoint(DirectX::XMFLOAT3 globalPoint, DirectX::XMFLOAT3 rotation)
{
	auto rotationMatrix =
		XMMatrixTranslationFromVector(-1.0f * globalPoint) *
		XMMatrixRotationY(rotation.y) *
		XMMatrixRotationX(rotation.x) *
		XMMatrixRotationZ(rotation.z) *
		XMMatrixTranslationFromVector(1 * globalPoint);

	auto modelMatrix = GetModelMatrix();
	auto m = modelMatrix * rotationMatrix;
	this->translation = { m._41, m._42, m._43 };
}

void Point::Draw( std::shared_ptr<Camera> camera)
{
	shaderInfoSingleColorVs->SetUpRender();
	meshInfo.SetUpRender();

	shaderInfoSingleColorVs->SetVertexBuffer(meshInfo.vertexBuffer.get());

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	shaderInfoSingleColorVs->constantBufferStruct.mvp =
		GetModelMatrix() *
		XMLoadFloat4x4(&v) *
		XMLoadFloat4x4(&p);
	shaderInfoSingleColorVs->constantBufferStruct.color = XMLoadFloat3(&meshInfo.color);

	shaderInfoSingleColorVs->CopyConstantBuffers();

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
	}
}

void Point::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	this->meshInfo.color = color;
}
