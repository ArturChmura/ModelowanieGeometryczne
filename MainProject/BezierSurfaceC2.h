#pragma once

#include "IModel.h"
#include "Point.h"
#include "PolygonalChain.h"
#include "SingleBezierSurfaceC2.h"

class BezierSurfaceC2 : public IModel
{
public:

	
	BezierSurfaceC2(std::vector<std::shared_ptr<SingleBezierSurfaceC2>> singleSurfaces, std::string name = "Bezier Surface C2");


	virtual void Draw(std::shared_ptr<Camera> camera) override;
	virtual void RenderGUI() override;
	virtual std::vector<std::shared_ptr<IModel>> GetContainingModels() override;

	std::vector<std::shared_ptr<SingleBezierSurfaceC2>> singleSurfaces;
private:
	bool drawPolygonChain = false;
	int horizontalSlicesCount;
	int verticalSlicesCount;

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