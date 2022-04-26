#include "vsConstColor.h"
#include "dxDevice.h"
using namespace DirectX::SimpleMath;

VSConstColor::VSConstColor()
{
	const auto vsBytes = DxDevice::LoadByteCode(L"vsConstColor.cso");

	m_vertexShader = DxDevice::instance->CreateVertexShader(vsBytes);
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
	{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_layout = DxDevice::instance->CreateInputLayout(elements, vsBytes);
	constantBufferVS = DxDevice::instance->CreateConstantBuffer<Matrix>();
}

void VSConstColor::SetVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	ID3D11Buffer* vbs[] = { vertexBuffer };
	UINT strides[] = { sizeof(VSConstColorIn) };
	UINT offsets[] = { 0 };
	DxDevice::instance->context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);
}

void VSConstColor::SetConstantBuffer(Matrix mvp)
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferVS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &mvp, sizeof(Matrix));
	DxDevice::instance->context()->Unmap(constantBufferVS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferVS.get() };
	DxDevice::instance->context()->VSSetConstantBuffers(0, 1, cbs);
}
