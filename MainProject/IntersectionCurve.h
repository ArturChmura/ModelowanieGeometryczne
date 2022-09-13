#pragma once
#include "BezierCurveInterpolating.h"
#include "SimpleMath.h"
#include <vector>
#include "Scene.h"
#include "IFilter.h"
#define TEXTURE_SIZE 1024
class IntersectionCurve : public BezierCurveInterpolating
{
public:
	IntersectionCurve(std::vector<std::shared_ptr<Point>> points, std::shared_ptr<Scene> scene, std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2);
	virtual void RenderGUI() override;
	std::shared_ptr<Scene> scene;


	std::shared_ptr<IParameterized> surfaces[2];
	std::shared_ptr<IFilter> filters[2];
	mini::dx_ptr<ID3D11ShaderResourceView> filterTextureViews[2];
	mini::dx_ptr<ID3D11Texture2D> filterTextures[2];

	void UpdateFilter(int index);
};