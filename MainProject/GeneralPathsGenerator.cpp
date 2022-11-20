#include "GeneralPathsGenerator.h"
#include "DirectXMath.h"
#include "OrthographicCamera.h"
#include "ArcCameraModel.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
std::shared_ptr< StraightCurveInterpolating> GeneralPathsGenerator::GeneralPaths(std::vector<std::shared_ptr<IModel>> models)
{
	auto heightMap = GetHeightMapFromRenderingToDepthBuffer(models);

	auto diffusedHeightMap = GetDiffusedHeightMap(heightMap);

	delete[] heightMap;


	float pathsDistance = drillRadiusP1;

	int pathDistanceInPixels = pathsDistance / 15.0f * textureSize;


	std::vector<Vector3> positions;
	positions.push_back(beginPosition);

	int radiusInPixels = GetDrillRadiusInPixels(drillRadiusP1);

	auto beginCoords = std::make_pair(-2 * radiusInPixels, -2 * radiusInPixels);
	auto endCoords = std::make_pair(textureSize + 2 * radiusInPixels, textureSize + 2 * radiusInPixels);

	auto beginPositionTopLeftPlane = GetPositionFromCoordinates(beginCoords.first, beginCoords.second);
	auto beginPositionTopLeft = Vector3(beginPositionTopLeftPlane.x, beginPosition.y, beginPositionTopLeftPlane.y);
	positions.push_back(beginPositionTopLeft);


	auto makePlane = [&](float planeHeight) {
		bool topToBottom = true;

		for (int j = beginCoords.second; j <= endCoords.second; j += radiusInPixels)
		{
			for (int i = beginCoords.first; i <= endCoords.first; i += 1)
			{
				float height;
				int heightIndex = topToBottom ? i : textureSize - i - 1;
				if (heightIndex < 0 || heightIndex >= textureSize || j < 0 || j >= textureSize)
				{
					height = planeHeight;
				}
				else
				{
					int index = heightIndex * textureSize + j;
					height = max(diffusedHeightMap[index], planeHeight);
				}
				auto position = GetPositionFromCoordinates(heightIndex, j);
				auto pointPosition = Vector3(position.x, height, position.y);
				positions.push_back(pointPosition);
			}
			topToBottom = !topToBottom;
		}
		auto lastPosition = positions[positions.size() - 1];
		lastPosition.y = beginPosition.y;
		positions.push_back(lastPosition);
	};


	float planeHeight = 3.35f;
	makePlane(planeHeight);

	positions.push_back(beginPositionTopLeft);
	planeHeight = 1.6f;
	makePlane(planeHeight);

	positions.push_back(beginPosition);


	std::vector<std::shared_ptr<Point>> points;
	for (auto position : positions)
	{
		auto point = std::make_shared<Point>(position);
		points.push_back(point);
	}

	auto interpolated = std::make_shared<StraightCurveInterpolating>(points);


	delete[] diffusedHeightMap;
	return interpolated;
}

