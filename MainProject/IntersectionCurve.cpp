#include "IntersectionCurve.h"
#include "imgui.h"
#include "ArrayFilter.h"
#include "IFilter.h"

using namespace DirectX::SimpleMath;

IntersectionCurve::IntersectionCurve(
	std::vector<std::shared_ptr<Point>> points,
	std::shared_ptr<Scene> scene,
	std::shared_ptr<IParameterized> surfaces[2],
	std::vector<Pair<double>> UVs[2]
)
	:BezierCurveInterpolating(points)
{
	this->name = "Intersection Curve";
	this->scene = scene;
	this->UVs[0] = UVs[0];
	this->UVs[1] = UVs[1];
	for (int i = 0; i < 2; i++)
	{
		this->surfaces[i] = surfaces[i];
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

		filters[i] = std::make_shared<ArrayFilter>(UVs[i], surfaces[i]->IsUWrapped(), surfaces[i]->IsVWrapped());

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
		ImGui::Image((ImTextureID)(intptr_t)(filterTextureViews[i].get()), { 512,512 }, uv_min, uv_max, tint_col, border_col);
		if (ImGui::Button(("Swap##swap" + std::to_string(i)).c_str()))
		{
			filters[i]->Swap();
			UpdateFilter(i);
		}
		if (ImGui::Checkbox(("Filter surface##filterSurface" + std::to_string(i)).c_str(), &(filterSurface[i])))
		{
			if (filterSurface[i])
			{
				surfaces[i]->AddFilter(filters[i]);
			}
			else
			{
				surfaces[i]->RemoveFilter(filters[i]->FilterId);
			}
		}

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

	if (ImGui::Button("Transform to interpolating curve with distance"))
	{
		auto pointsCopy = this->points;
		scene->RemoveModel(this->id);
		auto interpolatingCurve = std::make_shared< BezierCurveInterpolating>(pointsCopy);
		auto surface = surfaces[transformDistanceObjectIndex];
		auto UV = UVs[transformDistanceObjectIndex];
		for (int i = 0; i < pointsCopy.size(); ++i)
		{
			auto point = pointsCopy[i];
			auto uv = UV[i];
			auto derU = surface->GetUDerivativeValue(uv.a, uv.b);
			auto derV = surface->GetVDerivativeValue(uv.a, uv.b);
			auto normal = derU.Cross(derV);

			if (projectOnPlane)
			{
				auto planeNormal = Vector3(planeNormalX, planeNormalY, planeNormalZ);
				planeNormal.Normalize();

				normal = normal - normal.Dot(planeNormal) * planeNormal;
			}
			normal.Normalize();

			auto position = Vector3(point->GetTranslation());
			position = position + normal * transformDistance;

			point->SetTranslation(position.x, position.y, position.z);
			scene->AddModel(point);
		}
		scene->AddModel(interpolatingCurve);
		scene->Select(interpolatingCurve);
	}
	ImGui::DragFloat("Distance", &transformDistance, 0.1f, 0.0f, 3.0f);
	const char* items[] = { "Surface 1", "Surface 2" };
	if (ImGui::Combo("Object for distance", &transformDistanceObjectIndex, items, IM_ARRAYSIZE(items)))
	{

	}
	if (ImGui::Checkbox("Cast on plane", &projectOnPlane))
	{

	}
	if (projectOnPlane)
	{
		ImGui::DragFloat("plane normal X", &planeNormalX, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("plane normal Y", &planeNormalY, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("plane normal Z", &planeNormalZ, 0.1f, 0.0f, 1.0f);
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

			BYTE color = filters[index]->IsFiltered(u, v);
			if (color != 0 && color != 255)
			{
				int e = 1;
			}
			pTexels[4 * (rowStart + j)] = color > 0 ? 255 : 0;
			pTexels[4 * (rowStart + j) + 1] = color > 200 ? 255 : 0;
			pTexels[4 * (rowStart + j) + 2] = color > 200 ? 255 : 0;
			pTexels[4 * (rowStart + j) + 3] = (BYTE)255;

		}
	}
	memcpy(bytes, (void*)pTexels, textureSizeBytes);

	DxDevice::instance->context()->Unmap(filterTextures[index].get(), 0);
	free((void*)pTexels);

	surfaces[index]->AddFilter(filters[index]);
}

bool IntersectionCurve::Serializable()
{
	return false;
}
