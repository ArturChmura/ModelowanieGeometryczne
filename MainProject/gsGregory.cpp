#include "gsGregory.h"

#include "dxDevice.h"
using namespace DirectX::SimpleMath;

GSGregory::GSGregory()
{
	const auto vsBytes = DxDevice::LoadByteCode(L"gsGregory.cso");
	m_geometryShader = DxDevice::instance->CreateGeometryShader(vsBytes);
	constantBufferVS = DxDevice::instance->CreateConstantBuffer<GSGregoryConstantBuffer>();
}


void GSGregory::SetConstantBuffer(GSGregoryConstantBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE res;
	DxDevice::instance->context()->Map(constantBufferVS.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &buffer, sizeof(GSGregoryConstantBuffer));
	DxDevice::instance->context()->Unmap(constantBufferVS.get(), 0);
	ID3D11Buffer* cbs[] = { constantBufferVS.get() };
	DxDevice::instance->context()->GSSetConstantBuffers(0, 1, cbs);
}