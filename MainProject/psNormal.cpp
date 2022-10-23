#include "psNormal.h"
#include "dxDevice.h"

PSNormal::PSNormal()
{
	const auto psBytes = DxDevice::LoadByteCode(L"psNormal.cso");
	m_pixelShader = DxDevice::instance->CreatePixelShader(psBytes);

	SamplerDescription sd;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = 16;
	m_samplerWrap = DxDevice::instance->CreateSamplerState(sd);
	constantBufferPS = DxDevice::instance->CreateConstantBuffer<PSNormalConstantBuffer>();
}


void PSNormal::SetTexture(ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* colorTexture)
{
	std::initializer_list<ID3D11ShaderResourceView*> list = { normalTexture,  colorTexture };
	DxDevice::instance->context()->PSSetShaderResources(0, list.size(), list.begin());
	auto s_ptr = m_samplerWrap.get();
	DxDevice::instance->context()->PSSetSamplers(0, 1, &s_ptr);

}

void PSNormal::SetConstantBuffer(PSNormalConstantBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferPS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &buffer, sizeof(PSNormalConstantBuffer));
	DxDevice::instance->context()->Unmap(constantBufferPS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferPS.get() };
	DxDevice::instance->context()->PSSetConstantBuffers(0, 1, cbs);
}
