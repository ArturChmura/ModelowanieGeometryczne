#include "VertexModel.h"
#include "MVPColorConstantBuffer.h"
#include "Vertex.h"
#include "ImGui/imgui.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

VertexModel::VertexModel()
{
	const auto vsBytes = DxDevice::LoadByteCode(L"vsSingleColor.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
	shaderInfo.m_pixelShader = DxDevice::instance->CreatePixelShader(psBytes);
	shaderInfo.m_vertexShader = DxDevice::instance->CreateVertexShader(vsBytes);
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	shaderInfo.m_layout = DxDevice::instance->CreateInputLayout(elements, vsBytes);
	this->scale = { 1,1,1 };
	this->translation = { 0,0,0 };
	this->rotation = { 0,0,0 };
	UpdateModelMatrix();
}

DirectX::XMFLOAT3 VertexModel::GetColor()
{
	return meshInfo.color;
}

void VertexModel::SetColor(DirectX::XMFLOAT3 color)
{
	meshInfo.color = color;
}

void VertexModel::Draw(std::shared_ptr<Camera> camera)
{
	DxDevice::instance->context()->IASetPrimitiveTopology(meshInfo.topology);
	DxDevice::instance->context()->IASetInputLayout(shaderInfo.m_layout.get());
	DxDevice::instance->context()->VSSetShader(shaderInfo.m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->PSSetShader(shaderInfo.m_pixelShader.get(), nullptr, 0);

	ID3D11Buffer* vbs[] = { meshInfo.vertexBuffer.get() };
	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };
	DxDevice::instance->context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);
	DxDevice::instance->context()->IASetIndexBuffer(meshInfo.indexBuffer.get(),
		DXGI_FORMAT_R32_UINT, 0);

	shaderInfo.constantBuffer = DxDevice::instance->CreateConstantBuffer<MVPColorConstantBuffer>();
	D3D11_MAPPED_SUBRESOURCE res;
	DirectX::XMMATRIX mvp;
	DxDevice::instance->context()->Map(shaderInfo.constantBuffer.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);

	auto constantBufferStruct = MVPColorConstantBuffer();
	auto m = modelMatrix;
	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	constantBufferStruct.mvp =
		XMLoadFloat4x4(&m) *
		XMLoadFloat4x4(&v) *
		XMLoadFloat4x4(&p);
	constantBufferStruct.color = XMLoadFloat3(&meshInfo.color);
	memcpy(res.pData, &constantBufferStruct, sizeof(MVPColorConstantBuffer));
	DxDevice::instance->context()->Unmap(shaderInfo.constantBuffer.get(), 0);


	ID3D11Buffer* cbs[] = { shaderInfo.constantBuffer.get() };
	DxDevice::instance->context()->VSSetConstantBuffers(0, 1, cbs);

	auto count = GetIndicesCount();
	DxDevice::instance->context()->DrawIndexed(count, 0, 0);
}



void VertexModel::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
	UpdateModelMatrix();
}

DirectX::XMFLOAT3 VertexModel::GetScale()
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

DirectX::XMFLOAT3 VertexModel::GetTranslation()
{
	return translation;
}

void VertexModel::SetRotation(float x, float y, float z)
{
	rotation = { x,y,z };
	UpdateModelMatrix();
}

XMFLOAT3 VertexModel::GetRotation()
{
	return rotation;
}

void VertexModel::ScaleFromPoint(XMFLOAT3 globalPoint, XMFLOAT3 scale)
{
	auto modelMatrix = this->modelMatrix;
	auto inverseMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&modelMatrix));
	auto localPoint = XMVector3Transform(XMLoadFloat3(&globalPoint), inverseMatrix);
	auto scaleMatrix =
		XMMatrixTranslationFromVector(-1.0f * localPoint) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslationFromVector(localPoint);

	XMStoreFloat4x4(&this->modelMatrix, scaleMatrix * XMLoadFloat4x4(&modelMatrix) );
}

void VertexModel::RotateFromPoint(XMFLOAT3 globalPoint, XMFLOAT3 rotation)
{
	auto elo = this->modelMatrix;
	auto inverseMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&elo));
	XMFLOAT4 point4 = { globalPoint.x,globalPoint.y,globalPoint.z, 1.0f };
	auto localPoint = XMVector3Transform(XMLoadFloat4(&point4), inverseMatrix);

	auto rotationMatrix =
		XMMatrixTranslationFromVector(-1.0f * localPoint) *
		XMMatrixRotationX(rotation.x) *
		XMMatrixRotationY(rotation.y) *
		XMMatrixRotationZ(rotation.z) *
		XMMatrixTranslationFromVector(localPoint);

	this->modelMatrix = rotationMatrix * this->modelMatrix;

	Matrix m(this->modelMatrix);
	Vector3 scale;
	Quaternion quat;
	Vector3 translation;
	m.Decompose(scale, quat, translation);

	Vector3 rot = quat.ToEuler();

	this->rotation =  rot;
	this->translation =  translation;
	this->scale =  scale;

	//UpdateModelMatrix();
}

void VertexModel::UpdateModelMatrix()
{
	XMStoreFloat4x4(&modelMatrix,
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		GetRotationMatrix() *
		XMMatrixTranslation(translation.x, translation.y, translation.z)
	);
}

DirectX::XMMATRIX VertexModel::GetRotationMatrix()
{
	return
		XMMatrixRotationX(rotation.x) *
		XMMatrixRotationY(rotation.y) *
		XMMatrixRotationZ(rotation.z);
}

void VertexModel::RenderGUI()
{
	IModel::RenderGUI();
	ImGui::Text("Scale");
	float minScale = 0.1f;
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
	if (
		ImGui::DragFloat("x##xRotation", &rotation.x, 0.01f)
		|| ImGui::DragFloat("y##yRotation", &rotation.y, 0.01f)
		|| ImGui::DragFloat("z##zRotation", &rotation.z, 0.01f)
		)
	{
		UpdateModelMatrix();
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
