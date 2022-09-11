#include "IBezierSurface.h"
#include "imgui.h"
using namespace DirectX::SimpleMath;

IBezierSurface::IBezierSurface(int horizontalCount, int verticalCount, bool isWrapped, std::string name)
	:IUnmovableModel(name)
{
	this->horizontalSlicesCount = horizontalCount;
	this->verticalSlicesCount = verticalCount;
	this->isWrapped = isWrapped;
}

void IBezierSurface::Draw(std::shared_ptr<Camera> camera)
{
	auto singleSurfaces = GetSingleSurfaces();
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

void IBezierSurface::RenderGUI()
{
	IModel::RenderGUI();
	auto singleSurfaces = GetSingleSurfaces();
	static int selectedIndex = -1;
	if (ImGui::BeginListBox("##ObjectsListBox", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < singleSurfaces.size(); i++)
		{
			auto model = singleSurfaces[i];
			const bool is_selected = i == selectedIndex;

			if (ImGui::Selectable((model->name + "##" + std::to_string(model->id)).c_str(), is_selected))
			{
				if (is_selected)
				{
					selectedIndex = -1;
					singleSurfaces[i]->ChangeColor(selectedColor);
				}
				else
				{
					if (selectedIndex >= 0)
					{
						singleSurfaces[selectedIndex]->ChangeColor(selectedColor);
					}
					selectedIndex = i;
					singleSurfaces[i]->ChangeColor(Vector3(1, 0, 0));
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
		singleSurfaces[selectedIndex]->RenderGUI();
	}
}


void IBezierSurface::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	auto singleSurfaces = GetSingleSurfaces();
	for (auto single : singleSurfaces)
	{
		single->ChangeColor(color);
	}
}

bool IBezierSurface::IsWrappedU()
{
	return false;
}

bool IBezierSurface::IsWrappedV()
{
	return isWrapped;
}

DirectX::SimpleMath::Vector3 IBezierSurface::GetValue(float u, float v)
{
	auto singleSurfaceParameter = GetSingleSurfaceParameter(u, v);
	return singleSurfaceParameter.singleSurface->GetValue(singleSurfaceParameter.u, singleSurfaceParameter.v);

}

DirectX::SimpleMath::Vector3 IBezierSurface::GetUDerivativeValue(float u, float v)
{
	auto singleSurfaceParameter = GetSingleSurfaceParameter(u, v);
	auto singleDeriviative =  singleSurfaceParameter.singleSurface->GetUDerivativeValue(singleSurfaceParameter.u, singleSurfaceParameter.v);
	return singleDeriviative * horizontalSlicesCount;
}

DirectX::SimpleMath::Vector3 IBezierSurface::GetVDerivativeValue(float u, float v)
{
	auto singleSurfaceParameter = GetSingleSurfaceParameter(u, v);
	auto singleDeriviative = singleSurfaceParameter.singleSurface->GetVDerivativeValue(singleSurfaceParameter.u, singleSurfaceParameter.v);
	return singleDeriviative * verticalSlicesCount;
}

bool IBezierSurface::IsUWrapped()
{
	return isWrapped;
}

bool IBezierSurface::IsVWrapped()
{
	return false;
}

SingleSurfaceParameter IBezierSurface::GetSingleSurfaceParameter(float u, float v)
{

	int horizontalSlice = ((int)(u * horizontalSlicesCount)) % horizontalSlicesCount;
	int verticalSlice = ((int)(v * verticalSlicesCount)) % verticalSlicesCount;

	float du = 1.0 / horizontalSlicesCount;
	float dv = 1.0 / verticalSlicesCount;

	auto singleSurfaces = GetSingleSurfaces();
	auto surface = singleSurfaces[verticalSlice * horizontalSlicesCount + horizontalSlice];


	SingleSurfaceParameter singleSurfaceParameter;
	singleSurfaceParameter.u = (u - horizontalSlice * du) * horizontalSlicesCount;
	singleSurfaceParameter.v = (v - verticalSlice * dv) * verticalSlicesCount;
	singleSurfaceParameter.singleSurface = surface;
	return singleSurfaceParameter;
}



std::vector<std::shared_ptr<IModel>> IBezierSurface::GetContainingModels()
{
	auto singleSurfaces = GetSingleSurfaces();
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