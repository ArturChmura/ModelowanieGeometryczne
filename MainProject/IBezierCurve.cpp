#include "IBezierCurve.h"
#include "imgui.h"
#include <algorithm>
#include "ShadersManager.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

IBezierCurve::IBezierCurve(std::vector<std::shared_ptr<Point>> points, std::string name)
	:IUnmovableModel(name)
{
	for (size_t i = 0; i < points.size(); i++)
	{
		AddPoint(points[i]);
	}
	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
}



void IBezierCurve::UpdateVertices()
{
	resetDrawing = false;

	std::vector<VSBezierIn> vertices = std::vector<VSBezierIn>();
	auto bezierPoints = GetBezierPoints();
	int ps = bezierPoints.size();
	int sectionCount = (ps + 1) / 3;
	for (int i = 0; i < sectionCount; i++)
	{
		VSBezierIn in;
		int pointsCount = min(ps - (i * 3), 4);

		Vector3 t1 = 0 < pointsCount ? bezierPoints[i * 3 + 0] : Vector3();
		Vector3 t2 = 1 < pointsCount ? bezierPoints[i * 3 + 1] : Vector3();
		Vector3 t3 = 2 < pointsCount ? bezierPoints[i * 3 + 2] : Vector3();
		Vector3 t4 = 3 < pointsCount ? bezierPoints[i * 3 + 3] : Vector3();
		in.X = Vector4(t1.x, t2.x, t3.x, t4.x);
		in.Y = Vector4(t1.y, t2.y, t3.y, t4.y);
		in.Z = Vector4(t1.z, t2.z, t3.z, t4.z);
		in.SIZE = pointsCount;
		vertices.push_back(in);
	}

	this->verticesCount = vertices.size();
	//if (verticesCount > 0)
	{
		this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
	}
}

void IBezierCurve::Draw(std::shared_ptr<Camera> camera)
{
	if (resetDrawing)
	{
		UpdateVertices();
	}

	meshInfo.SetUpRender();

	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsBezier->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsBezier->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(ShadersManager::gsBezier->m_geometryShader.get(), nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psConstColor->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsBezier->SetVertexBuffer(meshInfo.vertexBuffer.get());

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp = v * p;

	GSBezierConstantBuffer gsCB;
	gsCB.mvp = mvp;
	gsCB.screenWidth = DxDevice::windowSize.cx;
	gsCB.screenHeight = DxDevice::windowSize.cy;
	ShadersManager::gsBezier->SetConstantBuffer(gsCB);

	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	DxDevice::instance->context()->Draw(verticesCount, 0);

	if (drawPolygonChain)
	{
		auto points = GetPolygonChainPoints();
		PolygonalChain::Draw(camera, points);
	}
}

void IBezierCurve::AddPoint(std::shared_ptr<Point> point)
{
	this->points.push_back(point);
	point->onModelChangeCallback.Add([this](std::shared_ptr<Point> point) { this->ResetDrawing(); }, id);
	point->onRemovedFromSceneCallback.Add([this](std::shared_ptr < Point> point) { RemovePoint(point->id); }, id);
	ResetDrawing();
}

void IBezierCurve::AddPointToBeggining(std::shared_ptr<Point> point)
{
	this->points.insert(this->points.begin(), point);
	point->onModelChangeCallback.Add([this](std::shared_ptr<Point> point) { this->ResetDrawing(); }, id);
	point->onRemovedFromSceneCallback.Add([this](std::shared_ptr < Point> point) { RemovePoint(point->id); }, id);
	ResetDrawing();
}

void IBezierCurve::RemovePoint(int pointId)
{
	auto iter = std::find_if(points.begin(), points.end(), [pointId](std::shared_ptr<Point> point) {
		return point->id == pointId;
		});
	if (iter == points.end())
	{
		return;
	}
	auto model = (*iter);
	points.erase(iter, iter + 1);

	model->onModelChangeCallback.Remove(id);
	model->onRemovedFromSceneCallback.Remove(id);

	ResetDrawing();
}

void IBezierCurve::SetDrawPolygonChain(bool draw)
{
	this->drawPolygonChain = draw;
}

void IBezierCurve::ResetDrawing()
{
	resetDrawing = true;
}



void IBezierCurve::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	meshInfo.color = color;
}

void IBezierCurve::RenderGUI()
{
	IModel::RenderGUI();
	if (ImGui::BeginListBox("##ObjectsListBox", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < points.size(); i++)
		{
			auto point = points[i];
			const bool is_selected = std::find(selectedIndexes.begin(), selectedIndexes.end(), i) != selectedIndexes.end();
			if (ImGui::Selectable((point->name + "##" + std::to_string(i)).c_str(), is_selected))
			{
				auto io = ImGui::GetIO();
				if (io.KeyCtrl)
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
				else
				{
					selectedIndexes.clear();
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
			Point::onSelectCallback.Remove(id);
			Point::onAddedToSceneCallback.Remove(id);
		}
	}
	else
	{
		if (ImGui::Button("Start adding vertices"))
		{
			isAddingMode = true;
			auto callback = [this](std::shared_ptr<Point> point) {
				AddPoint(point);
			};
			Point::onSelectCallback.Add(callback, id);
			Point::onAddedToSceneCallback.Add(callback, id);
		}
	}
	bool draw = drawPolygonChain;
	if (ImGui::Checkbox("Draw polygonal chain", &draw))
	{
		SetDrawPolygonChain(draw);
	}
}

void IBezierCurve::OnRemovedFromScene()
{
	std::vector<int> toDelete;
	for (auto point : points)
	{
		toDelete.push_back(point->id);
	}
	for (auto id : toDelete)
	{
		RemovePoint(id);
	}
	Point::onSelectCallback.Remove(id);
	Point::onAddedToSceneCallback.Remove(id);
}

std::vector<std::shared_ptr<IModel>> IBezierCurve::GetContainingModels()
{
	std::vector<std::shared_ptr<IModel>> models(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		models[i] = points[i];
	}

	return models;
}


