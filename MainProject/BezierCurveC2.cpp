#include "BezierCurveC2.h"
#include "Vertex.h"
#include "ImGui/imgui.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

BezierCurveC2::BezierCurveC2(std::vector<std::shared_ptr<Point>> points)
	:IBezierCurve(points, "Bezier Curve C2")
{
}

void BezierCurveC2::Draw(std::shared_ptr<Camera> camera)
{
	if (resetDrawing)
	{
		UpdateBezierPoints();
	}
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

	if (bezierRepresentation)
	{
		for (int i = 0; i < bezierPoints.size(); i++)
		{
			bezierPoints[i]->Draw(camera);
		}
	}
	else
	{
		for (int i = 0; i < points.size(); i++)
		{
			points[i]->Draw(camera);
		}
	}
}

void BezierCurveC2::SetRepresentation(bool bezier)
{
	this->bezierRepresentation = bezier;
	for (auto pointPtr : points)
	{
		pointPtr->SetVisible(!bezier);
	}
	ResetDrawing();
}

void BezierCurveC2::RenderGUI()
{
	IBezierCurve::RenderGUI();

	bool rep = bezierRepresentation;
	if (ImGui::Checkbox("Bezier Representation", &rep))
	{
		SetRepresentation(rep);
	}
}

void BezierCurveC2::UpdateVertices()
{
	resetDrawing = false;
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<int> indices = std::vector<int>();
	int ps = bezierPoints.size();
	int sectionCount = (ps + 1) / 3;
	for (int i = 0; i < sectionCount; i++)
	{
		int pointsCount = min(bezierPoints.size() - (i * 3), 4);
		std::vector<float> coefficientsX;
		std::vector<float> coefficientsY;
		std::vector<float> coefficientsZ;
		for (int j = 0; j < pointsCount; j++)
		{
			auto translation = bezierPoints[i * 3 + j]->GetTranslation();
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
	if (drawPolygonChain)
	{
		auto currentSize = vertices.size();
		if (bezierRepresentation)
		{
			if (bezierPoints.size() > 1)
			{
				for (int i = 0; i < bezierPoints.size(); i++)
				{
					auto point = bezierPoints[i];
					auto translation = point->GetTranslation();
					vertices.push_back({ {translation.x, translation.y, translation.z } });
				}
			}
		}
		else
		{
			if (points.size() > 1)
			{
				for (int i = 0; i < points.size(); i++)
				{
					auto point = points[i];
					auto translation = point->GetTranslation();
					vertices.push_back({ {translation.x, translation.y, translation.z } });
				}
			}
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

inline Vector3 GetMidPoint(Vector3 a, Vector3 b)
{
	return (a + b) / 2.0f;
}

inline Vector3 GetOneThirdPoint(Vector3 a, Vector3 b)
{
	return a + (b - a) / 3.0f;
}

inline Vector3 GetTwoThirdPoint(Vector3 a, Vector3 b)
{
	return GetOneThirdPoint(b, a);
}

void BezierCurveC2::UpdateBezierPoints()
{
	if (points.size() < 4)
	{
		bezierPoints.resize(0);
		return;
	}
	int bezierPointsCount = 1 + (points.size() - 3) * 3;
	bezierPoints.resize(bezierPointsCount);
	std::vector<Vector3> deBoorePoints(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		auto p = points[i]->GetTranslation();
		deBoorePoints[i] = Vector3(p);
	}
	auto v1 = GetTwoThirdPoint(deBoorePoints[0], deBoorePoints[1]);
	auto v3 = GetOneThirdPoint(deBoorePoints[1], deBoorePoints[2]);
	auto v2 = GetMidPoint(v1, v3);

	bezierPoints[0] = std::make_shared<Point>(v2);
	int j = 1;
	for (int i = 3; i < deBoorePoints.size(); i++)
	{
		bezierPoints[j++] = std::make_shared<Point>(v3);
		v1 = GetTwoThirdPoint(deBoorePoints[i - 2], deBoorePoints[i - 1]);
		v3 = GetOneThirdPoint(deBoorePoints[i - 1], deBoorePoints[i]);
		v2 = GetMidPoint(v1, v3);
		bezierPoints[j++] = std::make_shared<Point>(v1);
		bezierPoints[j++] = std::make_shared<Point>(v2);
	}
	for (auto point : bezierPoints)
	{
		point->ChangeDefaultColor(Vector3(0, 0.5, 1));
	}
	for (int i = 0; i < bezierPoints.size(); i++)
	{
		auto point = bezierPoints[i];
		point->onModelChangeCallback.push_back(
			{ id,
			[this, i](std::shared_ptr<Point> point)
			{this->OnBezierPointChange(i, point->GetTranslation()); } });
	}
}

void BezierCurveC2::OnBezierPointChange(int pointIndex, Vector4 translation)
{
	int closestDeBooreIndex = (pointIndex + 1) / 3 + 1;
	int delta = 0;
	if ((pointIndex % 3) == 1) // po prawo od bli¿szego 
	{
		delta = 1;
	}
	else if ((pointIndex % 3) == 2)// po lewo od bli¿szego 
	{
		delta = -1;
	}
	else // punkt œrodkowy
	{
		auto A = Vector3(points[closestDeBooreIndex - 1]->GetTranslation());
		auto B = Vector3(points[closestDeBooreIndex + 1]->GetTranslation());
		auto X = Vector3(translation);
		auto S = -B / 4.0f - A / 4.0f + 3 * X / 2;

		points[closestDeBooreIndex]->SetTranslation(S.x, S.y, S.z);
		return;
	}
	int farDeBooreIndex = closestDeBooreIndex + delta;
	auto farPos = Vector3(points[farDeBooreIndex]->GetTranslation());
	auto fromFarToNew = Vector3(translation) - farPos;
	auto closeDeBooreTrans = farPos + fromFarToNew * 3.0f / 2.0f;
	points[closestDeBooreIndex]->SetTranslation(closeDeBooreTrans.x, closeDeBooreTrans.y, closeDeBooreTrans.z);
}

void BezierCurveC2::UpdateSlicesCount(std::shared_ptr<Camera> camera)
{
	int segmentsCount = (bezierPoints.size() + 1) / 3;
	desiredSlices.resize(segmentsCount);
	currentSlices.resize(segmentsCount);
	auto viewMatrix = camera->GetViewMatrix();
	auto perspectiveMatrix = camera->GetPerspectiveMatrix();
	auto PV = viewMatrix * perspectiveMatrix;
	std::vector<Vector2> camPositions;
	bool isLargerNow = false;
	for (size_t i = 0; i < bezierPoints.size(); i++)
	{
		auto point = bezierPoints[i];
		auto translation = point->GetTranslation();
		auto camPosition = Vector4::Transform(translation, PV);
		camPosition /= camPosition.w != 0 ? camPosition.w : 1.0;
		Vector2 xy(
			(camPosition.x + 1) * DxDevice::winowSize.cx / 2.0f,
			(camPosition.y + 1) * DxDevice::winowSize.cy / 2.0f
		);

		camPositions.push_back(xy);
	}
	for (int i = 0; i < segmentsCount; i++)
	{
		int pointsCount = min(bezierPoints.size() - (i * 3), 4);
		float area = 0.0f;
		for (int j = 0; j < pointsCount - 1; j++)
		{
			auto length = (camPositions[i * 3 + j] - camPositions[i * 3 + j + 1]).Length() / 10.0f;
			length = min(length, 100);
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

std::shared_ptr<IModel> BezierCurveC2::SelectFromScreenCoords(float x, float y, DirectX::SimpleMath::Matrix VP)
{
	if (bezierRepresentation)
	{
		for (auto point : bezierPoints)
		{
			auto modelPtr = point->SelectFromScreenCoords(x, y, VP);
			if (modelPtr)
			{
				return modelPtr;
			}
		}
	}

	return nullptr;
}
