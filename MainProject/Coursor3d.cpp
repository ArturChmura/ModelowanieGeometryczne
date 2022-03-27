#include "Coursor3d.h"
#include "MVPConstantBuffer.h"
#include "Vertex.h"

Coursor3d::Coursor3d()
{
	this->translation = { 0,0,0 };
	
	float length = 1.0f;
	std::vector<VertexColor> vertices = {
		{{0,0,0},{1,0,0}},
		{{length,0,0},{1,0,0}},

		{{0,0,0},{0,1,0}},
		{{0,length,0},{0,1,0}},

		{{0,0,0},{0,0,1}},
		{{0,0,length},{0,0,1}}

	};
	shaderInfoVertexColor = std::make_shared<ShaderInfoVertexColor>();
	vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
}

void Coursor3d::Draw(std::shared_ptr<Camera> camera)
{
	shaderInfoVertexColor->SetUpRender();
	DxDevice::instance->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	shaderInfoVertexColor->SetVertexBuffer(vertexBuffer.get());

	

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	shaderInfoVertexColor->constantBufferStruct.mvp =
		DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z) *
		XMLoadFloat4x4(&v) *
		XMLoadFloat4x4(&p);
	
	shaderInfoVertexColor->CopyConstantBuffers();
	auto count = 6;
	DxDevice::instance->context()->Draw(count, 0);
}
