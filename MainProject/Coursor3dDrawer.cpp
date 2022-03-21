#include "Coursor3dDrawer.h"
#include "Vertex.h"
#include "MVPConstantBuffer.h"

Coursor3dDrawer::Coursor3dDrawer(std::shared_ptr<DxDevice> dxDevice, std::shared_ptr<Coursor3d> coursor)
{
	this->coursor3d = coursor;
	const auto vsBytes = DxDevice::LoadByteCode(L"vsVertexColor.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
	m_pixelShader = dxDevice->CreatePixelShader(psBytes);
	m_vertexShader = dxDevice->CreateVertexShader(vsBytes);
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{

	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};
	m_layout = dxDevice->CreateInputLayout(elements, vsBytes);
	float length = 1.0f;
	std::vector<VertexColor> vertices = {
		{{0,0,0},{1,0,0}},
		{{length,0,0},{1,0,0}},

		{{0,0,0},{0,1,0}},
		{{0,length,0},{0,1,0}},

		{{0,0,0},{0,0,1}},
		{{0,0,length},{0,0,1}}

	};

	vertexBuffer = dxDevice->CreateVertexBuffer(vertices);
}

void Coursor3dDrawer::Draw(std::shared_ptr<DxDevice> m_device, std::shared_ptr<Camera> camera)
{
	m_device->context()->IASetInputLayout(m_layout.get());
	m_device->context()->VSSetShader(m_vertexShader.get(), nullptr, 0);
	m_device->context()->PSSetShader(m_pixelShader.get(), nullptr, 0);

	ID3D11Buffer* vbs[] = { vertexBuffer.get() };
	UINT strides[] = { sizeof(VertexColor) };
	UINT offsets[] = { 0 };
	m_device->context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);

	constantBuffer = m_device->CreateConstantBuffer<MVPConstantBuffer>();
	D3D11_MAPPED_SUBRESOURCE res;
	DirectX::XMMATRIX mvp;
	m_device->context()->Map(constantBuffer.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);

	auto constantBufferStruct = MVPConstantBuffer();
	auto m = coursor3d->GetModelMatrix();
	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	constantBufferStruct.mvp =
		m *
		XMLoadFloat4x4(&v) *
		XMLoadFloat4x4(&p);
	memcpy(res.pData, &constantBufferStruct, sizeof(MVPConstantBuffer));
	m_device->context()->Unmap(constantBuffer.get(), 0);


	ID3D11Buffer* cbs[] = { constantBuffer.get() };
	m_device->context()->VSSetConstantBuffers(0, 1, cbs);

	auto count = 6;
	m_device->context()->Draw(count, 0);
}
