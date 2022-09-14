#include "IntersectionCurve.h"
#include "imgui.h"
#include "ArrayFilter.h"
#include "IFilter.h"

IntersectionCurve::IntersectionCurve(std::vector<std::shared_ptr<Point>> points, std::shared_ptr<Scene> scene, std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2)
	:BezierCurveInterpolating(points)
{
	this->name = "Intersection Curve";
	this->scene = scene;
	this->surfaces[0] = surface1;
	this->surfaces[1] = surface2;

	for (int i = 0; i < 2; i++)
	{
		D3D11_TEXTURE2D_DESC textureDescription;
		ZeroMemory(&textureDescription, sizeof(textureDescription));
		textureDescription.Width = TEXTURE_SIZE;
		textureDescription.Height = TEXTURE_SIZE;
		textureDescription.SampleDesc.Count = 1;
		textureDescription.MipLevels = 1;
		textureDescription.ArraySize = 1;
		textureDescription.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDescription.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		textureDescription.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;


		filterTextures[i] = DxDevice::instance->CreateTexture(textureDescription);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* viewPtr1;
		DxDevice::instance->operator->()->CreateShaderResourceView(filterTextures[i].get(), &srvDesc, &viewPtr1);
		filterTextureViews[i] = mini::dx_ptr<ID3D11ShaderResourceView>(viewPtr1);

		filters[i] = std::make_shared<ArrayFilter>(i);

		UpdateFilter(i);
	}
	
}


void IntersectionCurve::RenderGUI()
{
ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

	for (int i = 0; i < 2; i++)
	{
		ImGui::Image((ImTextureID)(intptr_t)(filterTextureViews[i].get()), {300,300}, uv_min, uv_max, tint_col, border_col);
	}


	if (ImGui::Button("Transform to interpolating curve"))
	{
		auto pointsCopy = this->points;
		scene->RemoveModel(this->id);
		auto interpolatingCurve = std::make_shared< BezierCurveInterpolating>(pointsCopy);
		for (auto point : pointsCopy)
		{
			scene->AddModel(point);
		}
		scene->AddModel(interpolatingCurve);
		scene->Select(interpolatingCurve);
	}
}

void IntersectionCurve::UpdateFilter(int index)
{
	D3D11_MAPPED_SUBRESOURCE texmap;
	ZeroMemory(&texmap, sizeof(D3D11_MAPPED_SUBRESOURCE));
	DxDevice::instance->context()->Map(filterTextures[index].get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &texmap);
	BYTE* bytes = (BYTE*)texmap.pData;
	auto textureSizeBytes = TEXTURE_SIZE * TEXTURE_SIZE * 4 * sizeof(BYTE);
	BYTE* pTexels = (BYTE*)malloc(textureSizeBytes);

	for (int i = 0; i < TEXTURE_SIZE; i++) {
		int rowStart = i * TEXTURE_SIZE;
		for (int j = 0; j < TEXTURE_SIZE; j++)
		{
			float u = i / (float)TEXTURE_SIZE;
			float v = j / (float)TEXTURE_SIZE;

			auto filtered = filters[index]->IsFiltered(u, v);
			BYTE color = filtered ? (BYTE)0 : (BYTE)255;
			pTexels[4 * (rowStart + j)] = color;
			pTexels[4 * (rowStart + j) + 1] = color;
			pTexels[4 * (rowStart + j) + 2] = color;
			pTexels[4 * (rowStart + j) + 3] = (BYTE)255;

		}
	}
	memcpy(bytes, (void*)pTexels, textureSizeBytes);

	DxDevice::instance->context()->Unmap(filterTextures[index].get(), 0);
	free((void*)pTexels);

	surfaces[index]->AddFilter(filters[index]);
}
