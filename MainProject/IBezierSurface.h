#pragma once


#include "PolygonalChain.h"
#include "IUnmovableModel.h"
#include "ISingleBezierSurface.h"


class IBezierSurface : public IUnmovableModel
{
public:
	IBezierSurface(int horizontalCount, int verticalCount, bool isWrapped, std::string name = "IBezierSurface");
	virtual void Draw(std::shared_ptr<Camera> camera) override;
	virtual void RenderGUI() override;
	virtual std::vector<std::shared_ptr<IModel>> GetContainingModels() override;
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
	virtual std::vector<std::shared_ptr<ISingleBezierSurface>> GetSingleSurfaces() = 0;
	int horizontalSlicesCount;
	int verticalSlicesCount;
	bool IsWrappedU();
	bool IsWrappedV();
private:
	bool drawPolygonChain = false;
	bool isWrapped;
};
