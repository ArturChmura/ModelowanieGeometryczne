#pragma once
#include "BezierCurveInterpolating.h"
#include "SimpleMath.h"
#include <vector>
#include "Scene.h"
#include "ArrayFilter.h"
#include "Pair.h"

#define TEXTURE_SIZE 1024
class IntersectionCurve : public BezierCurveInterpolating
{
public:
	IntersectionCurve(std::vector<std::shared_ptr<Point>> points,
		std::shared_ptr<Scene> scene,
		std::shared_ptr<IParameterized> surfaces[2],
		std::vector<Pair<double>> UVs[2]
		);
	virtual void RenderGUI() override;
	std::shared_ptr<Scene> scene;

	bool filterSurface[2];
	std::shared_ptr<IParameterized> surfaces[2];
	std::shared_ptr<ArrayFilter> filters[2];
	mini::dx_ptr<ID3D11ShaderResourceView> filterTextureViews[2];
	mini::dx_ptr<ID3D11Texture2D> filterTextures[2];

	void UpdateFilter(int index);

	virtual bool Serializable() override;

private:
	std::vector<Pair<double>> UVs[2];
	float transformDistance = 0.5;
	bool projectOnPlane = false;
	float planeNormalX = 0.0f;
	float planeNormalY = 1.0f;
	float planeNormalZ = 0.0f;
	int transformDistanceObjectIndex = 0;
};