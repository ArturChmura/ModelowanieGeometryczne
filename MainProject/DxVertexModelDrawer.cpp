#include "DxVertexModelDrawer.h"
#include <stdio.h>
#include "MVPColorConstantBuffer.h"

DxVertexModelDrawer::DxVertexModelDrawer(std::shared_ptr<DxDevice> dxDevice, std::shared_ptr<VertexModel> vertexModel)
{
	const auto vsBytes = DxDevice::LoadByteCode(L"vsSingleColor.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
	m_pixelShader = dxDevice->CreatePixelShader(psBytes);
	m_vertexShader = dxDevice->CreateVertexShader(vsBytes);
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_layout = dxDevice->CreateInputLayout(elements, vsBytes);

	this->vertexModel = vertexModel;
	this->dxDevicce = dxDevice;
	vertexModel->act.push_back([this]() {UpdateBuffers(); });
	UpdateBuffers();
}

void DxVertexModelDrawer::Draw(std::shared_ptr<DxDevice> m_device, std::shared_ptr <Camera> camera)
{
	m_device->context()->IASetInputLayout(m_layout.get());
	m_device->context()->VSSetShader(m_vertexShader.get(), nullptr, 0);
	m_device->context()->PSSetShader(m_pixelShader.get(), nullptr, 0);

	ID3D11Buffer* vbs[] = { vertexBuffer.get() };
	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };
	m_device->context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);
	m_device->context()->IASetIndexBuffer(indexBuffer.get(),
		DXGI_FORMAT_R32_UINT, 0);

	constantBuffer = m_device->CreateConstantBuffer<MVPColorConstantBuffer>();
	D3D11_MAPPED_SUBRESOURCE res;
	DirectX::XMMATRIX mvp;
	m_device->context()->Map(constantBuffer.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);

	auto constantBufferStruct = MVPColorConstantBuffer();
	auto m = vertexModel->GetModelMatrix();
	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	constantBufferStruct.mvp =
		XMLoadFloat4x4(&m) *
		XMLoadFloat4x4(&v) *
		XMLoadFloat4x4(&p);
	constantBufferStruct.color = XMLoadFloat3(&vertexModel->color);
	memcpy(res.pData, &constantBufferStruct, sizeof(MVPColorConstantBuffer));
	m_device->context()->Unmap(constantBuffer.get(), 0);


	ID3D11Buffer* cbs[] = { constantBuffer.get() };
	m_device->context()->VSSetConstantBuffers(0, 1, cbs);

	auto count = vertexModel->GetIndicesCount();
	m_device->context()->DrawIndexed(count, 0, 0);
}

void DxVertexModelDrawer::UpdateBuffers()
{
	vertexBuffer = dxDevicce->CreateVertexBuffer(vertexModel->GetVertices());
	indexBuffer = dxDevicce->CreateVertexBuffer(vertexModel->GetIndices());
}
