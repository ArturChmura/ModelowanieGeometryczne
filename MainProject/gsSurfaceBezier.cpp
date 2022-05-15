#include "gsSurfaceBezier.h"
#include "dxDevice.h"
using namespace DirectX::SimpleMath;

GSSurfaceBezier::GSSurfaceBezier()
{
	const auto gsBytes = DxDevice::LoadByteCode(L"gsSurfaceBezier.cso");
	m_geometryShader = DxDevice::instance->CreateGeometryShader(gsBytes);
	constantBufferGS = DxDevice::instance->CreateConstantBuffer<GSSurfaceBezierConstantBuffer>();
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
}
