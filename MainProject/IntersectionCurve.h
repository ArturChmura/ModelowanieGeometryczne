#pragma once
#include "BezierCurveInterpolating.h"
#include "SimpleMath.h"
#include <vector>
#include "Scene.h"
class IntersectionCurve : public BezierCurveInterpolating
{
public:
	IntersectionCurve(std::vector<std::shared_ptr<Point>> points, std::shared_ptr<Scene> scene);
	virtual void RenderGUI() override;
	std::shared_ptr<Scene> scene;
	mini::dx_ptr<ID3D11Texture2D> texture;
	ID3D11ShaderResourceView* pFontTextureView;

	int textureSize = 1000;
};