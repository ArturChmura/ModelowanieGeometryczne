#include "application.h"
#include "MVPColorConstantBuffer.h"
#include "Coursor3d.h"
#include "Point.h"
#include "ShadersManager.h"
#include "PerspectiveCamera.h"
#include "StereoscopicCamera.h"

using namespace mini;
using namespace DirectX;

Application::Application(SIZE size)
{
	ID3D11Texture2D* temp = nullptr;
	//DxDevice::instance->swapChain()->GetBuffer(0,__uuidof(ID3D11Texture2D),
	//	reinterpret_cast<void**>(&temp));

	//const dx_ptr<ID3D11Texture2D> backTexture{ temp };
	//m_backBuffer = DxDevice::instance->CreateRenderTargetView(backTexture);

	//m_depthBuffer = DxDevice::instance->CreateDepthStencilView(size);

	Viewport viewport{ size };
	DxDevice::instance->context()->RSSetViewports(1, &viewport);

	ShadersManager::LoadShaders();

	auto cursor = std::make_shared<Coursor3d>();

	scene = std::make_shared<Scene>(cursor, size);

	XMFLOAT3 targetPosition = { 0,0,0 };
	auto arcCameraMovement = std::make_shared<ArcCameraModel>(targetPosition, 30.0f);
	auto perspectiveCamera = std::make_shared<PerspectiveCamera>(arcCameraMovement,XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.1f, 1000.0f);
	auto stereoscopicCamera = std::make_shared<StereoscopicCamera>(arcCameraMovement, XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.1f, 1000.0f, 2.0f, 100.0f);

	scene->AddCamera(perspectiveCamera);
	scene->AddCamera(stereoscopicCamera);

	backgroundColor = { 0,0,0 };

	objectsListWindow = std::make_shared<ObjectsListWindow>(scene);
	propertiesWindow = std::make_shared<PropertiesWindow>(scene);
	cameraOptionsWindow = std::make_shared<CameraOptionsWindow>();
	cursorOptionsWindow = std::make_shared<CursorOptionsWindow>(cursor, scene, size);
	mouseEvents = std::make_shared<MouseEvents>(arcCameraMovement, scene);
	keyboardHandler = std::make_shared<KeyboardHandler>(scene);
	messageHandler = std::make_shared<MessageHandler>(scene);
	debugWindow = std::make_shared<DebugWindow>();



	//float r = 0, fi = 0, phi = 0;
	//for (int i = 0; i < 10; i++)
	//{
	//	float x = r * cosf(fi) * cosf(phi);
	//	float y = r * sinf(fi) * cosf(phi);
	//	float z = r * sinf(phi);
	//	z = 0;
	//	scene->cursor->translation = { x,y,z };
	//	auto point = scene->AddPoint();
	//	scene->ChangeSelection(point);

	//	r += 1.0f;
	//	fi += 0.2f;
	//	phi += 0.2f;
	//}
	scene->cursor->translation = { 0,0,0 };
	auto point = scene->AddPoint();
	scene->cursor->translation = { 0,0,5 };
	point = scene->AddPoint();
	scene->ChangeSelection(point);

	//scene->AddBezierCurveInterpolatingFromSelectedPoints();
	//scene->AddBezierCurveC2FromSelectedPoints();
	//scene->AddBezierCurveC0FromSelectedPoints();


	scene->cursor->translation = { 10,10,10 };
	//scene->AddTorus();

}

void Application::Render()
{
	const float clearColor[] = { backgroundColor.x,backgroundColor.y,backgroundColor.z, 1.0f };

	DxDevice::instance->context()->ClearRenderTargetView(DxDevice::g_mainRenderTargetView, clearColor);
	DxDevice::instance->context()->ClearDepthStencilView(DxDevice::g_depthBufferTargetView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	scene->activeCamera->RenderScene(scene);
}

void Application::RenderGui()
{
	cameraOptionsWindow->Render(scene);
	cursorOptionsWindow->Render();
	objectsListWindow->Render();
	propertiesWindow->Render();
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
