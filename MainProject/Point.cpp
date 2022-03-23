#include "Point.h"
#include "Vertex.h"
#include "MVPColorConstantBuffer.h"
#include "ImGui/imgui.h"
using namespace DirectX;

Point::Point(DirectX::XMFLOAT3 translation)
	: IModel()
{
	const auto vsBytes = DxDevice::LoadByteCode(L"vsSingleColor.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
	shaderInfo.m_pixelShader = DxDevice::instance->CreatePixelShader(psBytes);
	shaderInfo.m_vertexShader = DxDevice::instance->CreateVertexShader(vsBytes);
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
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

DirectX::XMFLOAT3 Point::GetScale()
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

DirectX::XMFLOAT3 Point::GetTranslation()
{
	return translation;
}

void Point::SetRotation(float x, float y, float z)
{
}

DirectX::XMFLOAT3 Point::GetRotation()
{
	return { 0,0,0 };
}

XMMATRIX Point::GetModelMatrix()
{
	return XMMatrixTranslation(translation.x, translation.y, translation.z);
}

void Point::ScaleFromPoint(DirectX::XMFLOAT3 globalPoint, DirectX::XMFLOAT3 scale)
{
	auto modelMatrix = GetModelMatrix();
	auto inverseMatrix = XMMatrixInverse(nullptr, modelMatrix);
	auto localPoint = XMVector3Transform(XMLoadFloat3(&globalPoint), inverseMatrix);
	auto scaleMatrix =
		XMMatrixTranslationFromVector(-1.0f * localPoint) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslationFromVector(localPoint);

	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, modelMatrix * scaleMatrix);
	this->translation = { m._41, m._42, m._43 };
}

void Point::RotateFromPoint(DirectX::XMFLOAT3 globalPoint, DirectX::XMFLOAT3 rotation)
{
	auto modelMatrix = GetModelMatrix();
	auto inverseMatrix = XMMatrixInverse(nullptr,modelMatrix);
	auto localPoint = XMVector3Transform(XMLoadFloat3(&globalPoint), inverseMatrix);

	auto rotationMatrix =
		XMMatrixTranslationFromVector(-1.0f * localPoint) *
		XMMatrixRotationX(rotation.x) *
		XMMatrixRotationY(rotation.y) *
		XMMatrixRotationZ(rotation.z) *
		XMMatrixTranslationFromVector(localPoint);

	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, modelMatrix * rotationMatrix);
	this->translation = { m._41, m._42, m._43 };
}

void Point::Draw( std::shared_ptr<Camera> camera)
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
	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	constantBufferStruct.mvp =
		GetModelMatrix() *
		XMLoadFloat4x4(&v) *
		XMLoadFloat4x4(&p);
	constantBufferStruct.color = XMLoadFloat3(&meshInfo.color);
	memcpy(res.pData, &constantBufferStruct, sizeof(MVPColorConstantBuffer));
	DxDevice::instance->context()->Unmap(shaderInfo.constantBuffer.get(), 0);


	ID3D11Buffer* cbs[] = { shaderInfo.constantBuffer.get() };
	DxDevice::instance->context()->VSSetConstantBuffers(0, 1, cbs);

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
