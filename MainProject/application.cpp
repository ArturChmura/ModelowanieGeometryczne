#include "application.h"
#include "Cube.h"
#include "Cone.h"
#include "windowsx.h"
#include "Torus.h"
#include "ConstantBuffer.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

using namespace mini;
using namespace DirectX;


Application::Application(HWND windowHandle, SIZE size, const std::string& app_name)
{
	mWindow = std::make_unique<DXDWindow>();
	mWindow->init(windowHandle, size, app_name);
}

void Application::loop()
{
	while (mWindow->is_running())
	{
		mWindow->render();
	}
}
