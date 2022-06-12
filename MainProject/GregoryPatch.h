#pragma once
#include "IBezierSurface.h"
#include <array>
#include "SingleGregoryPatch.h"
#include "PatchSide.h"

class GregoryPatch : public IBezierSurface
{
public:
	GregoryPatch(std::vector<std::shared_ptr<SingleGregoryPatch>> singleSurfaces, std::string name = "Gregory Patch");

	std::vector<std::shared_ptr<SingleGregoryPatch>> singleSurfaces;
	virtual void Accept(AbstractModelVisitor& visitor) override;

	// Inherited via IBezierSurface
	virtual std::vector<std::shared_ptr<ISingleBezierSurface>> GetSingleSurfaces() override;


};