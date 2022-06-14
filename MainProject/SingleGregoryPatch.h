#pragma once

#include "IUnmovableModel.h"
#include "Point.h"
#include <array>
#include "PolygonalChain.h"
#include "SimpleMath.h"

class SingleGregoryPatch : public IUnmovableModel
{
public:
	SingleGregoryPatch(
		std::array<DirectX::SimpleMath::Vector3 ,4> p,
		std::array<DirectX::SimpleMath::Vector3 ,4> dU,
		std::array<DirectX::SimpleMath::Vector3 ,4> dV,
		std::array<DirectX::SimpleMath::Vector3 ,4> dUV,
		std::array<DirectX::SimpleMath::Vector3 ,4> dVU
	);

public:
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	// Inherited via IModel
	virtual void Accept(AbstractModelVisitor& visitor) override;
	// Inherited via IUnmovableModel
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
private:
	std::array<DirectX::SimpleMath::Vector3, 4> p;
	std::array<DirectX::SimpleMath::Vector3, 4> dU;
	std::array<DirectX::SimpleMath::Vector3, 4> dV;
	std::array<DirectX::SimpleMath::Vector3, 4> dUV;
	std::array<DirectX::SimpleMath::Vector3, 4> dVU;


	MeshInfo meshInfo;
	bool resetDrawing = true;
	bool drawPolygonChain = false;
	int horizontalSlices = 20;
	int verticalSlices = 20;


};