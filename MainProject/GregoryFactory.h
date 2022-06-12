#pragma once
#include <memory>
#include "GregoryPatch.h"
#include "PatchSide.h"
#include "SimpleMath.h"
class GregoryFactory
{
public:
	static std::vector < std::shared_ptr< IModel>> CreateGregoryPatch(std::vector<std::shared_ptr<PatchSide>> patchesSides);

	static DirectX::SimpleMath::Vector3 DeCastilljeu(DirectX::SimpleMath::Vector3 coef[4], float t);
};