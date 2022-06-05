#pragma once
#include "IUnmovableModel.h"
#include <array>
class GregoryPatch : public IUnmovableModel
{
public:
	GregoryPatch(std::vector<std::array<std::shared_ptr<Point>,4>> lines, std::string nama = "Gregory Patch");
	std::vector<std::array<std::shared_ptr<Point>, 4>> lines;
	// Inherited via IUnmovableModel
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	virtual void Accept(AbstractModelVisitor& visitor) override;


	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;


};