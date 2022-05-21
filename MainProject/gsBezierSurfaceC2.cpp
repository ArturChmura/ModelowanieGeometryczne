#include "gsBezierSurfaceC2.h"
#include "dxDevice.h"
using namespace DirectX::SimpleMath;
GSBezierSurfaceC2::GSBezierSurfaceC2()
{
	const auto gsBytes = DxDevice::LoadByteCode(L"gsBezierSurfaceC2.cso");
	m_geometryShader = DxDevice::instance->CreateGeometryShader(gsBytes);
	constantBufferGS = DxDevice::instance->CreateConstantBuffer<GSBezierSurfaceC2ConstantBuffer>();
}

void GSBezierSurfaceC2::SetConstantBuffer(GSBezierSurfaceC2ConstantBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferGS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &buffer, sizeof(GSBezierSurfaceC2ConstantBuffer));
	DxDevice::instance->context()->Unmap(constantBufferGS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferGS.get() };
	DxDevice::instance->context()->GSSetConstantBuffers(0, 1, cbs);
}
