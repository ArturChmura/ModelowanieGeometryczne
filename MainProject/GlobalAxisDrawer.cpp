//#include "GlobalAxisDrawer.h"
//#include "Vertex.h"
//#include "MVPConstantBuffer.h"
//GlobalAxisDrawer::GlobalAxisDrawer()
//{
//	const auto vsBytes = DxDevice::LoadByteCode(L"vsVertexColor.cso");
//	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
//	shaderInfo.m_pixelShader = DxDevice::instance->CreatePixelShader(psBytes);
//	shaderInfo.m_vertexShader = DxDevice::instance->CreateVertexShader(vsBytes);
//	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
//
//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
//
//	};
//	shaderInfo.m_layout = DxDevice::instance->CreateInputLayout(elements, vsBytes);
//	float length = 1000.0f;
//	std::vector<VertexColor> vertices = {
//		{{0,0,0},{1,0,0}},
//		{{length,0,0},{1,0,0}},
//
//		{{0,0,0},{0,1,0}},
//		{{0,length,0},{0,1,0}},
//
//		{{0,0,0},{0,0,1}},
//		{{0,0,length},{0,0,1}}
//
//	};
//
//	vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
//}
//
//void GlobalAxisDrawer::Draw(std::shared_ptr<Camera> camera)
//{
//	DxDevice::instance->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//	DxDevice::instance->context()->IASetInputLayout(shaderInfo.m_layout.get());
//	DxDevice::instance->context()->VSSetShader(shaderInfo.m_vertexShader.get(), nullptr, 0);
//	DxDevice::instance->context()->PSSetShader(shaderInfo.m_pixelShader.get(), nullptr, 0);
//
//	ID3D11Buffer* vbs[] = { vertexBuffer.get() };
//	UINT strides[] = { sizeof(VertexColor) };
//	UINT offsets[] = { 0 };
//	DxDevice::instance->context()->IASetVertexBuffers(
//		0, 1, vbs, strides, offsets);
//
//	shaderInfo.constantBuffer = DxDevice::instance->CreateConstantBuffer<MVPConstantBuffer>();
//	D3D11_MAPPED_SUBRESOURCE res;
//	DirectX::XMMATRIX mvp;
//	DxDevice::instance->context()->Map(shaderInfo.constantBuffer.get(), 0,
//		D3D11_MAP_WRITE_DISCARD, 0, &res);
//
//	auto constantBufferStruct = MVPConstantBuffer();
//	auto m = DirectX::XMMatrixIdentity();
//	auto v = camera->GetViewMatrix();
//	auto p = camera->GetPerspectiveMatrix();
//	constantBufferStruct.mvp =
//		m *
//		XMLoadFloat4x4(&v) *
//		XMLoadFloat4x4(&p);
//	memcpy(res.pData, &constantBufferStruct, sizeof(MVPConstantBuffer));
//	DxDevice::instance->context()->Unmap(shaderInfo.constantBuffer.get(), 0);
//
//
//	ID3D11Buffer* cbs[] = { shaderInfo.constantBuffer.get() };
//	DxDevice::instance->context()->VSSetConstantBuffers(0, 1, cbs);
//
//	auto count = 6;
//	DxDevice::instance->context()->Draw(count, 0);
//}
