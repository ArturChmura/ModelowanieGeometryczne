#include "application.h"
#include "MVPColorConstantBuffer.h"
#include "Cursor3d.h"
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
#include "ModelSceneStartWindow.h"
#include "SimulationSceneStartWindow.h"
#include "SceneLoader.h"
#include "ModelFilterSelectorVisitor.h"
#include "GeneralPathsGenerator.h"
#include <imgui.h>
#include "OffsetParametrized.h"
#include "IntersectionFinder.h"
#include "BezierSurfacesFactory.h"
#include "IntersectionCurve.h"


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
	SIZE wpSize = { size.cx, size.cy };
	Viewport viewport{ wpSize };
	DxDevice::instance->context()->RSSetViewports(1, &viewport);

	ShadersManager::LoadShaders();

	auto cursor = std::make_shared<Cursor3d>();

	scene = std::make_shared<Scene>(cursor, size);
	simulationScene = std::make_shared<SimulationScene>();
	activeScene = scene;

	XMFLOAT3 targetPosition = { 0,0,0 };
	auto arcCameraMovement = std::make_shared<ArcCameraModel>(targetPosition, 30.0f);
	auto perspectiveCamera = std::make_shared<PerspectiveCamera>(arcCameraMovement, XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.1f, 1000.0f);
	auto stereoscopicCamera = std::make_shared<StereoscopicCamera>(arcCameraMovement, XMConvertToRadians(45), static_cast<float>(size.cx) / size.cy, 0.1f, 1000.0f, 2.0f, 100.0f, [this]() {return this->m_depthBuffer.get(); });

	scene->AddCamera(perspectiveCamera);
	scene->AddCamera(stereoscopicCamera);

	simulationScene->AddCamera(perspectiveCamera);

	backgroundColor = { 0.1f,0.1f,0.1f };

	auto scenes = std::vector<std::shared_ptr<IScene>>{ scene, simulationScene };
	startWindow = std::make_shared<StartWindow>(scenes,
		[&](std::shared_ptr<IScene> scene) {this->activeScene = scene; });

	std::vector< std::shared_ptr < IGuiWindow>> modelSceneGuiWindows;
	modelSceneGuiWindows.push_back(std::make_shared<ObjectsListWindow>(scene));
	modelSceneGuiWindows.push_back(std::make_shared<ObjectAdderWindow>(scene, &renderGui));
	modelSceneGuiWindows.push_back(std::make_shared<PropertiesWindow>(scene));
	modelSceneGuiWindows.push_back(std::make_shared<CameraOptionsWindow>(scene));
	modelSceneGuiWindows.push_back(std::make_shared<CursorOptionsWindow>(cursor, scene, size));
	modelSceneGuiWindows.push_back(std::make_shared<ModelSceneStartWindow>(scene));

	sceneWindowsMap.insert(std::make_pair(scene->id, modelSceneGuiWindows));


	std::vector< std::shared_ptr < IGuiWindow>> simulationSceneGuiWindows;
	simulationSceneGuiWindows.push_back(std::make_shared<SimulationSceneStartWindow>(simulationScene, [this](std::shared_ptr<ISimulation> simulation) {this->simulation = simulation; }));
	sceneWindowsMap.insert(std::make_pair(simulationScene->id, simulationSceneGuiWindows));



	//guiWindows.push_back(std::make_shared<DebugWindow>());


	mouseEvents = std::make_shared<MouseEvents>(arcCameraMovement, scene);
	keyboardHandler = std::make_shared<KeyboardHandler>(arcCameraMovement, scene);
	messageHandler = std::make_shared<MessageHandler>(scene);



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


	SceneLoader::LoadScene(scene, "..\\sciezki\\kubek.json");

}

void Application::Render()
{

	Viewport viewport{ DxDevice::windowSize };
	DxDevice::instance->context()->RSSetViewports(1, &viewport);
	float clearColor[] = { backgroundColor.x,backgroundColor.y,backgroundColor.z, 1.0f };

	auto backBuffer = m_backBuffer.get();
	DxDevice::instance->context()->OMSetRenderTargets(1, &backBuffer, m_depthBuffer.get());

	DxDevice::instance->context()->ClearRenderTargetView(m_backBuffer.get(), clearColor);
	DxDevice::instance->context()->ClearDepthStencilView(m_depthBuffer.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	scene->activeCamera->RenderScene(activeScene);
}

void Application::RenderGui()
{
	startWindow->Render();
	for (auto window : sceneWindowsMap[activeScene->id])
	{
		window->Render();
	}

	//ImGui::Begin("obrazek");

	//ImGui::Image((void*)image, ImVec2(512, 512));

	//ImGui::End();
}


void Application::Update()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrameTimePoint);
	lastFrameTimePoint = now;
	float dt = microseconds.count() / 1000000.0f;

	mouseEvents->HandleMouse();
	keyboardHandler->HandleKeyboard();
	if (this->simulation)
	{
		simulation->Update(dt);
	}
}

void Application::HandleMessage(MSG message)
{
	messageHandler->HandleMessage(message);
}


