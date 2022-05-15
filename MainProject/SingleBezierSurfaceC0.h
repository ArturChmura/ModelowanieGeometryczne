#pragma once
#include "IModel.h"
#include "Point.h"
#include <array>
#include "PolygonalChain.h"

class SingleBezierSurfaceC0 : public IModel
{
public:
	SingleBezierSurfaceC0(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points);
	void SetDrawPolygonChain(bool drawPolygonChain);

private:
	std::array<std::array<std::shared_ptr<Point>, 4>, 4> points;
	int horizontalSlices;
	int verticalSlices;
	MeshInfo meshInfo;
	bool resetDrawing = true;
	bool drawPolygonChain = false;
	std::vector<std::shared_ptr<PolygonalChain>> polygonalChains;

public:
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	void UpdateVertices();

	void OnRemovedFromScene();
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

	void RenderGUI() override;
};