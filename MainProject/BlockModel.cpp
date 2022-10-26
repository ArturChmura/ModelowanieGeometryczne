#include "BlockModel.h"
#include "ShadersManager.h"
#include "dxDevice.h"
#include <algorithm>
#include "BresenhamsAlgorithm.h"
#include "SphereCutter.h"
#include "FlatCutter.h"

using namespace DirectX::SimpleMath;
BlockModel::BlockModel(float widthSize, float lengthSize, float heightSize, int gridWidthCount, int gridLengthCount, float minHeight)
{
	this->widthSize = widthSize;
	this->lengthSize = lengthSize;
	this->heightSize = heightSize;
	this->gridWidthCount = gridWidthCount;
	this->gridLengthCount = gridLengthCount;
	this->minHeight = minHeight;
	heightMap = new float[gridWidthCount * gridLengthCount];

	for (int i = 0; i < gridLengthCount; i++)
	{
		for (int j = 0; j < gridWidthCount; j++)
		{
			heightMap[i * gridWidthCount + j] = heightSize;
		}
	}

	resetDrawing = true;

	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	InitializeMesh();


	Texture2DDescription desc(gridWidthCount, gridLengthCount);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;

	desc.Width = gridWidthCount;
	desc.Height = gridLengthCount;
	desc.SampleDesc.Count = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	heightMapTexture = DxDevice::instance->CreateTexture(desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;


	heightMapTextureSRV = DxDevice::instance->CreateShaderResourceView(heightMapTexture, &srvDesc);

	colorTexture = DxDevice::instance->CreateShaderResourceView(L"Textures/steeltex.jpg");


}

void BlockModel::InitializeMesh()
{

	int verticesCount = gridWidthCount * gridLengthCount;
	int indicesCount = ((gridWidthCount - 1) * (gridLengthCount - 1)) * 6;

	vertices.reserve(verticesCount);
	std::vector<int> indices = std::vector<int>();
	indices.reserve(indicesCount);

	int index = 0;
	for (int zIndex = 0; zIndex < gridLengthCount; zIndex++)
	{
		for (int xIndex = 0; xIndex < gridWidthCount; xIndex++)
		{
			float dx = widthSize / (gridWidthCount - 1);
			float dz = lengthSize / (gridLengthCount - 1);

			float x = -widthSize / 2 + dx * xIndex;
			float z = -lengthSize / 2 + dz * zIndex;

			VSNormalIn vertex;
			vertex.pos = Vector3(x, 10.0, z);
			vertex.normal = Vector3(0, 1, 0);
			vertices.push_back(vertex);
			index++;

		}
	}

	for (int zIndex = 0; zIndex < gridLengthCount - 1; zIndex++)
	{
		for (int xIndex = 0; xIndex < gridWidthCount - 1; xIndex++)
		{
			int bottomLeft = zIndex * gridWidthCount + xIndex;
			int bottomRight = zIndex * gridWidthCount + (xIndex + 1);
			int topLeft = (zIndex + 1) * gridWidthCount + xIndex;
			int topRight = (zIndex + 1) * gridWidthCount + (xIndex + 1);

			indices.push_back(topLeft);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);

			indices.push_back(topRight);
			indices.push_back(bottomRight);
			indices.push_back(bottomLeft);

		}
	}

	auto addSide = [&](int top, int bottom, Vector3 normal)
	{
		auto topVertex = vertices[top];
		auto bottomVertex = vertices[bottom];

		VSNormalIn topBaseVertex;
		topBaseVertex.pos = topVertex.pos;
		topBaseVertex.pos.y = 0;
		topBaseVertex.normal = normal;

		VSNormalIn bottomBaseVertex;
		bottomBaseVertex.pos = bottomVertex.pos;
		bottomBaseVertex.pos.y = 0;
		bottomBaseVertex.normal = normal;

		topVertex.normal = normal;
		bottomVertex.normal = normal;

		vertices.push_back(topVertex);
		int topVertexIndex = index++;
		vertices.push_back(bottomVertex);
		int bottomVertexIndex = index++;
		vertices.push_back(topBaseVertex);
		int topBaseVertexIndex = index++;
		vertices.push_back(bottomBaseVertex);
		int bottomBaseVertexIndex = index++;

		indices.push_back(topVertexIndex);
		indices.push_back(topBaseVertexIndex);
		indices.push_back(bottomVertexIndex);

		indices.push_back(bottomVertexIndex);
		indices.push_back(topBaseVertexIndex);
		indices.push_back(bottomBaseVertexIndex);
	};

	//lewa œciana
	for (int zIndex = 0; zIndex < gridLengthCount - 1; zIndex++)
	{
		int top = zIndex * gridWidthCount;
		int bottom = (zIndex + 1) * gridWidthCount;

		addSide(top, bottom, Vector3(-1, 0, 0));
	}

	//prawa œciana
	for (int zIndex = 0; zIndex < gridLengthCount - 1; zIndex++)
	{
		int bottom = zIndex * gridWidthCount + gridWidthCount - 1;
		int top = (zIndex + 1) * gridWidthCount + gridWidthCount - 1;

		addSide(top, bottom, Vector3(1, 0, 0));
	}

	//dolna œciana
	for (int xIndex = 0; xIndex < gridWidthCount - 1; xIndex++)
	{
		int top = xIndex + 1;
		int bottom = xIndex;

		addSide(top, bottom, Vector3(0, 0, -1));
	}

	//górna œciana
	for (int xIndex = 0; xIndex < gridWidthCount - 1; xIndex++)
	{
		int top = (gridLengthCount - 1) * gridWidthCount + xIndex;
		int bottom = (gridLengthCount - 1) * gridWidthCount + xIndex + 1;

		addSide(top, bottom, Vector3(0, 0, 1));
	}

	//podstawa
	VSNormalIn topLeft;
	topLeft.pos = Vector3(-widthSize / 2, 0, -lengthSize / 2);
	topLeft.normal = Vector3(0, -1, 0);

	VSNormalIn topRight;
	topRight.pos = Vector3(widthSize / 2, 0, -lengthSize / 2);
	topRight.normal = Vector3(0, -1, 0);

	VSNormalIn bottomLeft;
	bottomLeft.pos = Vector3(-widthSize / 2, 0, lengthSize / 2);
	bottomLeft.normal = Vector3(0, -1, 0);

	VSNormalIn bottomRight;
	bottomRight.pos = Vector3(widthSize / 2, 0, lengthSize / 2);
	bottomRight.normal = Vector3(0, -1, 0);

	vertices.push_back(topLeft);
	int topLeftIndex = index++;
	vertices.push_back(topRight);
	int topRightIndex = index++;
	vertices.push_back(bottomLeft);
	int bottomLeftIndex = index++;
	vertices.push_back(bottomRight);
	int bottomRightIndex = index++;


	indices.push_back(topLeftIndex);
	indices.push_back(bottomRightIndex);
	indices.push_back(bottomLeftIndex);

	indices.push_back(topLeftIndex);
	indices.push_back(topRightIndex);
	indices.push_back(bottomRightIndex);

	this->meshInfo.indexBuffer = DxDevice::instance->CreateIndexBuffer(indices);
	this->indicesCount = indices.size();

	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
	this->verticesCount = vertices.size();
}

