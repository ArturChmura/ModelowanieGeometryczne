#include "application.h"
#include "Cube.h"
#include "Cone.h"
#include "windowsx.h"

using namespace mini;
using namespace DirectX;

Application::Application(HINSTANCE hInstance)
	:m_hInstance(hInstance), m_window(hInstance, Window::m_defaultWindowWidth, Window::m_defaultWindowHeight, this), m_device(m_window)
{
	ID3D11Texture2D* temp = nullptr;
	m_device.swapChain()->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&temp));

	const dx_ptr<ID3D11Texture2D> backTexture{ temp };
	m_backBuffer =
		m_device.CreateRenderTargetView(backTexture);


	SIZE wndSize = m_window.getClientSize();
	m_depthBuffer = m_device.CreateDepthStencilView(wndSize);

	auto backBuffer = m_backBuffer.get();
	m_device.context()->OMSetRenderTargets(1, &backBuffer, m_depthBuffer.get());

	Viewport viewport{ wndSize };
	m_device.context()->RSSetViewports(1, &viewport);

	const auto vsBytes = DxDevice::LoadByteCode(L"vs.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
	m_pixelShader = m_device.CreatePixelShader(psBytes);
	m_vertexShader = m_device.CreateVertexShader(vsBytes);

	drawables = std::vector<std::shared_ptr<Shape>>();

	std::shared_ptr<Shape> cube = std::make_shared<Cube>();
	AddDrawable(cube);

	std::shared_ptr<Shape> cone = std::make_shared<Cone>();
	cone->Translate(2.0f, 0.0f, 0.0f);
	AddDrawable(cone);



	camera = std::make_shared<ArcCameraModel>();
	camera->SetPerspective(XMConvertToRadians(45), static_cast<float>(wndSize.cx) / wndSize.cy, 0.1f, 100.0f);
	camera->ChangeDistance(20);
	camera->LookAt(0, 0, 0);
}



int Application::MainLoop()
{
	MSG msg{};
	do {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else {
			Update();
			Render();
			m_device.swapChain()->Present(0, 0);

		}

	} while (msg.message != WM_QUIT);
	return msg.wParam;
}

bool Application::ProcessMessage(mini::WindowMessage& msg)
{
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	{
		lastMousePosition = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
		break;
	}
	case WM_MOUSEMOVE:
	{
		POINT diff;
		if (msg.wParam & MK_LBUTTON || msg.wParam & MK_RBUTTON)
		{
			POINT point{ GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
			diff = { point.x - lastMousePosition.x, point.y - lastMousePosition.y };
			lastMousePosition = point;
		}
		if (msg.wParam & MK_LBUTTON)
		{
			camera->Rotate(diff.x * -rotationSpeedPerPixel, diff.y * -rotationSpeedPerPixel);
		}
		else if (msg.wParam & MK_RBUTTON)
		{
			/*distance += distancePerPixel * diff;
			distance = 0 > distance ? 0 : distance;
			distance = 50 < distance ? 50 : distance;
			XMStoreFloat4x4(&m_viewMtx, XMLoadFloat4x4(&m_viewMtxWithoutTranslation) * XMMatrixTranslation(0, 0, distance));*/
		}
		break;
	}
	default:
		break;
	}
	return false;
}

void Application::Render()
{
	const float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_device.context()->ClearRenderTargetView(m_backBuffer.get(), clearColor);
	m_device.context()->ClearDepthStencilView(m_depthBuffer.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	m_device.context()->VSSetShader(m_vertexShader.get(), nullptr, 0);
	m_device.context()->PSSetShader(m_pixelShader.get(), nullptr, 0);

	m_device.context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11Buffer* vbs[] = { m_vertexBuffer.get() };
	UINT strides[] = { sizeof(DirectX::XMFLOAT3) };
	UINT offsets[] = { 0 };
	m_device.context()->IASetVertexBuffers(
		0, 1, vbs, strides, offsets);
	m_device.context()->IASetIndexBuffer(m_indexBuffer.get(),
		DXGI_FORMAT_R32_UINT, 0);

	MVPMatrix = m_device.CreateConstantBuffer<DirectX::XMFLOAT4X4>();

	int vertexOffset = 0;
	int indexOffset = 0;
	for (size_t i = 0; i < drawables.size(); i++)
	{
		D3D11_MAPPED_SUBRESOURCE res;

		DirectX::XMMATRIX mvp;
		auto drawable = drawables[i];
		m_device.context()->Map(MVPMatrix.get(), 0,
			D3D11_MAP_WRITE_DISCARD, 0, &res);
		mvp =
			XMLoadFloat4x4(&drawable->translationMatrix) *
			XMLoadFloat4x4(&camera->viewMatrix) *
			XMLoadFloat4x4(&camera->perspectiveMatrix);
		memcpy(res.pData, &mvp, sizeof(DirectX::XMMATRIX));
		m_device.context()->Unmap(MVPMatrix.get(), 0);

		ID3D11Buffer* cbs[] = { MVPMatrix.get() };
		m_device.context()->VSSetConstantBuffers(0, 1, cbs);

		auto vertices = drawable->GetVertices();
		auto indices = drawable->GetIndices();

		m_device.context()->DrawIndexed(indices.size(), indexOffset, vertexOffset);
		vertexOffset += vertices.size();
		indexOffset += indices.size();

	}

}

void Application::Update()
{

}

int Application::Run(int cmdShow)
{
	m_window.Show(cmdShow);
	return MainLoop();
}

void Application::AddDrawable(std::shared_ptr<Shape> drawable)
{
	drawables.push_back(drawable);

	std::vector<XMFLOAT3> vertices = std::vector<XMFLOAT3>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	for (size_t i = 0; i < drawables.size(); i++)
	{
		auto dv = drawables[i]->GetVertices();
		for (size_t j = 0; j < dv.size(); j++)
		{
			vertices.push_back(dv[j]);
		}
		auto di = drawables[i]->GetIndices();
		for (size_t j = 0; j < di.size(); j++)
		{
			indices.push_back(di[j]);
		}
	}
	m_vertexBuffer = m_device.CreateVertexBuffer(vertices);
	m_indexBuffer = m_device.CreateIndexBuffer(indices);
}
