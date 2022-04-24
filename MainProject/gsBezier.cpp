#include "gsBezier.h"
#include "dxDevice.h"
#include <utility> 
using namespace DirectX::SimpleMath;
GSBezier::GSBezier()
{
	const auto gsBytes = DxDevice::LoadByteCode(L"gsBezier.cso");
	m_geometryShader = DxDevice::instance->CreateGeometryShader(gsBytes);
	constantBufferGS = DxDevice::instance->CreateConstantBuffer<Matrix>();
}

void GSBezier::SetConstantBuffer(GSBezierConstantBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferGS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &buffer, sizeof(Matrix) + 2*sizeof(int));
	DxDevice::instance->context()->Unmap(constantBufferGS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferGS.get() };
	DxDevice::instance->context()->GSSetConstantBuffers(0, 1, cbs);
}
