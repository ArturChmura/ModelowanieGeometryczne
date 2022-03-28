#include "BezierCurve.h"
#include <iterator>
#include "Vertex.h"
#include "ImGui/imgui.h"
#include <algorithm>
#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
BezierCurve::BezierCurve(std::vector<Point*> points)
	:IModel("Bezier Curve")
{
	this->shaderInfoSingleColorVs = std::make_shared< ShaderInfoSingleColorVs>();
	this->points = points;
	for (size_t i = 0; i < points.size(); i++)
	{
		points[i]->onModelChangeCallback.push_back([this]() { ResetDrawing(); });
	}
	ResetDrawing();
	//this->activeCamera = activeCamera;
}

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

void BezierCurve::Draw(std::shared_ptr<Camera> camera)
{
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
	if (adaptiveDrawing)
	{
		UpdateVertices();
	}
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
				auto new_end = remove_if(points.begin(), points.end(), [pointsToDelete](Point* point) {
					return std::find(pointsToDelete.begin(), pointsToDelete.end(), point->id) != pointsToDelete.end();
					});
				points.erase(new_end, points.end());
			}
			selectedIndexes.clear();
			ResetDrawing();
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
		DrawPolygonChain(draw);
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
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<int> indices = std::vector<int>();

	int ps = points.size();
	int count = (ps + 1) / 3;
	for (int i = 0; i < count; i ++)
	{
		int pointsCount = min(points.size() - (i*3), 4);
		std::vector<float> coefficientsX;
		std::vector<float> coefficientsY;
		std::vector<float> coefficientsZ;
		for (int j = 0; j < pointsCount; j++)
		{
			auto translation = points[i*3 + j]->GetTranslation();
			coefficientsX.push_back(translation.x);
			coefficientsY.push_back(translation.y);
			coefficientsZ.push_back(translation.z);
		}
		int slices = currentSlices[i];
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
		for (int i = 0; i < points.size(); i++)
		{
			auto point = points[i];
			auto translation = point->GetTranslation();
			vertices.push_back({ {translation.x, translation.y, translation.z } });
		}
		for (int i = currentSize; i < vertices.size() - 1; i++)
		{
			indices.push_back(i);
			indices.push_back(i+1);
		}
	}

	this->indicesCount = indices.size();
	this->verticesCount = vertices.size();
	if (verticesCount > 0)
	{
		this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
		this->meshInfo.indexBuffer = DxDevice::instance->CreateVertexBuffer(indices);
	}
	UpdateAdaptiveDrawing();
}

void BezierCurve::AddPoint(Point* point)
{
	this->points.push_back(point);
	point->onModelChangeCallback.push_back([this]() { ResetDrawing(); });
	ResetDrawing();
}

void BezierCurve::DrawPolygonChain(bool draw)
{
	this->drawPolygonChain = draw;
	ResetDrawing();
}

void BezierCurve::ResetDrawing()
{
	int count = (points.size() + 1) / 3;
	currentSlices.resize(count);
	maxSlices.resize(count);
	adaptiveDrawing = true;
	//auto viewMatrix = (**activeCamera).GetViewMatrix();
	//auto perspectiveMatrix = (**activeCamera).GetPerspectiveMatrix();
	/*auto PV = perspectiveMatrix * viewMatrix;
	std::vector<Vector4> camPositions;
	for (size_t i = 0; i < points.size(); i++)
	{
		auto point = points[i];
		auto translation = point->GetTranslation();
		auto camPosition = Vector4::Transform(translation,PV);
		camPosition /= camPosition.w;
		camPositions.push_back(camPosition);
	}
	for (int i = 0; i < currentSlices.size(); i++)
	{
		int pointsCount = min(points.size() - (i * 3), 4);
		float area = 0.0f;
		for (int j = 0; j < pointsCount - 1; j++)
		{
			area += (camPositions[i * 3 + j] - camPositions[i * 3 + j + 1]).LengthSquared();
		}
		currentSlices[i] = minSlices;
		maxSlices[i] = (int)area;
	}*/
	UpdateVertices();
}

void BezierCurve::UpdateAdaptiveDrawing()
{
	bool updated = false;
	for (size_t i = 0; i < currentSlices.size(); i++)
	{
		if (currentSlices[i] < maxSlices[i])
		{
			updated = true;
			currentSlices[i] *= 2;
		}
	}
	adaptiveDrawing = updated;
	
}