#include "ShaderInfoPolynomial.h"
#include "dxDevice.h"
#include "Vertex.h"

ShaderInfoPolynomial::ShaderInfoPolynomial()
{
	const auto vsBytes = DxDevice::LoadByteCode(L"vsPolynomial.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"psConstColor.cso");
	const auto gsBytes = DxDevice::LoadByteCode(L"gsPolynomial.cso");

	m_pixelShader = DxDevice::instance->CreatePixelShader(psBytes);
	m_vertexShader = DxDevice::instance->CreateVertexShader(vsBytes);
	m_geometryShader = DxDevice::instance->CreateGeometryShader(gsBytes);

	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{

	{ "SV_POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "B", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Coef, B), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "C", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Coef, C), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "D", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Coef, D), D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	m_layout = DxDevice::instance->CreateInputLayout(elements, vsBytes);
	constantBufferGS = DxDevice::instance->CreateConstantBuffer<DirectX::XMVECTOR>();
	constantBufferPS = DxDevice::instance->CreateConstantBuffer<DirectX::XMMATRIX>();
}


void ShaderInfoPolynomial::SetUpRender()
{
	DxDevice::instance->context()->IASetInputLayout(m_layout.get());
	DxDevice::instance->context()->VSSetShader(m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(m_geometryShader.get(), nullptr, 0);
	DxDevice::instance->context()->PSSetShader(m_pixelShader.get(), nullptr, 0);
}

void ShaderInfoPolynomial::SetVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	ID3D11Buffer* vbs[] = { vertexBuffer };
	UINT strides[] = { sizeof(Coef) };
	UINT offsets[] = { 0 };
	DxDevice::instance->context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);
}

void ShaderInfoPolynomial::CopyConstantBuffers()
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferGS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &constantBufferStruct.mvp, sizeof(DirectX::XMMATRIX));
	DxDevice::instance->context()->Unmap(constantBufferGS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferGS.get() };
	DxDevice::instance->context()->GSSetConstantBuffers(0, 1, cbs);

	DxDevice::instance->context()->Map(constantBufferPS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &constantBufferStruct.color, sizeof(DirectX::XMMATRIX));
	DxDevice::instance->context()->Unmap(constantBufferPS.get(), 0);
	ID3D11Buffer* cbs2[] = { constantBufferPS.get() };
	DxDevice::instance->context()->PSSetConstantBuffers(0, 1, cbs2);
}