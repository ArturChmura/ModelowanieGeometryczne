#include "BezierShaders.h"
#include "dxDevice.h"
BezierShaders::BezierShaders()
	: vertexShader(), geometryShader(), pixelShader()
{
}


void BezierShaders::SetupRender()
{
	DxDevice::instance->context()->IASetInputLayout(vertexShader.m_layout.get());
	DxDevice::instance->context()->VSSetShader(vertexShader.m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(geometryShader.m_geometryShader.get(), nullptr, 0);
	DxDevice::instance->context()->PSSetShader(pixelShader.m_pixelShader.get(), nullptr, 0);
}
