#pragma once
#include "dxDevice.h"
#include <memory>
#include "DirectXMath.h"
#include "Scene.h"
class SceneView
{
public:
	SceneView(
	);
	void Render();

	mini::dx_ptr<ID3D11RenderTargetView> m_backBuffer;
	mini::dx_ptr<ID3D11DepthStencilView> m_depthBuffer;
	mini::dx_ptr<ID3D11VertexShader> m_vertexShader;
	mini::dx_ptr<ID3D11PixelShader> m_pixelShader;
	mini::dx_ptr<ID3D11InputLayout> m_layout;


	mini::dx_ptr<ID3D11Buffer> constantBuffer;

	DirectX::XMFLOAT3 backgroundColor;

	std::shared_ptr<Scene> scene;
	SIZE mSize; 
	std::shared_ptr<DxDevice> dxDevice;
};