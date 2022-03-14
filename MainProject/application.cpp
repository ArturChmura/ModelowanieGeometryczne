#include "application.h"
#include "Cube.h"
#include "Cone.h"
#include "windowsx.h"
#include "Torus.h"

using namespace mini;
using namespace DirectX;

Application::Application(SIZE size, ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain)
	:m_device(device, context, swapChain)
{
	ID3D11Texture2D* temp = nullptr;
	m_device.swapChain()->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&temp));

	const dx_ptr<ID3D11Texture2D> backTexture{ temp };
	m_backBuffer =
		m_device.CreateRenderTargetView(backTexture);


	m_depthBuffer = m_device.CreateDepthStencilView(size);
	auto backBuffer = m_backBuffer.get();
	m_device.context()->OMSetRenderTargets(1, &backBuffer, m_depthBuffer.get());

	Viewport viewport{ size };
	m_device.context()->RSSetViewports(1, &viewport);

	const auto vsBytes = DxDevice::LoadByteCode(L"vs.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
	m_pixelShader = m_device.CreatePixelShader(psBytes);
	m_vertexShader = m_device.CreateVertexShader(vsBytes);

	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	offsetof(Vertex, color),
	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_layout = m_device.CreateInputLayout(elements, vsBytes);
	/*drawables = std::vector<std::shared_ptr<Shape>>();

	std::shared_ptr<Shape> cube = std::make_shared<Cube>();
	AddDrawable(cube);

	std::shared_ptr<Shape> cone = std::make_shared<Cone>();
	cone->Translate(2.0f, 0.0f, 0.0f);
	AddDrawable(cone);*/



	camera = std::make_shared<ArcCameraModel>();
	camera->SetPerspective(XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.1f, 100.0f);
	camera->ChangeDistance(20);
	camera->LookAt(0, 0, 0);

	backgroundColor = { 1,1,1 };

	torusModel = std::make_shared<Torus>(6, 1, 6,5);
	UpdateVertices();
}


void Application::Render()
{
	const float clearColor[] = { backgroundColor.x,backgroundColor.y,backgroundColor.z, 1.0f };

	m_device.context()->ClearRenderTargetView(m_backBuffer.get(), clearColor);
	m_device.context()->ClearDepthStencilView(m_depthBuffer.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	m_device.context()->VSSetShader(m_vertexShader.get(), nullptr, 0);
	m_device.context()->PSSetShader(m_pixelShader.get(), nullptr, 0);
	m_device.context()->IASetInputLayout(m_layout.get());
	m_device.context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	ID3D11Buffer* vbs[] = { m_vertexBuffer.get() };
	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };
	m_device.context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);
	m_device.context()->IASetIndexBuffer(m_indexBuffer.get(),
		DXGI_FORMAT_R32_UINT, 0);

	MVPMatrix = m_device.CreateConstantBuffer<DirectX::XMFLOAT4X4>();

	/*int vertexOffset = 0;
	int indexOffset = 0;
	for (size_t i = 0; i < drawables.size(); i++)
	{
		D3D11_MAPPED_SUBRESOURCE res;

		DirectX::XMMATRIX mvp;
		auto drawable = drawables[i];
		m_device.context()->Map(MVPMatrix.get(), 0,
			D3D11_MAP_WRITE_DISCARD, 0, &res);
		mvp =
			XMLoadFloat4x4(&drawable->translationMatrix) *
			XMLoadFloat4x4(&camera->viewMatrix) *
			XMLoadFloat4x4(&camera->perspectiveMatrix);
		memcpy(res.pData, &mvp, sizeof(DirectX::XMMATRIX));
		m_device.context()->Unmap(MVPMatrix.get(), 0);

		ID3D11Buffer* cbs[] = { MVPMatrix.get() };
		m_device.context()->VSSetConstantBuffers(0, 1, cbs);

		auto vertices = drawable->GetVertices();
		auto indices = drawable->GetIndices();

		m_device.context()->DrawIndexed(indices.size(), indexOffset, vertexOffset);
		vertexOffset += vertices.size();
		indexOffset += indices.size();

	}*/
	D3D11_MAPPED_SUBRESOURCE res;

	DirectX::XMMATRIX mvp;
	m_device.context()->Map(MVPMatrix.get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &res);
	mvp =
		XMLoadFloat4x4(&torusModel->modelMatrix) *
		XMLoadFloat4x4(&camera->viewMatrix) *
		XMLoadFloat4x4(&camera->perspectiveMatrix);
	memcpy(res.pData, &mvp, sizeof(DirectX::XMMATRIX));
	m_device.context()->Unmap(MVPMatrix.get(), 0);

	ID3D11Buffer* cbs[] = { MVPMatrix.get() };
	m_device.context()->VSSetConstantBuffers(0, 1, cbs);


	m_device.context()->DrawIndexed(torusModel->indicesCount, 0, 0);

}

void Application::Update()
{

}

void Application::AddDrawable(std::shared_ptr<Shape> drawable)
{
	drawables.push_back(drawable);

	std::vector<XMFLOAT3> vertices = std::vector<XMFLOAT3>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	for (size_t i = 0; i < drawables.size(); i++)
	{
		auto dv = drawables[i]->GetVertices();
		for (size_t j = 0; j < dv.size(); j++)
		{
			vertices.push_back(dv[j]);
		}
		auto di = drawables[i]->GetIndices();
		for (size_t j = 0; j < di.size(); j++)
		{
			indices.push_back(di[j]);
		}
	}
	m_vertexBuffer = m_device.CreateVertexBuffer(vertices);
	m_indexBuffer = m_device.CreateIndexBuffer(indices);
}

void Application::UpdateVertices()
{
	auto triangulation = torusModel->GetVertices();
	m_vertexBuffer = m_device.CreateVertexBuffer(triangulation.Vertices);
	m_indexBuffer = m_device.CreateIndexBuffer(triangulation.Indices);
}
