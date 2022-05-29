#include "BezierSurfaceC2.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;

BezierSurfaceC2::BezierSurfaceC2(std::vector<std::shared_ptr<SingleBezierSurfaceC2>> singleSurfaces, std::string name)
	: IModel(name)
{
	this->singleSurfaces = singleSurfaces;
}

void BezierSurfaceC2::Draw(std::shared_ptr<Camera> camera)
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

void BezierSurfaceC2::RenderGUI()
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

std::vector<std::shared_ptr<IModel>> BezierSurfaceC2::GetContainingModels()
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


void BezierSurfaceC2::SetScale(float x, float y, float z)
{
}

void BezierSurfaceC2::Scale(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 BezierSurfaceC2::GetScale()
{
	return DirectX::SimpleMath::Vector3();
}

void BezierSurfaceC2::ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale)
{
}

void BezierSurfaceC2::SetTranslation(float x, float y, float z)
{
}

void BezierSurfaceC2::Translate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector4 BezierSurfaceC2::GetTranslation()
{
	return DirectX::SimpleMath::Vector4();
}

void BezierSurfaceC2::SetRotation(float x, float y, float z)
{
}

void BezierSurfaceC2::Rotate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 BezierSurfaceC2::GetRotation()
{
	return DirectX::SimpleMath::Vector3();
}

void BezierSurfaceC2::RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation)
{
}

void BezierSurfaceC2::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	for (auto single : singleSurfaces)
	{
		single->ChangeColor(color);
	}
}

void BezierSurfaceC2::Accept(SerializationVisitor& visitor)
{
	visitor.Accept(*this);
}