#pragma once
#include "IModel.h"

class GeneralPathsGenerator
{
public:
	ID3D11ShaderResourceView* GeneratePaths(std::vector<std::shared_ptr<IModel>> models, ID3D11RenderTargetView* rtvPassed = nullptr);
};