BlockModel::DrawLineResult BlockModel::DrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end)
{
	DrawLineResult result;
	Vector3 direction = end - start;
	direction.Normalize();

	auto direction2d = Vector2(direction.x, direction.z);
	direction2d.Normalize();
	auto perpendicular = Vector2(direction2d.y, -direction2d.x);
	//œrodek w 0,0
	auto diameterP1 = perpendicular * cutter->GetRadius();
	auto diameterP2 = -perpendicular * cutter->GetRadius();

	auto diameterP1Coords = GetCoordinatesFromPosition(diameterP1);
	auto diameterP2Coords = GetCoordinatesFromPosition(diameterP2);

	auto diameterPointsCoords = std::vector<std::pair<int, int>>();
	auto saveDiameterPoints = [&](int x, int y)
	{

		diameterPointsCoords.push_back(std::make_pair(x, y));
	};

	BresenhamsAlgorithm::DrawLine(diameterP1Coords.first, diameterP1Coords.second, diameterP2Coords.first, diameterP2Coords.second, LINE_OVERLAP_BOTH, saveDiameterPoints);


	auto cuttingPointsOffets = std::vector<std::tuple<int, int, float>>();
	Vector3 e1; 
	for (auto diameterPointCoords : diameterPointsCoords)
	{
		auto diameterPointPos = GetPositionFromCoordinates(diameterPointCoords);
		auto cuttingPoint = cutter->GetCuttingPointInDirection(diameterPointPos, direction);
		auto [cx, cy] = GetCoordinatesFromPosition(Vector2(cuttingPoint.x, cuttingPoint.z));
		cx -= gridWidthCount / 2;
		cy -= gridLengthCount / 2;
		auto ch = cuttingPoint.y;
		if (cuttingPointsOffets.size() > 0)
		{
			auto [lx, ly, lh] = cuttingPointsOffets[cuttingPointsOffets.size() - 1];
			auto dx = lx - cx;
			auto dy = ly - cy;
			if (dx > 1 || dx < -1 || dy > 1 || dy < -1)
			{
				float heightDiff = lh - ch;
				auto lengthCoords = std::sqrtf(dx * dx + dy * dy);
				BresenhamsAlgorithm::DrawLine(cx, cy, lx, ly, LINE_OVERLAP_NONE, [&](int x, int y) {
					if (
						(x == cx && y == cy) ||
						(x == lx && y == ly)
						)
					{
						return;
					}

					int dxi = x - cx;
					int dyi = y - cy;

					auto currentLengthCoords = std::sqrtf(dxi * dxi + dyi * dyi);

					float heightPercentage;

					heightPercentage = currentLengthCoords / lengthCoords;


					auto heightOffset = heightDiff * heightPercentage;
					cuttingPointsOffets.push_back(std::make_tuple(x, y, ch + heightOffset));

					});
			}
		}
		e1 = cuttingPoint;
		cuttingPointsOffets.push_back(std::make_tuple(cx , cy, ch));
	}



	auto startCoords = GetCoordinatesFromPosition(Vector2(start.x, start.z));
	auto endCoords = GetCoordinatesFromPosition(Vector2(end.x, end.z));

	int xLength = endCoords.first - startCoords.first;
	int yLength = endCoords.second - startCoords.second;

	auto lengthCoords = std::sqrtf(xLength * xLength + yLength * yLength);

	float heightDiff = end.y - start.y;

	auto drawOffset = [&](int x, int y, std::vector<std::tuple<int, int, float>>& offsets)
	{
		int dxi = x - startCoords.first;
		int dyi = y - startCoords.second;

		auto currentLengthCoords = std::sqrtf(dxi * dxi + dyi * dyi);

		float heightPercentage;
		if (lengthCoords == 0) // pinowa
		{
			heightPercentage = 1.0;
		}
		else
		{
			heightPercentage = currentLengthCoords / lengthCoords;
		}

		auto heightOffset = heightDiff * heightPercentage;

		for (auto [dx, dy, dh] : offsets)
		{
			int xIndex = x + dx;
			int yIndex = y + dy;
			int index = yIndex * gridWidthCount + xIndex;
			if (xIndex >= gridWidthCount || yIndex >= gridLengthCount || xIndex < 0 || yIndex < 0)
			{
				continue;
			}
			float height = start.y + heightOffset + dh;

			auto currentHeight = heightMap[index];
			if (currentHeight > height)
			{
				heightMap[index] = height;
				result.anyChanges = true;
				result.maxHeightChange = max(result.maxHeightChange, currentHeight - height);
				result.minHeightCutted = min(result.minHeightCutted, height);
			}

		}
	};

	auto cut = [&](int x, int y)
	{
		drawOffset(x, y, cuttingPointsOffets);
	};

	BresenhamsAlgorithm::DrawLine(startCoords.first, startCoords.second, endCoords.first, endCoords.second, LINE_OVERLAP_BOTH, cut);


	auto drawCircle = [&](int x, int y)
	{
		drawOffset(x, y, cutterCircleOffsets);
	};

	drawCircle(endCoords.first, endCoords.second);

	resetDrawing = true;


	return result;
}

