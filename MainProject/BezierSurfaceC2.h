#pragma once

#include "IModel.h"
#include "Point.h"
#include "PolygonalChain.h"
#include "SingleBezierSurfaceC2.h"
#include "IUnmovableModel.h"

class BezierSurfaceC2 : public IUnmovableModel
{
public:

	
	BezierSurfaceC2(std::vector<std::shared_ptr<SingleBezierSurfaceC2>> singleSurfaces,int horizontalCount, int verticalCount, std::string name = "Bezier Surface C2");


	virtual void Draw(std::shared_ptr<Camera> camera) override;
	virtual void RenderGUI() override;
	virtual std::vector<std::shared_ptr<IModel>> GetContainingModels() override;

	std::vector<std::shared_ptr<SingleBezierSurfaceC2>> singleSurfaces;
	int horizontalSlicesCount;
	int verticalSlicesCount;
private:
	bool drawPolygonChain = false;

	// Inherited via IModel
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;



	// Inherited via IModel
	virtual void Accept(AbstractModelVisitor& visitor) override;


};