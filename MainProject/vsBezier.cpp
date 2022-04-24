#include "vsBezier.h"
#include "dxDevice.h"

VSBezier::VSBezier()
{
	const auto vsBytes = DxDevice::LoadByteCode(L"vsBezier.cso");

	m_vertexShader = DxDevice::instance->CreateVertexShader(vsBytes);

	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{

	{ "SV_POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "Y", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VSBezierIn, Y), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "Z", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VSBezierIn, Z), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "SIZE", 0, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0, offsetof(VSBezierIn, SIZE), D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	m_layout = DxDevice::instance->CreateInputLayout(elements, vsBytes);
}

void VSBezier::SetVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	ID3D11Buffer* vbs[] = { vertexBuffer };
	UINT strides[] = { sizeof(VSBezierIn) };
	UINT offsets[] = { 0 };
	DxDevice::instance->context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);

}
