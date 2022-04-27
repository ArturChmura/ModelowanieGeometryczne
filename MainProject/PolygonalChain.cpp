#include "PolygonalChain.h"
#include "ShadersManager.h"
using namespace DirectX::SimpleMath;

PolygonalChain::PolygonalChain(std::vector<DirectX::SimpleMath::Vector3> points)
{

	float halfSideLength = 0.1f;
	std::vector<VSConstColorIn> vertices(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		vertices[i] = { points[i] };
	}
	std::vector<int> indices(max(points.size() * 2 - 2, 0));
	for (int i = 1; i < points.size(); i++)
	{
		indices[2 * i - 1 - 1] = i - 1;
		indices[2 * i - 1] = i;

	}
	
	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
	this->meshInfo.indexBuffer = DxDevice::instance->CreateVertexBuffer(indices);
	this->meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
	this->meshInfo.color = { 0, 0.5f, 1.0f };
	indexCount = indices.size();
}

void PolygonalChain::Draw(std::shared_ptr<Camera> camera)
{
	meshInfo.SetUpRender();
	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsConstColor->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsConstColor->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(nullptr, nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psConstColor->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsConstColor->SetVertexBuffer(meshInfo.vertexBuffer.get());

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp = v * p;


	ShadersManager::vsConstColor->SetConstantBuffer(mvp);

	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	DxDevice::instance->context()->DrawIndexed(indexCount, 0, 0);
}
