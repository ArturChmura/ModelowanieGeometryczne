#include "vsNormal.h"
#include "dxDevice.h"

using namespace DirectX::SimpleMath;
VSNormal::VSNormal()
{
	const auto vsBytes = DxDevice::LoadByteCode(L"vsNormal.cso");

	m_vertexShader = DxDevice::instance->CreateVertexShader(vsBytes);

	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VSNormalIn, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_layout = DxDevice::instance->CreateInputLayout(elements, vsBytes);
	constantBufferVS = DxDevice::instance->CreateConstantBuffer<VSNormalConstantBuffer>();
}

void VSNormal::SetVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	ID3D11Buffer* vbs[] = { vertexBuffer };
	UINT strides[] = { sizeof(VSNormalIn) };
	UINT offsets[] = { 0 };
	DxDevice::instance->context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);
}

void VSNormal::SetConstantBuffer(VSNormalConstantBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferVS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &buffer, sizeof(VSNormalConstantBuffer));
	DxDevice::instance->context()->Unmap(constantBufferVS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferVS.get() };
	DxDevice::instance->context()->VSSetConstantBuffers(0, 1, cbs);
}
