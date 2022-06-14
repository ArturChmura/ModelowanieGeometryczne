#pragma once
#include "IUnmovableModel.h"
#include <array>
#include "SingleGregoryPatch.h"
#include "PatchSide.h"

class GregoryPatch : public IUnmovableModel
{
public:
	GregoryPatch(std::vector<std::shared_ptr<PatchSide>> patchesSides, std::string name = "Gregory Patch");

	void UpdateVertices();
	std::vector<std::shared_ptr<PatchSide>> patchesSides;

	std::vector<std::shared_ptr<SingleGregoryPatch>> singleSurfaces;
	virtual void Accept(AbstractModelVisitor& visitor) override;


	int horizontalSlicesCount = 20;
	int verticalSlicesCount = 20;
	bool drawPolygonChain = false;

	bool resetDrawing = true;
	void ResetDrawing();

	virtual void Draw(std::shared_ptr<Camera> camera) override;

	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;

	virtual void RenderGUI() override;

};