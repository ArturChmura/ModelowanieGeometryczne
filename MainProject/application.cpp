#include "application.h"
#include "Cube.h"
#include "Cone.h"
#include "windowsx.h"
#include "Torus.h"
#include "MVPColorConstantBuffer.h"
#include "DxVertexModelDrawer.h"
#include "GlobalAxisDrawer.h"
#include "Coursor3d.h"
#include "Coursor3dDrawer.h"

using namespace mini;
using namespace DirectX;

static std::vector<std::function<void()>> onUpdate;
Application::Application(SIZE size, ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain)
{
	m_device = std::make_shared<DxDevice>(device, context, swapChain);
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

	

	scene = std::make_shared<Scene>();
	XMFLOAT3 targetPosition = { 0,0,0 };
	auto camera = std::make_shared<ArcCameraModel>(targetPosition, 30, XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.01f, 1000.0f);
	scene->AddCamera(camera);

	backgroundColor = { 0,1,1 };

	auto torus = std::make_shared<Torus>(10, 3, 20, 20);
	auto torusDxModel = std::make_shared<DxVertexModelDrawer>(m_device, torus);
	scene->AddModel(torusDxModel);

	auto torus2 = std::make_shared<Torus>(5, 1, 40, 20);
	auto torusDxModel2 = std::make_shared<DxVertexModelDrawer>(m_device, torus2);
	scene->AddModel(torusDxModel2);

	//onUpdate.push_back([torus2]() {torus2->RotateFromPoint({ 0,10,0 }, { 0.01f, 0.000f, 0.000f }); });

	auto globalAxis = std::make_shared<GlobalAxisDrawer>(m_device);
	scene->AddModel(globalAxis);

	auto cursor = std::make_shared<Coursor3d>();
	cursor->SetTranslation({ 10, 2, 2 });
	auto cursorDrawer = std::make_shared<Coursor3dDrawer>(m_device, cursor);
	scene->AddModel(cursorDrawer);

	torusOptionsWindow = std::make_shared<TorusOptionsWindow>(torus);
	perspectiveCameraOptionsWindow = make_shared<PerspectiveCameraOptionsWindow>(camera);
	mouseEvents = std::make_shared<MouseEvents>(camera);
}

void Application::Render()
{
	torusOptionsWindow->Render();
	perspectiveCameraOptionsWindow->Render();
	mouseEvents->HandleMouse();

	const float clearColor[] = { backgroundColor.x,backgroundColor.y,backgroundColor.z, 1.0f };
	m_device->context()->ClearRenderTargetView(m_backBuffer.get(), clearColor);
	m_device->context()->ClearDepthStencilView(m_depthBuffer.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_device->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	for (size_t i = 0; i < scene->models.size(); i++)
	{
		auto model = scene->models[i];
		model->Draw(m_device, scene->activeCamera);
	}
}
void Application::Update()
{
	for (auto& f : onUpdate) { f(); }
}
