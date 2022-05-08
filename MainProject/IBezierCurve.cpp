#include "IBezierCurve.h"
#include "ImGui/imgui.h"
#include <algorithm>
#include "ShadersManager.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

IBezierCurve::IBezierCurve(std::vector<std::shared_ptr<Point>> points, std::string name)
	:IModel(name)
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
		std::vector<float> coefficientsX;
		std::vector<float> coefficientsY;
		std::vector<float> coefficientsZ;
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

	if (drawPolygonChain)
	{
		auto points = GetPolygonChainPoints();
		polygonalChain = std::make_shared<PolygonalChain>(points);
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
	gsCB.screenWidth = DxDevice::winowSize.cx;
	gsCB.screenHeight = DxDevice::winowSize.cy;
	ShadersManager::gsBezier->SetConstantBuffer(gsCB);

	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	DxDevice::instance->context()->Draw(verticesCount, 0);

	if (drawPolygonChain && polygonalChain)
	{
		polygonalChain->Draw(camera);
	}
}

void IBezierCurve::AddPoint(std::shared_ptr<Point> point)
{
	this->points.push_back(point);
	point->onModelChangeCallback.push_back({ id,[this](std::shared_ptr<Point> point) { this->ResetDrawing(); } });
	point->onRemovedFromSceneCallback.push_back({ id,[this](std::shared_ptr < Point> point) { RemovePoint(point->id); } });
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
	auto newEnd = std::remove_if(model->onModelChangeCallback.begin(), model->onModelChangeCallback.end(), [this](std::tuple<int, std::function<void(std::shared_ptr<Point>)>> t) { return id == std::get<0>(t); });
	model->onModelChangeCallback.erase(newEnd, model->onModelChangeCallback.end());


	auto newEnd2 = std::remove_if(model->onRemovedFromSceneCallback.begin(), model->onRemovedFromSceneCallback.end(), [this](std::tuple<int, std::function<void(std::shared_ptr<Point>)>> t) { return id == std::get<0>(t); });
	model->onRemovedFromSceneCallback.erase(newEnd2, model->onRemovedFromSceneCallback.end());
	ResetDrawing();
}

void IBezierCurve::SetDrawPolygonChain(bool draw)
{
	this->drawPolygonChain = draw;
	if (drawPolygonChain)
	{

	}
	ResetDrawing();
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
			if (ImGui::Selectable((point->name + "##"+ std::to_string(i)).c_str(), is_selected))
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
			Point::onSelectCallback.pop_back();
			Point::onAddedToSceneCallback.pop_back();
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
	if (Point::onSelectCallback.size() > 0)
	{
		Point::onSelectCallback.pop_back();
	}
	if (Point::onAddedToSceneCallback.size() > 0)
	{
		Point::onAddedToSceneCallback.pop_back();
	}
}



//Unused from IModel;
void IBezierCurve::SetScale(float x, float y, float z)
{
}

void IBezierCurve::Scale(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 IBezierCurve::GetScale()
{
	return DirectX::SimpleMath::Vector3();
}

void IBezierCurve::ScaleFromPoint(Vector4 point, DirectX::XMFLOAT3 scale)
{
}

void IBezierCurve::SetTranslation(float x, float y, float z)
{
}

void IBezierCurve::Translate(float x, float y, float z)
{
}

Vector4 IBezierCurve::GetTranslation()
{
	return DirectX::SimpleMath::Vector4();
}

void IBezierCurve::SetRotation(float pitch, float yaw, float roll)
{
}

void IBezierCurve::Rotate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 IBezierCurve::GetRotation()
{
	return DirectX::SimpleMath::Vector3();
}

void IBezierCurve::RotateFromPoint(Vector4 globalPoint, DirectX::XMFLOAT3 ratation)
{

}


