#include "application.h"
#include "windowsx.h"
#include "Torus.h"
#include "MVPColorConstantBuffer.h"
#include "GlobalAxisDrawer.h"
#include "Coursor3d.h"
#include "Point.h"

using namespace mini;
using namespace DirectX;

Application::Application(SIZE size)
{
	ID3D11Texture2D* temp = nullptr;
	DxDevice::instance->swapChain()->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&temp));

	const dx_ptr<ID3D11Texture2D> backTexture{ temp };
	m_backBuffer = DxDevice::instance->CreateRenderTargetView(backTexture);

	m_depthBuffer = DxDevice::instance->CreateDepthStencilView(size);
	auto backBuffer = m_backBuffer.get();
	DxDevice::instance->context()->OMSetRenderTargets(1, &backBuffer, m_depthBuffer.get());

	Viewport viewport{ size };
	DxDevice::instance->context()->RSSetViewports(1, &viewport);

	auto cursor = std::make_shared<Coursor3d>();

	scene = std::make_shared<Scene>(cursor, size);
	XMFLOAT3 targetPosition = { 0,0,0 };
	auto camera = std::make_shared<ArcCameraModel>(targetPosition, 30, XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.01f, 1000.0f);
	scene->AddCamera(camera);

	backgroundColor = { 0,0,0 };

	auto torus = std::make_shared<Torus>(5, 2, 20, 20);
	torus->SetTranslation(-10, 0, 0);
	scene->AddModel(torus);

	torus = std::make_shared<Torus>(5, 2, 20, 20);
	torus->SetTranslation(10, 0, 0);
	scene->AddModel(torus);

	/*auto globalAxis = std::make_shared<GlobalAxisDrawer>(m_device);
	scene->AddModel(globalAxis);*/


	objectsListWindow = std::make_shared<ObjectsListWindow>(scene);
	propertiesWindow = std::make_shared<PropertiesWindow>(scene);
	perspectiveCameraOptionsWindow = make_shared<PerspectiveCameraOptionsWindow>(camera);
	cursorOptionsWindow = make_shared<CursorOptionsWindow>(cursor, scene, size);
	mouseEvents = std::make_shared<MouseEvents>(camera, scene);
}

void Application::Render()
{
	perspectiveCameraOptionsWindow->Render();
	cursorOptionsWindow->Render();
	mouseEvents->HandleMouse();
	objectsListWindow->Render();
	propertiesWindow->Render();
	const float clearColor[] = { backgroundColor.x,backgroundColor.y,backgroundColor.z, 1.0f };
	DxDevice::instance->context()->ClearRenderTargetView(m_backBuffer.get(), clearColor);
	DxDevice::instance->context()->ClearDepthStencilView(m_depthBuffer.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	for (size_t i = 0; i < scene->models.size(); i++)
	{
		auto model = scene->models[i];
		model->Draw(scene->activeCamera);
	}
	if (scene->selectedModel)
	{
		scene->selectedModel->Draw(scene->activeCamera);
	}
	scene->cursor->Draw(scene->activeCamera);
}
void Application::Update()
{

}
