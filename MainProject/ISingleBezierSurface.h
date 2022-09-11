#pragma once
#include "IUnmovableModel.h"
#include "Point.h"
#include <array>
#include "PolygonalChain.h"
#include "IParameterized.h"

class ISingleBezierSurface : public IUnmovableModel, public IParameterized
{

public:
	ISingleBezierSurface(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices, std::string name = "ISingleBezierSurface");
	std::array<std::array<std::shared_ptr<Point>, 4>, 4> points;
	int horizontalSlices;
	int verticalSlices;

	void DrawPolygonChain(std::shared_ptr<Camera> camera);
	void UpdateVertices();
	void OnRemovedFromScene();
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
	void RenderGUI() override;
	void SetDrawPolygonChain(bool drawPolygonChain);
	virtual std::vector<std::shared_ptr<IModel>> GetContainingModels() override;
	// Inherited via IParameterized
	
protected:
	MeshInfo meshInfo;
	bool resetDrawing = true;
	bool drawPolygonChain = false;




};