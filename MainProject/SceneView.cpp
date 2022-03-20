#include "SceneView.h"
#include "ConstantBuffer.h"
#include "ImGui/imgui.h"
#include "DirectXMath.h"
#include "ArcCameraModel.h"
#include "Torus.h"
using namespace mini;
using namespace DirectX;
SceneView::SceneView(
)
{
	mSize = { 600,600 };
	this->dxDevice = std::make_shared<DxDevice>(mSize);
	ID3D11Texture2D* temp = nullptr;
	dxDevice->swapChain()->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&temp));

	const dx_ptr<ID3D11Texture2D> backTexture{ temp };
	m_backBuffer = dxDevice->CreateRenderTargetView(backTexture);

	SIZE clientSize = { 600,600 };
	m_depthBuffer = dxDevice->CreateDepthStencilView(clientSize);
	auto backBuffer = m_backBuffer.get();
	dxDevice->context()->OMSetRenderTargets(1, &backBuffer, m_depthBuffer.get());


	Viewport viewport { clientSize };
	dxDevice->context()->RSSetViewports(1, &viewport);

	const auto vsBytes = DxDevice::LoadByteCode(L"vs.cso");
	const auto psBytes = DxDevice::LoadByteCode(L"ps.cso");
	m_pixelShader = dxDevice->CreatePixelShader(psBytes);
	m_vertexShader = dxDevice->CreateVertexShader(vsBytes);
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_layout = dxDevice->CreateInputLayout(elements, vsBytes);

	scene = std::make_shared<Scene>();
	XMFLOAT3 targetPosition = { 0,0,0 };
	auto camera = std::make_shared<ArcCameraModel>(targetPosition, 30, XMConvertToRadians(45), static_cast<float>(clientSize.cx) / clientSize.cy, 0.01f, 1000.0f);


	scene->AddCamera(camera);

	backgroundColor = { 1,1,1 };

	auto torus = std::make_shared<Torus>(10, 3, 20, 20);
	auto torusDxModel = std::make_shared<DxVertexModelDrawer>( torus, dxDevice);
	scene->AddModel(torusDxModel);
}

void SceneView::Render()
{
	ImGui::Begin("Scene");

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	//mSize = { viewportPanelSize.x, viewportPanelSize.y };
	const auto back = m_backBuffer.get();
	dxDevice->context()->OMSetRenderTargets(1, &back, NULL);


	const float clearColor[] = { backgroundColor.x,backgroundColor.y,backgroundColor.z, 1.0f };
	dxDevice->context()->ClearRenderTargetView(m_backBuffer.get(), clearColor);
	dxDevice->context()->ClearDepthStencilView(m_depthBuffer.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	dxDevice->context()->VSSetShader(m_vertexShader.get(), nullptr, 0);
	dxDevice->context()->PSSetShader(m_pixelShader.get(), nullptr, 0);
	dxDevice->context()->IASetInputLayout(m_layout.get());
	dxDevice->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	for (size_t i = 0; i < scene->models.size(); i++)
	{
		auto model = scene->models[i];

		ID3D11Buffer* vbs[] = { model->vertexBuffer.get() };
		UINT strides[] = { sizeof(Vertex) };
		UINT offsets[] = { 0 };
		dxDevice->context()->IASetVertexBuffers(
			0, 1, vbs, strides, offsets);
		dxDevice->context()->IASetIndexBuffer(model->indexBuffer.get(),
			DXGI_FORMAT_R32_UINT, 0);

		constantBuffer = dxDevice->CreateConstantBuffer<ConstantBuffer>();
		D3D11_MAPPED_SUBRESOURCE res;
		DirectX::XMMATRIX mvp;
		dxDevice->context()->Map(constantBuffer.get(), 0,
			D3D11_MAP_WRITE_DISCARD, 0, &res);
		auto constantBufferStruct = ConstantBuffer();
		auto m = model->vertexModel->GetModelMatrix();
		auto v = scene->activeCamera->GetViewMatrix();
		auto p = scene->activeCamera->GetPerspectiveMatrix();
		constantBufferStruct.mvp =
			XMLoadFloat4x4(&m) *
			XMLoadFloat4x4(&v) *
			XMLoadFloat4x4(&p);
		constantBufferStruct.color = XMLoadFloat3(&model->vertexModel->color);
		memcpy(res.pData, &constantBufferStruct, sizeof(ConstantBuffer));
		dxDevice->context()->Unmap(constantBuffer.get(), 0);

		ID3D11Buffer* cbs[] = { constantBuffer.get() };
		dxDevice->context()->VSSetConstantBuffers(0, 1, cbs);

		auto count = model->vertexModel->GetIndicesCount();
		dxDevice->context()->DrawIndexed(count, 0, 0);
	}

	ID3D11ShaderResourceView* shaderResourceView;
	dxDevice->context()->CSGetShaderResources(0, 1, &shaderResourceView);
	//ImGui::Image(reinterpret_cast<void*>(shaderResourceView), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
}
