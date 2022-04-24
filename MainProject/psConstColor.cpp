#include "psConstColor.h"
#include "dxDevice.h"

using namespace DirectX::SimpleMath;

PSConstColor::PSConstColor()
{
	const auto psBytes = DxDevice::LoadByteCode(L"psConstColor.cso");
	m_pixelShader = DxDevice::instance->CreatePixelShader(psBytes);
	constantBufferPS = DxDevice::instance->CreateConstantBuffer<Vector4>();
}

void PSConstColor::SetConstantBuffer(DirectX::SimpleMath::Vector3 color)
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferPS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &color, sizeof(Vector3));
	DxDevice::instance->context()->Unmap(constantBufferPS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferPS.get() };
	DxDevice::instance->context()->PSSetConstantBuffers(0, 1, cbs);
}
