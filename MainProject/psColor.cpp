#include "psColor.h"
#include "dxDevice.h"

PSColor::PSColor()
{
	const auto psBytes = DxDevice::LoadByteCode(L"psColor.cso");
	m_pixelShader = DxDevice::instance->CreatePixelShader(psBytes);
}
