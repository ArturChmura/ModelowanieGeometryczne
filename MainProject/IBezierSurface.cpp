#include "IBezierSurface.h"
#include "imgui.h"
#include "ShadersManager.h"
#include "Helpers.h"
#include <algorithm>
using namespace DirectX::SimpleMath;

IBezierSurface::IBezierSurface(int horizontalCount, int verticalCount, bool isWrapped, std::string name)
	:IUnmovableModel(name)
{
	this->horizontalSlicesCount = horizontalCount;
	this->verticalSlicesCount = verticalCount;
	this->isWrapped = isWrapped;

	D3D11_TEXTURE2D_DESC textureDescription;
	ZeroMemory(&textureDescription, sizeof(textureDescription));
	textureDescription.Width = textureSize;
	textureDescription.Height = textureSize;
	textureDescription.SampleDesc.Count = 1;
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDescription.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	textureDescription.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;


	filterTexture = DxDevice::instance->CreateTexture(textureDescription);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	ID3D11ShaderResourceView* viewPtr;
	DxDevice::instance->operator->()->CreateShaderResourceView(filterTexture.get(), &srvDesc, &viewPtr);
	filterTextureView = mini::dx_ptr<ID3D11ShaderResourceView>(viewPtr);

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

DirectX::SimpleMath::Vector3 IBezierSurface::GetValue(double u, double v)
{
	auto singleSurfaceParameter = GetSingleSurfaceParameter(u, v);
	return singleSurfaceParameter.singleSurface->GetValue(singleSurfaceParameter.u, singleSurfaceParameter.v);

}

DirectX::SimpleMath::Vector3 IBezierSurface::GetUDerivativeValue(double u, double v)
{
	auto singleSurfaceParameter = GetSingleSurfaceParameter(u, v);
	auto singleDeriviative =  singleSurfaceParameter.singleSurface->GetUDerivativeValue(singleSurfaceParameter.u, singleSurfaceParameter.v);
	return singleDeriviative * horizontalSlicesCount;
}

DirectX::SimpleMath::Vector3 IBezierSurface::GetVDerivativeValue(double u, double v)
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

SingleSurfaceParameter IBezierSurface::GetSingleSurfaceParameter(double u, double v)
{
	u = GetInRange(u, 0.0, 1.0);
	v = GetInRange(v, 0.0, 1.0);
	int horizontalSlice = (int)std::floor(u * horizontalSlicesCount);
	int verticalSlice = (int)std::floor(v * verticalSlicesCount);
	horizontalSlice = std::clamp(horizontalSlice, 0, horizontalSlicesCount - 1);
	verticalSlice = std::clamp(verticalSlice, 0, verticalSlicesCount - 1);

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

void IBezierSurface::OnFilterUpdate()
{
	D3D11_MAPPED_SUBRESOURCE texmap;
	ZeroMemory(&texmap, sizeof(D3D11_MAPPED_SUBRESOURCE));
	DxDevice::instance->context()->Map(filterTexture.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &texmap);
	BYTE* bytes = (BYTE*)texmap.pData;
	auto textureSizeBytes = textureSize * textureSize * 4 * sizeof(BYTE);
	BYTE* pTexels = (BYTE*)malloc(textureSizeBytes);

	for (int i = 0; i < textureSize; i++) {
		int rowStart = i * textureSize;
		for (int j = 0; j < textureSize; j++)
		{
			float u = i / (float)textureSize;
			float v = j / (float)textureSize;
			bool visible = IsVisible(u, v);
			BYTE color = visible ? (BYTE)255 : (BYTE)0;
			pTexels[4 * (rowStart + j)] = color;
			pTexels[4 * (rowStart + j) + 1] = color;
			pTexels[4 * (rowStart + j) + 2] = color;
			pTexels[4 * (rowStart + j) + 3] = (BYTE)255;

		}
	}
	memcpy(bytes, (void*)pTexels, textureSizeBytes);

	DxDevice::instance->context()->Unmap(filterTexture.get(), 0);
	free((void*)pTexels);

	auto singleSurfaces = GetSingleSurfaces();
	for (auto singlesurface : singleSurfaces)
	{
		singlesurface->SetFilterTexture(filterTextureView.get());
	}
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