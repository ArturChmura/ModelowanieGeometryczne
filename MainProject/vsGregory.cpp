#include "vsGregory.h"
#include "dxDevice.h"
using namespace DirectX::SimpleMath;

VSGregory::VSGregory()
{
	const auto vsBytes = DxDevice::LoadByteCode(L"vsGregory.cso");

	m_vertexShader = DxDevice::instance->CreateVertexShader(vsBytes);

	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
	{ "SV_POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_layout = DxDevice::instance->CreateInputLayout(elements, vsBytes);
}

void VSGregory::SetVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	ID3D11Buffer* vbs[] = { vertexBuffer };
	UINT strides[] = { sizeof(VSGregoryIn) };
	UINT offsets[] = { 0 };
	DxDevice::instance->context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);
}


