#include "LineColorShaders.h"
#include "dxDevice.h"
LineColorShaders::LineColorShaders()
	: vertexShader(),  pixelShader()
{
}

void LineColorShaders::SetupRender()
{
	DxDevice::instance->context()->IASetInputLayout(vertexShader.m_layout.get());
	DxDevice::instance->context()->VSSetShader(vertexShader.m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(nullptr, nullptr, 0);
	DxDevice::instance->context()->PSSetShader(pixelShader.m_pixelShader.get(), nullptr, 0);
}
