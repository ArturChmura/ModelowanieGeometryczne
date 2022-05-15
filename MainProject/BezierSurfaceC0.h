#pragma once

#include "IModel.h"
#include "Point.h"
#include "SingleBezierSurfaceC0.h"
#include "PolygonalChain.h"

class BezierSurfaceC0 : public IModel
{
public:
	BezierSurfaceC0(
		int horizontalSlicesCount, 
		int verticalSlicesCount,
		float x,
		float y,
		bool cylinder, 
		DirectX::SimpleMath::Vector3 center
	);


	virtual void Draw(std::shared_ptr<Camera> camera) override;
	virtual void RenderGUI() override;
	virtual std::shared_ptr<IModel> SelectFromScreenCoords(float x, float y, DirectX::SimpleMath::Matrix VP) override;
	std::vector<std::vector<std::shared_ptr<Point>>> GetPoints();
	// Inherited via IModel
	virtual void SetScale(float x, float y, float z) override;


	virtual void Scale(float x, float y, float z) override;


	virtual DirectX::SimpleMath::Vector3 GetScale() override;


	virtual void ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale) override;


	virtual void SetTranslation(float x, float y, float z) override;


	virtual void Translate(float x, float y, float z) override;


	virtual DirectX::SimpleMath::Vector4 GetTranslation() override;


	virtual void SetRotation(float x, float y, float z) override;


	virtual void Rotate(float x, float y, float z) override;


	virtual DirectX::SimpleMath::Vector3 GetRotation() override;

	virtual void RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation) override;

	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;

private:
	std::vector<std::vector<std::shared_ptr<Point>>> points;
	std::vector<std::shared_ptr<SingleBezierSurfaceC0>> singleSurfaces;
	bool drawPolygonChain = false;
	int horizontalSlicesCount;
	int verticalSlicesCount;
};