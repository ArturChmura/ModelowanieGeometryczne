#include "application.h"
#include "Cube.h"
#include "Cone.h"
#include "windowsx.h"
#include "Torus.h"

using namespace mini;
using namespace DirectX;

Application::Application(SIZE size, ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain)
{
	m_device = make_shared<DxDevice>(device, context, swapChain);
	ID3D11Texture2D* temp = nullptr;
	m_device->swapChain()->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&temp));

	const dx_ptr<ID3D11Texture2D> backTexture{ temp };
	m_backBuffer = m_device->CreateRenderTargetView(backTexture);

	m_depthBuffer = m_device->CreateDepthStencilView(size);
	auto backBuffer = m_backBuffer.get();
	m_device->context()->OMSetRenderTargets(1, &backBuffer, m_depthBuffer.get());

	Viewport viewport{ size };
	m_device->context()->RSSetViewports(1, &viewport);

	const auto vsBytes = DxDevice::LoadByteCode(L"vs.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
	m_pixelShader = m_device->CreatePixelShader(psBytes);
	m_vertexShader = m_device->CreateVertexShader(vsBytes);
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	offsetof(Vertex, color),
	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_layout = m_device->CreateInputLayout(elements, vsBytes);

	scene = make_shared<Scene>();
	XMFLOAT3 targetPosition = { 0,0,0 };
	auto camera = std::make_shared<ArcCameraModel>(targetPosition, 30, XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.01f, 1000.0f);


	scene->AddCamera(camera);

	backgroundColor = { 1,1,1 };

	auto torus = make_shared<Torus>(10, 3, 20, 20);
	auto torusDxModel = make_shared<DxVertexModelDrawer>(m_device, torus);
	scene->AddModel(torusDxModel);

	torusOptionsWindow = make_shared<TorusOptionsWindow>(torus);
	perspectiveCameraOptionsWindow = make_shared<PerspectiveCameraOptionsWindow>(camera);
	mouseEvents = make_shared<MouseEvents>(camera);
}


void Application::Render()
{
	torusOptionsWindow->Render();
	perspectiveCameraOptionsWindow->Render();
	mouseEvents->HandleMouse();
	const float clearColor[] = { backgroundColor.x,backgroundColor.y,backgroundColor.z, 1.0f };
	m_device->context()->ClearRenderTargetView(m_backBuffer.get(), clearColor);
	m_device->context()->ClearDepthStencilView(m_depthBuffer.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	m_device->context()->VSSetShader(m_vertexShader.get(), nullptr, 0);
	m_device->context()->PSSetShader(m_pixelShader.get(), nullptr, 0);
	m_device->context()->IASetInputLayout(m_layout.get());
	m_device->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	for (size_t i = 0; i < scene->models.size(); i++)
	{
		auto model = scene->models[i];

		ID3D11Buffer* vbs[] = { model->vertexBuffer.get() };
		UINT strides[] = { sizeof(Vertex) };
		UINT offsets[] = { 0 };
		m_device->context()->IASetVertexBuffers(
			0, 1, vbs, strides, offsets);
		m_device->context()->IASetIndexBuffer(model->indexBuffer.get(),
			DXGI_FORMAT_R32_UINT, 0);

		MVPMatrix = m_device->CreateConstantBuffer<DirectX::XMFLOAT4X4>();
		D3D11_MAPPED_SUBRESOURCE res;
		DirectX::XMMATRIX mvp;
		m_device->context()->Map(MVPMatrix.get(), 0,
			D3D11_MAP_WRITE_DISCARD, 0, &res); 
		auto m = model->vertexModel->GetModelMatrix();
		auto v = scene->activeCamera->GetViewMatrix();
		auto p = scene->activeCamera->GetPerspectiveMatrix();
		mvp =
			XMLoadFloat4x4(&m) *
			XMLoadFloat4x4(&v) *
			XMLoadFloat4x4(&p);
		memcpy(res.pData, &mvp, sizeof(DirectX::XMMATRIX));
		m_device->context()->Unmap(MVPMatrix.get(), 0);

		ID3D11Buffer* cbs[] = { MVPMatrix.get() };
		m_device->context()->VSSetConstantBuffers(0, 1, cbs);

		auto count = model->vertexModel->GetIndicesCount();
		m_device->context()->DrawIndexed(count, 0, 0);
	}
}

void Application::Update()
{

}
