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

using namespace mini;
class Application
{
public:
	Application(SIZE size, ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain);
	void Render();
	void Update();
	mini::dx_ptr<ID3D11RenderTargetView> m_backBuffer;
	mini::dx_ptr<ID3D11DepthStencilView> m_depthBuffer;
	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;

	std::shared_ptr<DxDevice> m_device;

	mini::dx_ptr<ID3D11Buffer> MVPMatrix;
	mini::dx_ptr<ID3D11Buffer> constantBuffer;

	DirectX::XMFLOAT3 backgroundColor;

	std::shared_ptr<Scene> scene;

	std::shared_ptr<TorusOptionsWindow> torusOptionsWindow;
	std::shared_ptr<PerspectiveCameraOptionsWindow> perspectiveCameraOptionsWindow;
	std::shared_ptr<MouseEvents> mouseEvents;

};