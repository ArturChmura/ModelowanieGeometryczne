#include "BezierCurveC0.h"
#include <iterator>
#include "Vertex.h"
#include "ImGui/imgui.h"
#include <algorithm>
#include "SimpleMath.h"
#include "Helpers.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
BezierCurveC0::BezierCurveC0(std::vector<std::shared_ptr<Point>> points)
	:IBezierCurve(points, "Bezier Curve C0")
{
}

void BezierCurveC0::Draw(std::shared_ptr<Camera> camera)
{
	UpdateSlicesCount(camera);
	if (resetDrawing)
	{
		UpdateVertices();
	}
	meshInfo.SetUpRender();
	shaderInfoSingleColorVs->SetUpRender();
	shaderInfoSingleColorVs->SetVertexBuffer(meshInfo.vertexBuffer.get());

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	shaderInfoSingleColorVs->constantBufferStruct.mvp =
		XMLoadFloat4x4(&v) *
		XMLoadFloat4x4(&p);
	shaderInfoSingleColorVs->constantBufferStruct.color = XMLoadFloat3(&meshInfo.color);


	shaderInfoSingleColorVs->CopyConstantBuffers();

	DxDevice::instance->context()->DrawIndexed(indicesCount, 0, 0);

	for (int i = 0; i < points.size(); i++)
	{
		points[i]->Draw(camera);
	}
}

void BezierCurveC0::RenderGUI()
{
	IBezierCurve::RenderGUI();
}

void BezierCurveC0::UpdateVertices()
{
	resetDrawing = false;
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<int> indices = std::vector<int>();

	int ps = points.size();
	int sectionCount = (ps + 1) / 3;
	for (int i = 0; i < sectionCount; i++)
	{
		int pointsCount = min(points.size() - (i * 3), 4);
		std::vector<float> coefficientsX;
		std::vector<float> coefficientsY;
		std::vector<float> coefficientsZ;
		for (int j = 0; j < pointsCount; j++)
		{
			auto translation = points[i * 3 + j]->GetTranslation();
			coefficientsX.push_back(translation.x);
			coefficientsY.push_back(translation.y);
			coefficientsZ.push_back(translation.z);
		}
		int slices = desiredSlices[i];
		currentSlices[i] = slices;
		float step = 1.0f / slices;
		for (float t = 0; t <= 1.0f; t += step)
		{
			auto x = DeCasteljeu(coefficientsX, t);
			auto y = DeCasteljeu(coefficientsY, t);
			auto z = DeCasteljeu(coefficientsZ, t);
			vertices.push_back({ { x,y,z } });
		}
	}
	for (int i = 0; i < (int)vertices.size() - 1; i++)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	if (drawPolygonChain && points.size() > 1)
	{
		auto currentSize = vertices.size();
		for (int i = 0; i < points.size(); i++)
		{
			auto point = points[i];
			auto translation = point->GetTranslation();
			vertices.push_back({ {translation.x, translation.y, translation.z } });
		}
		for (int i = currentSize; i < (int)vertices.size() - 1; i++)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
		}
	}

	this->indicesCount = indices.size();
	this->verticesCount = vertices.size();
	if (verticesCount > 0)
	{
		this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
		this->meshInfo.indexBuffer = DxDevice::instance->CreateVertexBuffer(indices);
	}
}



void BezierCurveC0::UpdateSlicesCount(std::shared_ptr<Camera> camera)
{
	int segmentsCount = (points.size() + 1) / 3;
	desiredSlices.resize(segmentsCount);
	currentSlices.resize(segmentsCount);
	auto viewMatrix = camera->GetViewMatrix();
	auto perspectiveMatrix = camera->GetPerspectiveMatrix();
	auto PV = viewMatrix * perspectiveMatrix;
	std::vector<Vector2> camPositions;
	bool isLargerNow = false;
	for (size_t i = 0; i < points.size(); i++)
	{
		auto point = points[i];
		auto translation = point->GetTranslation();
		auto camPosition = Vector4::Transform(translation, PV);
		camPosition /= camPosition.w;
		Vector2 xy(
			(camPosition.x + 1) * DxDevice::winowSize.cx / 2.0f,
			(camPosition.y + 1) * DxDevice::winowSize.cy / 2.0f
		);

		camPositions.push_back(xy);
	}
	for (int i = 0; i < segmentsCount; i++)
	{
		int pointsCount = min(points.size() - (i * 3), 4);
		float area = 0.0f;
		for (int j = 0; j < pointsCount - 1; j++)
		{
			auto length = (camPositions[i * 3 + j] - camPositions[i * 3 + j + 1]).Length();
			length = min(length, DxDevice::winowSize.cx * 2);
			area += length;
		}
		int slices = max((int)area, 1);
		if (currentSlices[i] < slices)
		{
			isLargerNow = true;
		}
		desiredSlices[i] = slices;
	}
	if (isLargerNow)
	{
		ResetDrawing();
	}
}


