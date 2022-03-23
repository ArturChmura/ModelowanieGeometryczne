#pragma once
#include "VertexModel.h"
#include "DirectXMath.h"
class Point : public IModel
{
public:
	Point(DirectX::XMFLOAT3 translation);


	// Inherited via IModel
	virtual void SetScale(float x, float y, float z) override;


	virtual DirectX::XMFLOAT3 GetScale() override;


	virtual void SetTranslation(float x, float y, float z) override;


	virtual void Translate(float x, float y, float z) override;


	virtual DirectX::XMFLOAT3 GetTranslation() override;


	virtual void SetRotation(float x, float y, float z) override;


	virtual DirectX::SimpleMath::Quaternion GetRotation() override;



	virtual void ScaleFromPoint(DirectX::XMFLOAT3 point, DirectX::XMFLOAT3 scale) override;


	virtual void RotateFromPoint(DirectX::XMFLOAT3 globalPoint, DirectX::XMFLOAT3 ratation) override;


	virtual void Draw(std::shared_ptr<Camera> camera) override;

	DirectX::XMFLOAT3 translation;
	MeshInfo meshInfo;

private:
	DirectX::XMMATRIX GetModelMatrix();
	int verticesCount;
	int indicesCount;

	// Inherited via IModel
	virtual void RenderGUI() override;


	// Inherited via IModel
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;


};