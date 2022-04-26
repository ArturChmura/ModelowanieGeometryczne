#include "Coursor3d.h"
#include "MVPConstantBuffer.h"

using namespace DirectX::SimpleMath;

Coursor3d::Coursor3d()
{
	this->translation = { 0,0,0 };
	
	float length = 1.0f;
	std::vector<VSColorIn> vertices = {
		{{0,0,0},{1,0,0}},
		{{length,0,0},{1,0,0}},

		{{0,0,0},{0,1,0}},
		{{0,length,0},{0,1,0}},

		{{0,0,0},{0,0,1}},
		{{0,0,length},{0,0,1}}

	};
	vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
}

void Coursor3d::Draw(std::shared_ptr<Camera> camera)
{
	

	DxDevice::instance->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	shaders.SetupRender();
	shaders.vertexShader.SetVertexBuffer(vertexBuffer.get());
	

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp =
		Matrix::CreateTranslation(Vector3(translation)) *
		v *
		p;

	shaders.vertexShader.SetConstantBuffer(mvp);

	auto count = 6;
	DxDevice::instance->context()->Draw(count, 0);
}
