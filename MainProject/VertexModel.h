#pragma once
#include "IModel.h"
#include <memory>
#include "MeshInfo.h"

class VertexModel :public IModel
{
public:
	VertexModel();
	size_t GetVerticesCount() { return verticesCount; };
	size_t GetIndicesCount() { return indicesCount; };

	MeshInfo meshInfo;
	DirectX::XMFLOAT3 GetColor();
	void SetColor(DirectX::XMFLOAT3 color);

	void SetScale(float x, float y, float z) override;
	 DirectX::XMFLOAT3 GetScale()  override;

	 void SetTranslation(float x, float y, float z)  override;
	 void Translate(float x, float y, float z)  override;
	 DirectX::XMFLOAT3 GetTranslation()  override;

	 void SetRotation(float x, float y, float z)  override;
	 DirectX::XMFLOAT3 GetRotation()  override;

	 void ScaleFromPoint(DirectX::XMFLOAT3 point, DirectX::XMFLOAT3 scale)  override;
	 void RotateFromPoint(DirectX::XMFLOAT3 globalPoint, DirectX::XMFLOAT3 ratation)  override;

	 void Draw(std::shared_ptr<Camera> camera)  override;
	 virtual void RenderGUI() override;
protected:
	void UpdateModelMatrix();
	DirectX::XMMATRIX GetRotationMatrix();
	DirectX::XMFLOAT4X4 modelMatrix;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 rotationEuler;
	int verticesCount;
	int indicesCount;

	// Inherited via IModel


	// Inherited via IModel
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;


};