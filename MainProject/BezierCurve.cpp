#include "BezierCurve.h"
#include <iterator>
#include "Vertex.h"
#include "ImGui/imgui.h"
#include <algorithm>
#include "SimpleMath.h"
#include "Helpers.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
BezierCurve::BezierCurve(std::vector<Point*> points)
	:IModel("Bezier Curve")
{
	this->shaderInfoSingleColorVs = std::make_shared< ShaderInfoSingleColorVs>();
	for (size_t i = 0; i < points.size(); i++)
	{
		AddPoint(points[i]);
	}
}

void BezierCurve::Draw(std::shared_ptr<Camera> camera)
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
}

void BezierCurve::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	meshInfo.color = color;
}

void BezierCurve::RenderGUI()
{
	IModel::RenderGUI();
	if (ImGui::BeginListBox("##ObjectsListBox", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < points.size(); i++)
		{
			auto point = points[i];
			const bool is_selected = std::find(selectedIndexes.begin(), selectedIndexes.end(), i) != selectedIndexes.end();

			if (ImGui::Selectable((point->name + "##" + std::to_string(point->id)).c_str(), is_selected))
			{
				if (is_selected)
				{
					auto newEnd = std::remove(selectedIndexes.begin(), selectedIndexes.end(), i);
					selectedIndexes.erase(newEnd, selectedIndexes.end());
				}
				else
				{
					selectedIndexes.push_back(i);
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	if (selectedIndexes.size() > 0)
	{
		if (ImGui::Button("Remove selected"))
		{
			std::vector<int> pointsToDelete;
			for (auto index : selectedIndexes)
			{
				pointsToDelete.push_back(points[index]->id);
			}
			for (auto point : pointsToDelete)
			{
				RemovePoint(point);
			}
			selectedIndexes.clear();
		}
	}
	if (isAddingMode)
	{
		if (ImGui::Button("Stop adding vertices"))
		{
			isAddingMode = false;
			Point::onSelectCallback.pop_back();
			Point::onAddedToSceneCallback.pop_back();
		}
	}
	else
	{
		if (ImGui::Button("Start adding vertices"))
		{
			isAddingMode = true;
			auto callback = [this](Point* point) {
				if (std::any_of(this->points.begin(), this->points.end(), [point](Point* p) {return p->id == point->id; }))
				{

				}
				else
				{
					AddPoint(point);
				}
			};
			Point::onSelectCallback.push_back(callback);
			Point::onAddedToSceneCallback.push_back(callback);
		}
	}
	bool draw = drawPolygonChain;
	if (ImGui::Checkbox("Draw polygonal chain", &draw))
	{
		SetDrawPolygonChain(draw);
	}
}

float BezierCurve::DeCasteljeu(std::vector<float> coefficients, float t)
{
	int n = coefficients.size();
	float* piramid = new float[n]();
	for (int i = 0; i < n; i++)
	{
		piramid[i] = coefficients[i];
	}
	for (int i = 0; i < n - 1; i++)
	{
		int currentSize = n - i;
		for (int j = 0; j < currentSize - 1; j++)
		{
			piramid[j] = piramid[j] * (1 - t) + piramid[j + 1] * t;
		}
	}
	float value = piramid[0];

	delete[] piramid;
	return value;
}

void BezierCurve::UpdateVertices()
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

void BezierCurve::AddPoint(Point* point)
{
	this->points.push_back(point);
	point->onModelChangeCallback.push_back({ id,[this]() { this->ResetDrawing(); } });
	point->onRemovedFromSceneCallback.push_back({ id,[this](Point* point) { RemovePoint(point->id); } });
	ResetDrawing();
}

void BezierCurve::RemovePoint(int pointId)
{
	auto iter = std::find_if(points.begin(), points.end(), [pointId](Point* point) {
		return point->id == pointId;
		});
	auto model = (*iter);
	points.erase(iter, iter + 1); 
	auto newEnd = std::remove_if(model->onModelChangeCallback.begin(), model->onModelChangeCallback.end(), [this](std::tuple<int, std::function<void()>> t) { return id == std::get<0>(t); });
	model->onModelChangeCallback.erase(newEnd, model->onModelChangeCallback.end());


	auto newEnd2 = std::remove_if(model->onRemovedFromSceneCallback.begin(), model->onRemovedFromSceneCallback.end(), [this](std::tuple<int, std::function<void(Point*)>> t) { return id == std::get<0>(t); });
	model->onRemovedFromSceneCallback.erase(newEnd2, model->onRemovedFromSceneCallback.end());
	ResetDrawing();
}

void BezierCurve::SetDrawPolygonChain(bool draw)
{
	this->drawPolygonChain = draw;
	ResetDrawing();
}

void BezierCurve::UpdateSlicesCount(std::shared_ptr<Camera> camera)
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

void BezierCurve::ResetDrawing()
{
	resetDrawing = true;
}


//Unused from IModel;
void BezierCurve::SetScale(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 BezierCurve::GetScale()
{
	return DirectX::SimpleMath::Vector3();
}

void BezierCurve::ScaleFromPoint(Vector4 point, DirectX::XMFLOAT3 scale)
{
}

void BezierCurve::SetTranslation(float x, float y, float z)
{
}

void BezierCurve::Translate(float x, float y, float z)
{
}

Vector4 BezierCurve::GetTranslation()
{
	return DirectX::SimpleMath::Vector4();
}

void BezierCurve::SetRotation(float pitch, float yaw, float roll)
{
}

DirectX::SimpleMath::Vector3 BezierCurve::GetRotation()
{
	return DirectX::SimpleMath::Vector3();
}

void BezierCurve::RotateFromPoint(Vector4 globalPoint, DirectX::XMFLOAT3 ratation)
{
}

void BezierCurve::OnRemovedFromScene()
{
	std::vector<int> toDelete;
	for (auto point : points)
	{
		toDelete.push_back(point->id);
	}
	for (auto id: toDelete)
	{

		RemovePoint(id);
	}
	if (Point::onSelectCallback.size() > 0)
	{
		Point::onSelectCallback.pop_back();
	}
	if (Point::onAddedToSceneCallback.size() > 0)
	{
		Point::onAddedToSceneCallback.pop_back();
	}
}
