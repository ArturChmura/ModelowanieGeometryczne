#include "IntersectionCurve.h"
#include "imgui.h"

IntersectionCurve::IntersectionCurve(std::vector<std::shared_ptr<Point>> points, std::shared_ptr<Scene> scene)
	:BezierCurveInterpolating(points)
{
	this->name = "Intersection Curve";
	this->scene = scene;


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


	texture = DxDevice::instance->CreateTexture(textureDescription);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	DxDevice::instance->operator->()->CreateShaderResourceView(texture.get(), &srvDesc, &pFontTextureView);

	D3D11_MAPPED_SUBRESOURCE texmap;
	ZeroMemory(&texmap, sizeof(D3D11_MAPPED_SUBRESOURCE));
	DxDevice::instance->context()->Map(texture.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &texmap);
	BYTE* bytes = (BYTE*)texmap.pData;
	auto textureSizeBytes = textureSize * textureSize * 4 * sizeof(BYTE);
	BYTE* pTexels = (BYTE*)malloc(textureSizeBytes);

	for (int i = 0; i < textureSize; i++) {
		int rowStart = i * textureSize;
		for (int j = 0; j < textureSize; j++)
		{
			pTexels[4 * (rowStart + j)] = (BYTE)255;
			pTexels[4 * (rowStart + j) + 1] = (BYTE)182;
			pTexels[4 * (rowStart + j) + 2] = (BYTE)193;
			pTexels[4 * (rowStart + j) + 3] = (BYTE)255;

		}
	}
	memcpy(bytes, (void*)pTexels, textureSizeBytes);

	DxDevice::instance->context()->Unmap(texture.get(), 0);
	free((void*)pTexels);

	//texture->Release();
}


void IntersectionCurve::RenderGUI()
{
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

	ImGui::Image((ImTextureID)(intptr_t)pFontTextureView, { 500,500 }, uv_min, uv_max, tint_col, border_col);

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
