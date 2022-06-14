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
#include "GregoryFinder.h"
#include "PointsMerger.h"


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
	guiWindows.push_back(std::make_shared<ObjectAdderWindow>(scene, &renderGui));
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


	/*float width = 20;
	c0Adder->width = c0Adder->height = width;
	scene->cursor->SetPosition({ width,width/2,0 });
	auto prawy = std::make_shared<CompositeModel>();
	auto p = c0Adder->AddModel();
	for (auto model : p->GetContainingModels())
	{
		prawy->AddModel(model);
	}
	
	scene->cursor->SetPosition({ -width,width/2,0 });
	auto lewy = std::make_shared<CompositeModel>();
	auto l = c0Adder->AddModel();
	for (auto model : l->GetContainingModels())
	{
		lewy->AddModel(model);
	}
	scene->cursor->SetPosition({ 0,-width/2,0 });
	auto srodek = std::make_shared<CompositeModel>();
	auto s = c0Adder->AddModel();
	for (auto model : s->GetContainingModels())
	{
		srodek->AddModel(model);
	}

	srodek->RotateFromPoint({ 0,0,0,1 }, { DirectX::XM_PIDIV4,0,0 });
	lewy->RotateFromPoint({ 0,0,0,1 }, { 0,DirectX::XM_PIDIV4,0 });
	auto lSingle = l->GetSingleSurfaces()[0];
	auto pSingle = p->GetSingleSurfaces()[0];
	auto sSingle = s->GetSingleSurfaces()[0];

	std::vector<std::shared_ptr<Point>> points
	{
		lSingle->points[0][3],
		sSingle->points[3][0],
	};

	PointsMerger pointsMerger;
	pointsMerger.MergePoints(scene, points);

	points = 
	{
		pSingle->points[0][0],
		sSingle->points[3][3],
	};
	pointsMerger.MergePoints(scene, points);

	points =
	{
		pSingle->points[3][0],
		lSingle->points[3][3],
	};
	pointsMerger.MergePoints(scene, points);
*/

	
	c0Adder->cylinder = true;
	c0Adder->horizontalSlicesCount =  1;
	c0Adder->verticalSlicesCount = 3;
	scene->cursor->SetPosition({ 0,0,0 });
	c0Adder->AddModel();



	auto backBuffer = m_backBuffer.get();
	DxDevice::instance->context()->OMSetRenderTargets(1, &backBuffer, m_depthBuffer.get());



	SamplerDescription sd;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	sd.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = 16;
	auto m_samplerWrap = DxDevice::instance->CreateSamplerState(sd);

	auto s_ptr = m_samplerWrap.get();
	DxDevice::instance->context()->PSSetSamplers(0, 1, &s_ptr);
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
	//if(renderGui)
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
