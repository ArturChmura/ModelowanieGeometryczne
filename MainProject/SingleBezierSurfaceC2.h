#pragma once
#include "IModel.h"
#include "Point.h"
#include <array>
#include "PolygonalChain.h"

class SingleBezierSurfaceC2 : public IModel
{
public:
	SingleBezierSurfaceC2(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices);
	void SetDrawPolygonChain(bool drawPolygonChain);

	std::array<std::array<std::shared_ptr<Point>, 4>, 4> points;
	int horizontalSlices;
	int verticalSlices;
private:
	MeshInfo meshInfo;
	bool resetDrawing = true;
	bool drawPolygonChain = false;
	std::vector<std::shared_ptr<PolygonalChain>> polygonalChains;

public:
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	void UpdateVertices();

	void OnRemovedFromScene();

	void RenderGUI() override;
	

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



	// Inherited via IModel
	virtual void Accept(SerializationVisitor& visitor) override;


};