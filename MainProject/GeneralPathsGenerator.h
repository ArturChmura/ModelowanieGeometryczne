#pragma once
#include "IModel.h"
#include "BlockModel.h"
#include "StraightCurveInterpolating.h"
#include "Scene.h"
#include "IParameterized.h"

class GeneralPathsGenerator
{
public:
	std::shared_ptr< StraightCurveInterpolating>  GeneralPaths(std::vector<std::shared_ptr<IModel>> models);

	std::shared_ptr< StraightCurveInterpolating>  PlanePaths(std::vector<std::shared_ptr<IModel>> models);


	std::shared_ptr< StraightCurveInterpolating>  BorderPath(std::vector<std::shared_ptr<IModel>> models, std::shared_ptr<Scene> scene);

	void AddBeseIntersection(std::shared_ptr<BezierSurfaceC0> basePlane, std::shared_ptr<IParameterized> modelOffset, std::vector<DirectX::SimpleMath::Vector3>& positions, DirectX::SimpleMath::Vector3 cursorPosition);

	void AddRightCupBeseIntersection(std::shared_ptr<BezierSurfaceC0> basePlane, std::shared_ptr<IParameterized> modelOffset, std::vector<DirectX::SimpleMath::Vector3>& positions, DirectX::SimpleMath::Vector3 cursorPosition);

	void AddHandleBeseIntersection(std::shared_ptr<BezierSurfaceC0> basePlane, std::shared_ptr<IParameterized> handle, std::vector<DirectX::SimpleMath::Vector3>& positions, DirectX::SimpleMath::Vector3 cursorPosition);


private:
	int textureSize = 512;
	float baseSize = 15.0f;
	float drillRadiusP1 = 0.8f;
	float drillRadiusP2 = 0.6f;
	float drillRadiusP3 = 0.5f;
	float baseHeight = 1.6f;
	float pointsDistance = 0.005f;
	float precision = 0.001f;
	const DirectX::SimpleMath::Vector3 beginPosition = DirectX::SimpleMath::Vector3(0, 6.6f, 0);

	float GetMaxHeight(float* heightMap, int iCenter, int jCenter, int radiusInPixels);

	DirectX::SimpleMath::Vector2  GetPositionFromCoordinates(int row, int column);
	std::pair<int,int>  GetCoordinatesFromPosition(DirectX::SimpleMath::Vector2 position);
	float* GetHeightMapFromRenderingToDepthBuffer(std::vector<std::shared_ptr<IModel>> models);
	float* GetDiffusedHeightMap(float* heightMap);
	mini::dx_ptr<ID3D11Texture2D> CreateDepthTexture2D();
	mini::dx_ptr<ID3D11DepthStencilView> CreateDepthStencilView(ID3D11Texture2D* depthTexture2D);
	D3D11_VIEWPORT CreateViewport();
	std::shared_ptr<Camera> GetCamera();
	mini::dx_ptr<ID3D11Texture2D> CreateHeightMapTexture2D();


	int GetDrillRadiusInPixels(float drillRadius);

	std::vector<DirectX::SimpleMath::Vector3> RemoveSelfIntersections(std::vector<DirectX::SimpleMath::Vector3> positions);
	
};