void BlockModel::Draw(std::shared_ptr<Camera> camera)
{
	if (resetDrawing)
	{
		ResetMesh();
	}
	meshInfo.SetUpRender();

	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsNormal->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsNormal->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(nullptr, nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psNormal->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsNormal->SetVertexBuffer(meshInfo.vertexBuffer.get());
	ShadersManager::vsNormal->SetTexture(heightMapTextureSRV.get());
	ShadersManager::psNormal->SetTexture(heightMapTextureSRV.get(), colorTexture.get());

	VSNormalConstantBuffer constantBufferVS;
	constantBufferVS.modelMatrix = Matrix::Identity;
	constantBufferVS.viewMatrix = camera->GetViewMatrix();
	constantBufferVS.inversedViewMatrix = constantBufferVS.viewMatrix.Invert();
	constantBufferVS.perspectiveMatrix = camera->GetPerspectiveMatrix();
	constantBufferVS.gridWidthCount = gridWidthCount;
	constantBufferVS.gridLengthCount = gridLengthCount;
	constantBufferVS.lengthSize = lengthSize;
	constantBufferVS.widthSize = widthSize;
	ShadersManager::vsNormal->SetConstantBuffer(constantBufferVS);

	PSNormalConstantBuffer constantBufferPS;
	constantBufferPS.gridWidthCount = gridWidthCount;
	constantBufferPS.gridLengthCount = gridLengthCount;
	constantBufferPS.lengthSize = lengthSize;
	constantBufferPS.widthSize = widthSize;
	ShadersManager::psNormal->SetConstantBuffer(constantBufferPS);


	DxDevice::instance->context()->DrawIndexed(indicesCount, 0, 0);
}

void BlockModel::SetCutter(std::shared_ptr<ICutter> cutter)
{
	this->cutter = cutter;
	this->cutterCircleOffsets.clear();
	Vector2 top = Vector2(0, cutter->GetRadius());
	Vector2 bottom = Vector2(0, -cutter->GetRadius());
	Vector2 left = Vector2(-cutter->GetRadius(), 0);
	Vector2 right = Vector2(cutter->GetRadius(), 0);

	auto topCoords = GetCoordinatesFromPosition(top);
	auto bottomCoords = GetCoordinatesFromPosition(bottom);
	auto leftCoords = GetCoordinatesFromPosition(left);
	auto rightCoords = GetCoordinatesFromPosition(right);

	int leftX = leftCoords.first;
	int rightX = rightCoords.first;
	int topY = topCoords.second;
	int bottomY = bottomCoords.second;



	for (int y = bottomY; y <= topY; y++)
	{
		for (int x = leftX; x <= rightX; x++)
		{
			auto position = GetPositionFromCoordinates({ x,y });
			float dx = position.x;
			float dy = position.y;
			if (dx * dx + dy * dy <= cutter->GetRadius() * cutter->GetRadius())
			{
				float height = cutter->GetHeight(dx, dy);
				cutterCircleOffsets.push_back(std::make_tuple(x - gridWidthCount / 2, y - gridLengthCount / 2, height));
			}
		}
	}
	resetDrawing = true;
}

bool BlockModel::IsError(float minCuttedHeight, std::string& outErrorMessage)
{
	if (minCuttedHeight < minHeight)
	{
		outErrorMessage += "Entry of the cutter into the base too deep.\n";
		return true;
	}
	return false;
}

BlockModel::~BlockModel()
{
	free((void*)heightMap);
}

void BlockModel::ResetMesh()
{
	resetDrawing = false;


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	auto hr = DxDevice::instance->context()->Map(heightMapTexture.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	float* src = heightMap; // this comes from whatever your input is
	unsigned* dst = (unsigned*)mappedResource.pData; // msr is a D3D11_MAPPED_SUBRESOURCE derived from ID3D11DeviceContext::Map

	for (int i = 0; i < gridLengthCount; i++)
	{
		memcpy(dst, src, gridWidthCount * sizeof(float)); // copy one row at a time because msr.RowPitch may be != (width * 4)
		dst += mappedResource.RowPitch >> 2; // msr.RowPitch is in bytes so for 32-bit data we divide by 4 (or downshift by 2, same thing)
		src += gridWidthCount; // assumes pitch of source data is equal to width * 4
	}

	DxDevice::instance->context()->Unmap(heightMapTexture.get(), 0);
}

std::pair<int, int> BlockModel::GetCoordinatesFromPosition(DirectX::SimpleMath::Vector2 position)
{
	int x = (position.x + widthSize / 2) * ((gridWidthCount - 1) / widthSize) + 0.5;
	int y = (position.y + lengthSize / 2) * ((gridLengthCount - 1) / lengthSize) + 0.5;

	return std::make_pair(x, y);
}

DirectX::SimpleMath::Vector2 BlockModel::GetPositionFromCoordinates(std::pair<int, int> coordinates)
{
	float x = (coordinates.first * widthSize / (gridWidthCount - 1)) - widthSize / 2;
	float y = (coordinates.second * lengthSize / (gridLengthCount - 1)) - lengthSize / 2;

	return Vector2(x, y);
}


void BlockModel::SetMinHeight(float minHeight)
{
	this->minHeight = minHeight;
}