#pragma once
#include "window.h"
#include "dxDevice.h"
#include <list>
#include "Drawable.h"
#include <memory>
#include "ArcCameraModel.h"
using namespace mini;
class Application : protected IWindowMessageHandler
{
public:
	Application(HINSTANCE hInstance);
	int MainLoop();
	bool ProcessMessage(mini::WindowMessage& msg);
	void Render();
	void Update();
	int Run(int cmdShow = SW_SHOWNORMAL);

	void AddDrawable(std::shared_ptr<Shape> drawable);

	mini::dx_ptr<ID3D11RenderTargetView> m_backBuffer;
	mini::dx_ptr<ID3D11DepthStencilView> m_depthBuffer;
	mini::dx_ptr<ID3D11Buffer> m_vertexBuffer;
	mini::dx_ptr<ID3D11Buffer> m_indexBuffer;
	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;

	HINSTANCE m_hInstance;
	Window m_window;
	DxDevice m_device;

	std::vector<std::shared_ptr<Shape>> drawables;

	mini::dx_ptr<ID3D11Buffer> MVPMatrix;

	std::shared_ptr<ArcCameraModel> camera;

	POINT lastMousePosition;
	float rotationSpeedPerPixel = 0.01f;
};