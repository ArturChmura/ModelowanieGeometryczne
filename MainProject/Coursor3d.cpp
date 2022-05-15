#include "Coursor3d.h"
#include "MVPConstantBuffer.h"
#include "ShadersManager.h"

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

	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsColor->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsColor->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(nullptr, nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psColor->m_pixelShader.get(), nullptr, 0);


	ShadersManager::vsColor->SetVertexBuffer(vertexBuffer.get());


	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp =
		Matrix::CreateTranslation(Vector3(translation)) *
		v *
		p;

	ShadersManager::vsColor->SetConstantBuffer(mvp);

	auto count = 6;
	DxDevice::instance->context()->Draw(count, 0);
}

Vector3 Coursor3d::GetTranslation()
{
	return Vector3(translation);
}

void Coursor3d::SetPosition(DirectX::SimpleMath::Vector3 position)
{
	translation = Vector4(position.x, position.y, position.z, 1.0f);
	
	for (auto f : OnPositionChangeCallback.functions)
	{
		f(position);
	}


	return;
}
