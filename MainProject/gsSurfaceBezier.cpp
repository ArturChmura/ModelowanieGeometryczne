#include "gsSurfaceBezier.h"
#include "dxDevice.h"
using namespace DirectX::SimpleMath;

GSSurfaceBezier::GSSurfaceBezier()
{
	const auto gsBytes = DxDevice::LoadByteCode(L"gsSurfaceBezier.cso");
	m_geometryShader = DxDevice::instance->CreateGeometryShader(gsBytes);
	constantBufferGS = DxDevice::instance->CreateConstantBuffer<GSSurfaceBezierConstantBuffer>();	

	SamplerDescription sd;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = 16;
	m_samplerWrap = DxDevice::instance->CreateSamplerState(sd);
}

void GSSurfaceBezier::SetConstantBuffer(GSSurfaceBezierConstantBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferGS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &buffer, sizeof(GSSurfaceBezierConstantBuffer));
	DxDevice::instance->context()->Unmap(constantBufferGS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferGS.get() };
	DxDevice::instance->context()->GSSetConstantBuffers(0, 1, cbs);

	auto s_ptr = m_samplerWrap.get();
	DxDevice::instance->context()->GSSetSamplers(0, 1, &s_ptr);
}


void GSSurfaceBezier::SetFilterTexture(ID3D11ShaderResourceView* texture)
{
	std::initializer_list<ID3D11ShaderResourceView*> list = { texture };
	DxDevice::instance->context()->GSSetShaderResources(0, list.size(), list.begin());
	auto s_ptr = m_samplerWrap.get();
	DxDevice::instance->context()->GSSetSamplers(0, 1, &s_ptr);
}