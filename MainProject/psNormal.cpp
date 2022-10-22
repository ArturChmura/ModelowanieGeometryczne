#include "psNormal.h"
#include "dxDevice.h"

PSNormal::PSNormal()
{
	const auto psBytes = DxDevice::LoadByteCode(L"psNormal.cso");
	m_pixelShader = DxDevice::instance->CreatePixelShader(psBytes);
}
