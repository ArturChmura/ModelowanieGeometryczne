#pragma once
#include "window.h"
#include "dxDevice.h"
#include <list>
#include <memory>
#include "ArcCameraModel.h"
#include "Torus.h"
#include "Scene.h"
#include "MouseEvents.h"
#include "PerspectiveCameraOptionsWindow.h"
#include "CursorOptionsWindow.h"
#include "ObjectsListWindow.h"
#include "PropertiesWindow.h"


using namespace mini;
class Application
{
public:
	Application(SIZE size);
	void Render();
	void Update();
	mini::dx_ptr<ID3D11RenderTargetView> m_backBuffer;
	mini::dx_ptr<ID3D11DepthStencilView> m_depthBuffer;

	DirectX::XMFLOAT3 backgroundColor;

	std::shared_ptr<Scene> scene;

	std::shared_ptr<ObjectsListWindow> objectsListWindow;
	std::shared_ptr<PropertiesWindow> propertiesWindow;
	std::shared_ptr<PerspectiveCameraOptionsWindow> perspectiveCameraOptionsWindow;
	std::shared_ptr<CursorOptionsWindow> cursorOptionsWindow;
	std::shared_ptr<MouseEvents> mouseEvents;

};