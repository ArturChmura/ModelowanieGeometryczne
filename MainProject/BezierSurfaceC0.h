#pragma once

#include "IModel.h"
#include "Point.h"
#include "SingleBezierSurfaceC0.h"
#include "PolygonalChain.h"
#include "IUnmovableModel.h"

class BezierSurfaceC0 : public IUnmovableModel
{
public:

	BezierSurfaceC0(std::vector<std::shared_ptr<SingleBezierSurfaceC0>> singleSurfaces, std::string name = "Bezier Surface C0");


	virtual void Draw(std::shared_ptr<Camera> camera) override;
	virtual void RenderGUI() override;
	// Inherited via IModel
	virtual std::vector<std::shared_ptr<IModel>> GetContainingModels() override;
	
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;

	std::vector<std::shared_ptr<SingleBezierSurfaceC0>> singleSurfaces;
	int horizontalSlicesCount;
	int verticalSlicesCount;
	std::vector<std::shared_ptr<Point>> GetEdgePoints();
private:
	bool drawPolygonChain = false;

	// Inherited via IModel
	virtual void Accept(AbstractModelVisitor& visitor) override;

};