std::shared_ptr<StraightCurveInterpolating> GeneralPathsGenerator::PlanePaths(std::vector<std::shared_ptr<IModel>> models)
{
	auto heightMap = GetHeightMapFromRenderingToDepthBuffer(models);

	std::vector<Vector3> positions; 
	positions.push_back(beginPosition);

	int radiusInPixels = GetDrillRadiusInPixels(drillRadiusP2);



	auto geterateFromPosition = [&](Vector2 startPosition, Vector2 endPosition)
	{
		positions.push_back(Vector3(startPosition.x, beginPosition.y, startPosition.y));
		positions.push_back(Vector3(startPosition.x, baseHeight, startPosition.y));

		auto beginCoords = GetCoordinatesFromPosition(startPosition);
		auto endCoords = GetCoordinatesFromPosition(endPosition);

		int direction = 1; // 1 - top to bottom, -1 bottom to top
		int i = beginCoords.first;
		for (int j = beginCoords.second; j <= endCoords.second; j += radiusInPixels)
		{
			while (i <= endCoords.first && i >= beginCoords.first)
			{
				float maxHeight = GetMaxHeight(heightMap, i, j, radiusInPixels * 1.2);
				if (maxHeight - baseHeight <= 1e-5f)
				{
					auto position = GetPositionFromCoordinates(i, j);
					auto pointPosition = Vector3(position.x, baseHeight, position.y);
					positions.push_back(pointPosition);
					i += direction;
				}
				else
				{
					break;
				}
			}
			direction *= -1;
			bool canGoToNextLine = false;
			while (!canGoToNextLine)
			{
				i += direction;


				auto position = GetPositionFromCoordinates(i, j);
				auto pointPosition = Vector3(position.x, baseHeight, position.y);
				positions.push_back(pointPosition);

				float maxHeight = GetMaxHeight(heightMap, i, j + radiusInPixels, radiusInPixels * 1.2);
				canGoToNextLine = (maxHeight - baseHeight) <= 1e-5f;
			}
		}
		auto lastPosition = positions[positions.size() - 1];
		lastPosition.y = beginPosition.y;
		positions.push_back(lastPosition);


	};

	auto topLeftPosition = Vector2(-baseSize / 2 - drillRadiusP2 * 2, baseSize / 2 + drillRadiusP2 * 2);
	auto bottomRightPosition = Vector2(baseSize / 2 + drillRadiusP2 * 2, -baseSize / 2 - drillRadiusP2 * 2);

	geterateFromPosition(topLeftPosition, bottomRightPosition);


	topLeftPosition.x += drillRadiusP2 * 5;
	bottomRightPosition.x -= drillRadiusP2 * 3; // wyliczone rêcznie

	geterateFromPosition(topLeftPosition, bottomRightPosition);

	positions.push_back(beginPosition);


	std::vector<std::shared_ptr<Point>> points;
	for (auto position : positions)
	{
		auto point = std::make_shared<Point>(position);
		points.push_back(point);
	}

	auto interpolated = std::make_shared<StraightCurveInterpolating>(points);


	delete[] heightMap;
	return interpolated;

}

float GeneralPathsGenerator::GetMaxHeight(float* heightMap, int iCenter, int jCenter, int radiusInPixels)
{
	int radiusSquared = radiusInPixels * radiusInPixels;
	float maxHeight = -FLT_MAX;
	for (int di = -radiusInPixels; di <= radiusInPixels; di++)
	{
		for (int dj = -radiusInPixels; dj <= radiusInPixels; dj++)
		{
			if (di * di + dj * dj > radiusSquared)
			{
				continue;
			}
			int i = iCenter + di;
			int j = jCenter + dj;
			if (i < 0 || i >= textureSize || j < 0 || j >= textureSize)
			{
				continue;
			}
			int index = i * textureSize + j;
			float height = heightMap[index];
			if (height > maxHeight)
			{
				maxHeight = height;
			}
		}
	}
	return maxHeight;
}

DirectX::SimpleMath::Vector2 GeneralPathsGenerator::GetPositionFromCoordinates(int row, int column)
{
	float x = column / (float)textureSize * baseSize - baseSize/2;
	float z = -(row / (float)textureSize * baseSize - baseSize/2);
	return Vector2(x, z);
}

std::pair<int, int> GeneralPathsGenerator::GetCoordinatesFromPosition(DirectX::SimpleMath::Vector2 position)
{
	int i = (baseSize - 2*position.y) * textureSize / (2 * baseSize);
	int j = (2*position.x + baseSize) * textureSize / (2 * baseSize) ;
	return std::make_pair(i, j);
}

