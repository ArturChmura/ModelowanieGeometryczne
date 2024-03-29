#include "gsBezier.h"
#include "dxDevice.h"
using namespace DirectX::SimpleMath;
GSBezier::GSBezier()
{
	const auto gsBytes = DxDevice::LoadByteCode(L"gsBezier.cso");
	m_geometryShader = DxDevice::instance->CreateGeometryShader(gsBytes);
	constantBufferGS = DxDevice::instance->CreateConstantBuffer<GSBezierConstantBuffer>();
}

void GSBezier::SetConstantBuffer(GSBezierConstantBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferGS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &buffer, sizeof(GSBezierConstantBuffer));
	DxDevice::instance->context()->Unmap(constantBufferGS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferGS.get() };
	DxDevice::instance->context()->GSSetConstantBuffers(0, 1, cbs);
}
