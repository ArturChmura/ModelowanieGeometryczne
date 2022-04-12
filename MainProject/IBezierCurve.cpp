#include "IBezierCurve.h"
#include "ImGui/imgui.h"
#include <algorithm>
using namespace DirectX;
using namespace DirectX::SimpleMath;

IBezierCurve::IBezierCurve(std::vector<std::shared_ptr<Point>> points, std::string name)
	:IModel(name)
{
	for (size_t i = 0; i < points.size(); i++)
	{
		AddPoint(points[i]);
	}
	this->shaderInfoSingleColorVs = std::make_shared<ShaderInfoSingleColorVs>();
}

float IBezierCurve::DeCasteljeu(std::vector<float> coefficients, float t)
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

			if (ImGui::Selectable((point->name + "##" + std::to_string(point->id)).c_str(), is_selected))
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
				if (std::any_of(this->points.begin(), this->points.end(), [point](std::shared_ptr<Point> p) {return p->id == point->id; }))
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

DirectX::SimpleMath::Vector3 IBezierCurve::GetRotation()
{
	return DirectX::SimpleMath::Vector3();
}

void IBezierCurve::RotateFromPoint(Vector4 globalPoint, DirectX::XMFLOAT3 ratation)
{
}
