#pragma once
#include "window.h"
#include "dxDevice.h"
#include <list>
#include "Drawable.h"
#include <memory>
#include "ArcCameraModel.h"
#include "Torus.h"
#include "Scene.h"
#include "TorusOptionsWindow.h"
#include "MouseEvents.h"
#include "PerspectiveCameraOptionsWindow.h"
#include "DXWindow.h"

using namespace mini;
class Application
{
public:
	Application(HWND windowHandle, SIZE size, const std::string& app_name);

	void loop();

private:

	std::unique_ptr<DXDWindow> mWindow;

};