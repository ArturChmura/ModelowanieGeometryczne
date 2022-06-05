#include "BezierSurfaceC0.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;


BezierSurfaceC0::BezierSurfaceC0(std::vector<std::shared_ptr<SingleBezierSurfaceC0>> singleSurfaces, int horizontalCount, int verticalCount, std::string name)
	:IUnmovableModel(name)
{
	this->singleSurfaces = singleSurfaces;
	this->horizontalSlicesCount = horizontalCount;
	this->verticalSlicesCount = verticalCount;
}

void BezierSurfaceC0::Draw(std::shared_ptr<Camera> camera)
{
	for (int i = 0; i < singleSurfaces.size(); i++)
	{
		singleSurfaces[i]->Draw(camera);
	}

	if (drawPolygonChain)
	{
		for (auto single : singleSurfaces)
		{
			single->DrawPolygonChain(camera);
		}
	}
}

void BezierSurfaceC0::RenderGUI()
{
	IModel::RenderGUI();
	static int selectedIndex = -1;
	if (ImGui::BeginListBox("##ObjectsListBox", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < this->singleSurfaces.size(); i++)
		{
			auto model = this->singleSurfaces[i];
			const bool is_selected = i == selectedIndex;

			if (ImGui::Selectable((model->name + "##" + std::to_string(model->id)).c_str(), is_selected))
			{
				if (is_selected)
				{
					selectedIndex = -1;
					this->singleSurfaces[i]->ChangeColor(selectedColor);
				}
				else
				{
					if (selectedIndex >= 0)
					{
						this->singleSurfaces[selectedIndex]->ChangeColor(selectedColor);
					}
					selectedIndex = i;
					this->singleSurfaces[i]->ChangeColor(Vector3(1, 0, 0));
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	if (ImGui::Checkbox("Draw polygon chain", &drawPolygonChain))
	{
		for (auto single : singleSurfaces)
		{
			single->SetDrawPolygonChain(drawPolygonChain);
		}
	}
	if (selectedIndex >= 0)
	{
		this->singleSurfaces[selectedIndex]->RenderGUI();
	}
}


void BezierSurfaceC0::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	for (auto single : singleSurfaces)
	{
		single->ChangeColor(color);
	}
}

std::vector<std::shared_ptr<Point>> BezierSurfaceC0::GetEdgePoints()
{
	std::vector<std::shared_ptr<Point>> edgePoints;

	auto topLeftPoint = singleSurfaces[0]->points[0][0];

	for (size_t j = 0; j < horizontalSlicesCount; j++) // top row
	{
		auto single = singleSurfaces[j];
		edgePoints.push_back(single->points[0][1]);
		edgePoints.push_back(single->points[0][2]);
		edgePoints.push_back(single->points[0][3]);
	}	
	for (size_t i = 0; i < verticalSlicesCount; i++) // right edge
	{
		auto single = singleSurfaces[i*horizontalSlicesCount + horizontalSlicesCount - 1];
		edgePoints.push_back(single->points[1][3]);
		edgePoints.push_back(single->points[2][3]);
		edgePoints.push_back(single->points[3][3]);
	}
	for (size_t j = 0; j < horizontalSlicesCount; j++) // bottom edge
	{
		auto single = singleSurfaces[(verticalSlicesCount-1) * horizontalSlicesCount +j];
		edgePoints.push_back(single->points[3][2]);
		edgePoints.push_back(single->points[3][1]);
		edgePoints.push_back(single->points[3][0]);
	}
	for (size_t i = 0; i < verticalSlicesCount; i++) // left edge
	{
		auto single = singleSurfaces[i * horizontalSlicesCount];
		edgePoints.push_back(single->points[2][0]);
		edgePoints.push_back(single->points[1][0]);
		edgePoints.push_back(single->points[0][0]);
	}



	return edgePoints;
}

void BezierSurfaceC0::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<BezierSurfaceC0>());
}

std::vector<std::shared_ptr<IModel>> BezierSurfaceC0::GetContainingModels()
{
	auto models = std::vector<std::shared_ptr<IModel>>();
	for (auto single : singleSurfaces)
	{
		auto singleModels = single->GetContainingModels();
		for (auto model : singleModels)
		{
			models.push_back(model);
		}
	}
	return models;
}