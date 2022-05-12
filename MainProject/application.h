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

	std::shared_ptr<MessageHandler> messageHandler;
	std::shared_ptr<ObjectsListWindow> objectsListWindow;
	std::shared_ptr<PropertiesWindow> propertiesWindow;
	std::shared_ptr<CameraOptionsWindow> cameraOptionsWindow;
	std::shared_ptr<CursorOptionsWindow> cursorOptionsWindow;
	std::shared_ptr<DebugWindow> debugWindow;
	std::shared_ptr<MouseEvents> mouseEvents;
	std::shared_ptr<KeyboardHandler> keyboardHandler;

};