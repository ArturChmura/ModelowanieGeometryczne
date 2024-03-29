#pragma once


#include "PolygonalChain.h"
#include "IUnmovableModel.h"
#include "ISingleBezierSurface.h"
#include "IParameterized.h"
#include "SingleSurfaceParameter.h"

class IBezierSurface : public IUnmovableModel, public IParameterized
{
public:
	IBezierSurface(int horizontalCount, int verticalCount, bool isUWrapped, bool isVWrapped, std::string name = "IBezierSurface");
	virtual void Draw(std::shared_ptr<Camera> camera) override;
	virtual void RenderGUI() override;
	virtual std::vector<std::shared_ptr<IModel>> GetContainingModels() override;
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
	virtual std::vector<std::shared_ptr<ISingleBezierSurface>> GetSingleSurfaces() = 0;
	int horizontalSlicesCount;
	int verticalSlicesCount;
	// Inherited via IParameterized
	virtual DirectX::SimpleMath::Vector3 GetValue(double u, double v) override;
	virtual bool IsUWrapped() override;
	virtual bool IsVWrapped() override;
private:
	bool drawPolygonChain = false;
	bool isUWrapped;
	bool isVWrapped;


	SingleSurfaceParameter GetSingleSurfaceParameter(double u, double v);

	// Inherited via IParameterized
	virtual void OnFilterUpdate() override;

	int textureSize = 1024;
	mini::dx_ptr<ID3D11Texture2D> filterTexture;
	mini::dx_ptr<ID3D11ShaderResourceView> filterTextureView;

	int selectedIndex = -1;
};
