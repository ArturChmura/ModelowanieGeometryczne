#pragma once
#include "window.h"
#include <memory>
#include "Scene.h"
#include "MouseEvents.h"
#include "CameraOptionsWindow.h"
#include "CursorOptionsWindow.h"
#include "ObjectsListWindow.h"
#include "PropertiesWindow.h"
#include "MessageHandler.h"
#include "DebugWindow.h"
#include "KeyboardHandler.h"
#include "ObjectAdderWindow.h"
#include "SimulationScene.h"
#include "ISimulation.h"
#include <chrono>

using namespace mini;
class Application
{
public:
	Application(SIZE size);
	void Render();
	void RenderGui();
	void Update();
	void HandleMessage(MSG message);
	mini::dx_ptr<ID3D11RenderTargetView> m_backBuffer;
	mini::dx_ptr<ID3D11DepthStencilView> m_depthBuffer;

	DirectX::XMFLOAT3 backgroundColor;

	std::shared_ptr<Scene> scene;
	std::shared_ptr<SimulationScene> simulationScene;
	std::shared_ptr<IScene> activeScene;

	bool renderGui = true;
	std::shared_ptr<IGuiWindow> startWindow;

	std::shared_ptr<MessageHandler> messageHandler;
	std::shared_ptr<MouseEvents> mouseEvents;
	std::shared_ptr<KeyboardHandler> keyboardHandler;

	std::map<int, std::vector<std::shared_ptr<IGuiWindow>>> sceneWindowsMap;

	std::chrono::steady_clock::time_point lastFrameTimePoint;
	std::shared_ptr<ISimulation> simulation;
};