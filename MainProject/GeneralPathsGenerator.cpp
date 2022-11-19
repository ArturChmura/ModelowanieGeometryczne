#include "GeneralPathsGenerator.h"
#include "DirectXMath.h"
#include "OrthographicCamera.h"
#include "ArcCameraModel.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ID3D11ShaderResourceView* GeneralPathsGenerator::GeneratePaths(std::vector<std::shared_ptr<IModel>> models, ID3D11RenderTargetView* rtvPassed)
{
	int width = 512;
	int height = 512;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* r = NULL;
	auto hr = DxDevice::instance->Device()->CreateTexture2D(&descDepth, NULL, &r);
	mini::dx_ptr < ID3D11Texture2D> pDepthStencil(r);


	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	ID3D11DepthStencilView* tmp;
	//hr = DxDevice::instance->Device()->CreateDepthStencilView(pDepthStencil.get(), NULL, &pDSV);
	hr = DxDevice::instance->Device()->CreateDepthStencilView(pDepthStencil.get(), // Depth stencil texture
		&descDSV, // Depth stencil desc
		&tmp);  // [out] Depth stencil view

	mini::dx_ptr<ID3D11DepthStencilView>  pDSV(tmp);

	D3D11_TEXTURE2D_DESC descRenderTarget;
	descRenderTarget.Width = width;
	descRenderTarget.Height = height;
	descRenderTarget.MipLevels = 1;
	descRenderTarget.ArraySize = 1;
	descRenderTarget.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	descRenderTarget.SampleDesc.Count = 1;
	descRenderTarget.SampleDesc.Quality = 0;
	descRenderTarget.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	descRenderTarget.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	descRenderTarget.CPUAccessFlags = 0;
	descRenderTarget.MiscFlags = 0;

	hr = DxDevice::instance->Device()->CreateTexture2D(&descRenderTarget, NULL, &r);
	mini::dx_ptr<ID3D11Texture2D> pRenderTarget(r);
	mini::dx_ptr<ID3D11RenderTargetView> backBuffer = DxDevice::instance->CreateRenderTargetView(pRenderTarget);


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = descRenderTarget.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	ID3D11ShaderResourceView* shaderResourceView;
	// Create the shader resource view.
	DxDevice::instance->Device()->CreateShaderResourceView(pRenderTarget.get(), &shaderResourceViewDesc, &shaderResourceView);


	auto back = backBuffer.get();
	DxDevice::instance->context()->OMSetRenderTargets(1,
		&back,
		pDSV.get());
	float clearColor[] = { 1.0f,0.1f,0.1f, 1.0f };
	DxDevice::instance->context()->ClearRenderTargetView(back, clearColor);
	DxDevice::instance->context()->ClearDepthStencilView(pDSV.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Width = width;
	viewport.Height = height;
	viewport.MaxDepth = 1;

	DxDevice::instance->context()->RSSetViewports(1, &viewport);




	auto arcCameraMovement = std::make_shared<ArcCameraModel>(Vector3(0, 0, 0), 7.0f);
	arcCameraMovement->Rotate(0, -XM_PIDIV2);
	float farZ = 8.0f;
	float nearZ = 1.0f;
	auto camera = std::make_shared< OrthographicCamera>(arcCameraMovement, 15.0f, 15.0f, nearZ, farZ);

	for (auto model : models)
	{
		model->Draw(camera);
	}


	Texture2DDescription desc(width, height);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;

	desc.Width = width;
	desc.Height = height;
	desc.SampleDesc.Count = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;

	mini::dx_ptr<ID3D11Texture2D> heightMapTexture;
	heightMapTexture = DxDevice::instance->CreateTexture(desc);


	DxDevice::instance->context()->CopyResource(heightMapTexture.get(), pDepthStencil.get());

	D3D11_MAPPED_SUBRESOURCE ResourceDesc = {};
	DxDevice::instance->context()->Map(heightMapTexture.get(), 0, D3D11_MAP_READ, 0, &ResourceDesc);

	float* depthMap = new float[width * height];

	if (ResourceDesc.pData)
	{
		int const BytesPerPixel = sizeof(float);
		for (int i = 0; i < height; ++i)
		{
			std::memcpy(
				(byte*)depthMap + width * BytesPerPixel * i,
				(byte*)ResourceDesc.pData + ResourceDesc.RowPitch * i,
				width * BytesPerPixel);
		}
	}

	float* heightMap = new float[width * height];
	DxDevice::instance->context()->Unmap(heightMapTexture.get(), 0);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int index = j + i * width;
			float depth = depthMap[index];
			float height = 6.0f - 7.0f * depth;
			heightMap[index] = height;
		}
	}


	float* diffusedHeightMap = new float[width * height];
	std::fill(diffusedHeightMap, diffusedHeightMap + width * height, -100.0f);
	float drillRadius = 0.8f;
	int radiusInPixels = drillRadius / 15.0f * width;
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			int index = j + i * width;
			float currentHeight = heightMap[index];
			for (int k = i - radiusInPixels; k < i + radiusInPixels; k++)
			{
				if (k >= 0 && k < height)
				{
					for (int l = j - radiusInPixels; l < j + radiusInPixels; l++)
					{
						if (l >= 0 && l < width)
						{
							int offsetIndex = l + k * width;
							float offsetHeight = diffusedHeightMap[offsetIndex];
							if (currentHeight > offsetHeight)
							{
								diffusedHeightMap[offsetIndex] = currentHeight;
							}
						}
					}
				}
			}
		}
	}


	delete[] depthMap;
	delete[] heightMap;
	delete[] diffusedHeightMap;

	return shaderResourceView;

}