float* GeneralPathsGenerator::GetHeightMapFromRenderingToDepthBuffer(std::vector<std::shared_ptr<IModel>> models)
{
	auto depthTexture = CreateDepthTexture2D();
	auto depthStencilView = CreateDepthStencilView(depthTexture.get());
	auto viewport = CreateViewport();


	DxDevice::instance->context()->OMSetRenderTargets(0, nullptr, depthStencilView.get());
	DxDevice::instance->context()->ClearDepthStencilView(depthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	DxDevice::instance->context()->RSSetViewports(1, &viewport);

	auto camera = GetCamera();

	for (auto model : models)
	{
		model->Draw(camera);
	}


	auto heightMapTexture = CreateHeightMapTexture2D();

	DxDevice::instance->context()->CopyResource(heightMapTexture.get(), depthTexture.get());

	D3D11_MAPPED_SUBRESOURCE ResourceDesc = {};
	DxDevice::instance->context()->Map(heightMapTexture.get(), 0, D3D11_MAP_READ, 0, &ResourceDesc);

	float* depthMap = new float[textureSize * textureSize];

	if (ResourceDesc.pData)
	{
		int const BytesPerPixel = sizeof(float);
		for (int i = 0; i < textureSize; ++i)
		{
			std::memcpy(
				(byte*)depthMap + textureSize * BytesPerPixel * i,
				(byte*)ResourceDesc.pData + ResourceDesc.RowPitch * i,
				textureSize * BytesPerPixel);
		}
	}

	float* heightMap = new float[textureSize * textureSize];
	DxDevice::instance->context()->Unmap(heightMapTexture.get(), 0);

	float maxHeight = -10.0f;
	for (int i = 0; i < textureSize; ++i)
	{
		for (int j = 0; j < textureSize; ++j)
		{
			int index = j + i * textureSize;
			float depth = depthMap[index];
			float textureSize = 6 - 7 * depth;
			heightMap[index] = textureSize;
			if (textureSize > maxHeight)
				maxHeight = textureSize;
		}
	}

	delete[] depthMap;

	return heightMap;
}

float* GeneralPathsGenerator::GetDiffusedHeightMap(float* heightMap)
{
	float* diffusedHeightMap = new float[textureSize * textureSize];
	std::fill(diffusedHeightMap, diffusedHeightMap + textureSize * textureSize, -100.0f);
	int radiusInPixels = GetDrillRadiusInPixels(drillRadiusP1);
	for (int i = 0; i < textureSize; ++i)
	{
		for (int j = 0; j < textureSize; ++j)
		{
			int index = j + i * textureSize;
			float centerHeight = heightMap[index];
			auto centerPosition = GetPositionFromCoordinates(i, j);
			for (int k = i - radiusInPixels; k < i + radiusInPixels; k++)
			{
				if (k < 0 || k >= textureSize)
				{
					continue;
				}
				for (int l = j - radiusInPixels; l < j + radiusInPixels; l++)
				{
					if (l < 0 || l >= textureSize)
					{
						continue;
					}
					int widthDiff = abs(j - l);
					int heightDiff = abs(i - k);
					if (radiusInPixels * radiusInPixels < widthDiff * widthDiff + heightDiff * heightDiff)
					{
						continue;
					}

					int offsetIndex = l + k * textureSize;
					float currentOffsetHeight = diffusedHeightMap[offsetIndex];

					if (centerHeight > currentOffsetHeight)
					{
						diffusedHeightMap[offsetIndex] = centerHeight;
					}



				}

			}
		}
	}
	return diffusedHeightMap;
}

mini::dx_ptr<ID3D11Texture2D> GeneralPathsGenerator::CreateDepthTexture2D()
{
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = textureSize;
	descDepth.Height = textureSize;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* r = NULL;
	auto hr = DxDevice::instance->Device()->CreateTexture2D(&descDepth, NULL, &r);
	return mini::dx_ptr < ID3D11Texture2D>(r);
}

mini::dx_ptr<ID3D11DepthStencilView> GeneralPathsGenerator::CreateDepthStencilView(ID3D11Texture2D* depthTexture2D)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	ID3D11DepthStencilView* tmp;
	auto hr2 = DxDevice::instance->Device()->CreateDepthStencilView(depthTexture2D, &descDSV, &tmp);
	return mini::dx_ptr<ID3D11DepthStencilView>(tmp);
}

D3D11_VIEWPORT GeneralPathsGenerator::CreateViewport()
{

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Width = textureSize;
	viewport.Height = textureSize;
	viewport.MaxDepth = 1;
	return viewport;
}

std::shared_ptr<Camera> GeneralPathsGenerator::GetCamera()
{
	float cameraY = 7.0f;
	auto arcCameraMovement = std::make_shared<ArcCameraModel>(Vector3(0, 0, 0), cameraY);
	arcCameraMovement->Rotate(0, -XM_PIDIV2);
	float farZ = 8.0f;
	float nearZ = 1.0f;
	auto camera = std::make_shared< OrthographicCamera>(arcCameraMovement, 15.0f, 15.0f, nearZ, farZ);

	return camera;
}

mini::dx_ptr<ID3D11Texture2D> GeneralPathsGenerator::CreateHeightMapTexture2D()
{

	Texture2DDescription desc(textureSize, textureSize);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	desc.Width = textureSize;
	desc.Height = textureSize;
	desc.SampleDesc.Count = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;

	mini::dx_ptr<ID3D11Texture2D> heightMapTexture;
	return  DxDevice::instance->CreateTexture(desc);
}

int GeneralPathsGenerator::GetDrillRadiusInPixels(float drillRadius)
{
	return drillRadius / 15.0f * textureSize;
}
