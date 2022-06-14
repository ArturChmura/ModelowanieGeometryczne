#pragma once
#include "IUnmovableModel.h"
#include <array>
#include "SingleGregoryPatch.h"
#include "PatchSide.h"

class GregoryPatch : public IUnmovableModel
{
public:
	GregoryPatch(std::vector<std::shared_ptr<SingleGregoryPatch>> singleSurfaces, std::string name = "Gregory Patch");

	std::vector<std::shared_ptr<SingleGregoryPatch>> singleSurfaces;
	virtual void Accept(AbstractModelVisitor& visitor) override;


	int horizontalSlicesCount;
	int verticalSlicesCount;
	bool drawPolygonChain = false;


	virtual void Draw(std::shared_ptr<Camera> camera) override;

	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;

};