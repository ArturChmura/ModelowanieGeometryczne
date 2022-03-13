#pragma once
#include "window.h"
#include "dxDevice.h"
#include <list>
#include "Drawable.h"
#include <memory>
#include "ArcCameraModel.h"
#include "Torus.h"

using namespace mini;
class Application
{
public:
	Application(SIZE size, ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain);
	void Render();
	void Update();
	void AddDrawable(std::shared_ptr<Shape> drawable);
	void UpdateBuffers();
	mini::dx_ptr<ID3D11RenderTargetView> m_backBuffer;
	mini::dx_ptr<ID3D11DepthStencilView> m_depthBuffer;
	mini::dx_ptr<ID3D11Buffer> m_vertexBuffer;
	mini::dx_ptr<ID3D11Buffer> m_indexBuffer;
	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;

	DxDevice m_device;

	std::vector<std::shared_ptr<Shape>> drawables;

	mini::dx_ptr<ID3D11Buffer> MVPMatrix;

	std::shared_ptr<ArcCameraModel> camera;

	POINT lastMousePosition;
	float rotationSpeedPerPixel = 0.01f;

	std::shared_ptr<Torus> torusModel;

	DirectX::XMFLOAT3 backgroundColor;
};