#include "BezierSurfaceC0.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;


BezierSurfaceC0::BezierSurfaceC0(std::vector<std::shared_ptr<SingleBezierSurfaceC0>> singleSurfaces, std::string name)
	:IUnmovableModel(name)
{
	this->singleSurfaces = singleSurfaces;
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

void BezierSurfaceC0::Accept(SerializationVisitor& visitor)
{
	visitor.Accept(*this);
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