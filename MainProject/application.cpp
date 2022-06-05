#include "application.h"
#include "MVPColorConstantBuffer.h"
#include "Coursor3d.h"
#include "Point.h"
#include "ShadersManager.h"
#include "PerspectiveCamera.h"
#include "StereoscopicCamera.h"
#include "BezierSurfaceC0.h"
#include "BezierSurfaceC2.h"
#include "StartWindow.h"
#include "BezierSurfaceC0AdderWindow.h"


using namespace mini;
using namespace DirectX;

Application::Application(SIZE size)
{
	ID3D11Texture2D* temp = nullptr;
	DxDevice::instance->swapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&temp));

	const dx_ptr<ID3D11Texture2D> backTexture{ temp };
	m_backBuffer = DxDevice::instance->CreateRenderTargetView(backTexture);
	m_depthBuffer = DxDevice::instance->CreateDepthStencilView(size);

	Viewport viewport{ size };
	DxDevice::instance->context()->RSSetViewports(1, &viewport);

	ShadersManager::LoadShaders();

	auto cursor = std::make_shared<Coursor3d>();

	scene = std::make_shared<Scene>(cursor, size);

	XMFLOAT3 targetPosition = { 0,0,0 };
	auto arcCameraMovement = std::make_shared<ArcCameraModel>(targetPosition, 30.0f);
	auto perspectiveCamera = std::make_shared<PerspectiveCamera>(arcCameraMovement, XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.1f, 1000.0f);
	auto stereoscopicCamera = std::make_shared<StereoscopicCamera>(arcCameraMovement, XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.1f, 1000.0f, 2.0f, 100.0f, [this]() {return this->m_depthBuffer.get(); });

	scene->AddCamera(perspectiveCamera);
	scene->AddCamera(stereoscopicCamera);

	backgroundColor = { 0.1f,0.1f,0.1f };

	guiWindows.push_back(std::make_shared<ObjectsListWindow>(scene));
	guiWindows.push_back(std::make_shared<ObjectAdderWindow>(scene));
	guiWindows.push_back(std::make_shared<PropertiesWindow>(scene));
	guiWindows.push_back(std::make_shared<CameraOptionsWindow>(scene));
	guiWindows.push_back(std::make_shared<CursorOptionsWindow>(cursor, scene, size));
	guiWindows.push_back(std::make_shared<StartWindow>(scene));
	//guiWindows.push_back(std::make_shared<DebugWindow>());


	mouseEvents = std::make_shared<MouseEvents>(arcCameraMovement, scene);
	keyboardHandler = std::make_shared<KeyboardHandler>(scene);
	messageHandler = std::make_shared<MessageHandler>(scene);

	float r = 0, fi = 0, phi = 0;
	int pointsCount = 000;
	for (int i = 0; i < pointsCount; i++)
	{
		float x = r * cosf(fi) * cosf(phi);
		float y = r * sinf(fi) * cosf(phi);
		float z = r * sinf(phi);
		//z = 0;
		scene->cursor->SetPosition({ x,y,z });
		auto point = scene->AddPoint();
		scene->ChangeSelection(point);

		r += 100.0f / pointsCount;
		fi += 0.13f;
		phi += 0.2f;
	}


	bool open;
	auto c0Adder = std::make_shared< BezierSurfaceC0AdderWindow>(scene, &open);
	c0Adder->horizontalSlicesCount = c0Adder->verticalSlicesCount = 1;
	scene->cursor->SetPosition({ 10,10,0 });
	c0Adder->AddModel();
	scene->cursor->SetPosition({ -10,10,0 });
	c0Adder->AddModel();
	scene->cursor->SetPosition({ 0,-10,0 });
	c0Adder->AddModel();

	auto backBuffer = m_backBuffer.get();
	DxDevice::instance->context()->OMSetRenderTargets(1, &backBuffer, m_depthBuffer.get());
}

void Application::Render()
{
	const float clearColor[] = { backgroundColor.x,backgroundColor.y,backgroundColor.z, 1.0f };

	auto backBuffer = m_backBuffer.get();
	DxDevice::instance->context()->OMSetRenderTargets(1, &backBuffer, m_depthBuffer.get());

	DxDevice::instance->context()->ClearRenderTargetView(m_backBuffer.get(), clearColor);
	DxDevice::instance->context()->ClearDepthStencilView(m_depthBuffer.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	scene->activeCamera->RenderScene(scene);
}

void Application::RenderGui()
{
	for (int i = 0; i < guiWindows.size(); i++)
	{
		guiWindows[i]->Render();
	}
}


void Application::Update()
{
	mouseEvents->HandleMouse();
	keyboardHandler->HandleKeyboard();
}

void Application::HandleMessage(MSG message)
{
	messageHandler->HandleMessage(message);
}
