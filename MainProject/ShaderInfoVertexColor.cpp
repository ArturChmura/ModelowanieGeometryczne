#include "ShaderInfoVertexColor.h"
#include "dxDevice.h"
#include  "Vertex.h"

ShaderInfoVertexColor::ShaderInfoVertexColor()
{
	const auto vsBytes = DxDevice::LoadByteCode(L"vsVertexColor.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
	m_pixelShader = DxDevice::instance->CreatePixelShader(psBytes);
	m_vertexShader = DxDevice::instance->CreateVertexShader(vsBytes);
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{

	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};
	m_layout = DxDevice::instance->CreateInputLayout(elements, vsBytes);
}

void ShaderInfoVertexColor::SetUpRender()
{
	DxDevice::instance->context()->IASetInputLayout(m_layout.get());
	DxDevice::instance->context()->VSSetShader(m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->PSSetShader(m_pixelShader.get(), nullptr, 0);
	constantBuffer = DxDevice::instance->CreateConstantBuffer<MVPConstantBuffer>();
	DxDevice::instance->context()->Map(constantBuffer.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
}

void ShaderInfoVertexColor::SetVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	ID3D11Buffer* vbs[] = { vertexBuffer };
	UINT strides[] = { sizeof(VertexColor) };
	UINT offsets[] = { 0 };
	DxDevice::instance->context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);

}

void ShaderInfoVertexColor::CopyConstantBuffers()
{
	memcpy(res.pData, &constantBufferStruct, sizeof(MVPConstantBuffer));
	DxDevice::instance->context()->Unmap(constantBuffer.get(), 0);


	ID3D11Buffer* cbs[] = { constantBuffer.get() };
	DxDevice::instance->context()->VSSetConstantBuffers(0, 1, cbs);
}
