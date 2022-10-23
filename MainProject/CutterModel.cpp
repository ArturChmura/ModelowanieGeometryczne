#include "CutterModel.h"
#include "Helpers.h"
#include "imgui.h"
#include "vsConstColor.h"
#include "Pair.h"

using namespace DirectX::SimpleMath;
CutterModel::CutterModel(float radius, std::function<float(float, float)> getHeight)
{
	this->radius = radius;
	this->getHeight = getHeight;
	UpdateSlicesCount();
}

void CutterModel::SetRadius(float radius)
{
	this->radius = radius;
	UpdateSlicesCount();
}

void CutterModel::SetTranslation(float x, float y, float z)
{
	auto prevTranslation = GetTranslation();
	VertexModel::SetTranslation(x, y, z);
	auto translation = GetTranslation();

	auto distance = Vector4::Distance(prevTranslation, translation);
	Rotate(0, distance/10, 0);

}

void CutterModel::UpdateSlicesCount()
{
	auto verticesCount = ringPointsCount * (radiusPointsCount - 1) + 1 + ringPointsCount;
	auto indicesCount = ringPointsCount * (radiusPointsCount - 1) + ringPointsCount * (radiusPointsCount - 1) + ringPointsCount + ringPointsCount;

	std::vector<VSConstColorIn> vertices = std::vector<VSConstColorIn>();
	vertices.reserve(verticesCount);
	std::vector<int> indices = std::vector<int>();
	indices.reserve(indicesCount);



	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
	this->verticesCount = vertices.size();

	this->meshInfo.indexBuffer = DxDevice::instance->CreateVertexBuffer(indices);
	this->indicesCount = indices.size();

	int** indexArray = new int* [radiusPointsCount - 1];
	for (int i = 0; i < radiusPointsCount - 1; i++)
	{
		indexArray[i] = new int[ringPointsCount];
		std::fill_n(indexArray[i], ringPointsCount, -1);
	}

	int index = 0;
	VSConstColorIn vertex = { Vector3(0,0,0) };
	index++;
	vertices.push_back(vertex);

	for (int radiusIndex = 0; radiusIndex < radiusPointsCount - 1; radiusIndex++)
	{
		for (int ringIndex = 0; ringIndex < ringPointsCount; ringIndex++)
		{
			float alpha = 2 * PI * ringIndex / (float)ringPointsCount;
			auto direction = Vector2(cosf(alpha), sinf(alpha));
			auto planePos = direction * radius * (radiusIndex + 1) / (float)radiusPointsCount;

			auto height = getHeight(planePos.x, planePos.y);

			auto position = Vector3(planePos.x, height, planePos.y);

			VSConstColorIn vertex = { position };
			vertices.push_back(vertex);
			indexArray[radiusIndex][ringIndex] = index++;

		}
	}
	for (int radiusIndex = 0; radiusIndex < radiusPointsCount - 1; radiusIndex++)
	{
		for (int ringIndex = 0; ringIndex < ringPointsCount; ringIndex++)
		{
			Pair<int> top = { radiusIndex, modulo2(ringIndex + 1, ringPointsCount) };
			Pair<int> right = { modulo2(radiusIndex - 1, radiusPointsCount - 1), ringIndex };




			indices.push_back(indexArray[radiusIndex][ringIndex]);
			indices.push_back(indexArray[top.a][top.b]);

			indices.push_back(indexArray[radiusIndex][ringIndex]);
			auto rightIndex = indexArray[right.a][right.b];
			if (radiusIndex == 0)
			{
				rightIndex = 0;
			}
			indices.push_back(rightIndex);


		}
	}

	int upRingIndex = 0;
	for (int ringIndex = 0; ringIndex < ringPointsCount; ringIndex++)
	{
		auto downIndex = indexArray[ringPointsCount - 2][ringIndex];
		auto downVertex = vertices[downIndex];
		auto upVertex = downVertex;
		upVertex.pos.y += 100;

		vertices.push_back(upVertex);

		indices.push_back(downIndex);
		indices.push_back(index + upRingIndex);


		auto nextUprRingIndex = modulo2(upRingIndex + 1, ringPointsCount);

		indices.push_back(index + upRingIndex);
		indices.push_back(index + nextUprRingIndex);

		upRingIndex++;
	}

	free(indexArray);
	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
	this->verticesCount = vertices.size();

	this->meshInfo.indexBuffer = DxDevice::instance->CreateVertexBuffer(indices);
	this->indicesCount = indices.size();
}

void CutterModel::Accept(AbstractModelVisitor& visitor)
{
}
