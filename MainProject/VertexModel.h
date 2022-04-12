#pragma once
#include "IModel.h"
#include <memory>
#include "MeshInfo.h"
#include "ShaderInfoSingleColorVs.h"

class VertexModel :public IModel
{
public:
	VertexModel(std::string name = "VertexModel");
	size_t GetVerticesCount() { return verticesCount; };
	size_t GetIndicesCount() { return indicesCount; };

	MeshInfo meshInfo;
	DirectX::SimpleMath::Vector3 GetColor();
	void SetColor(DirectX::XMFLOAT3 color);

	void SetScale(float x, float y, float z) override;
	virtual void Scale(float x, float y, float z)  override;
	DirectX::SimpleMath::Vector3 GetScale()  override;

	 void SetTranslation(float x, float y, float z)  override;
	 void Translate(float x, float y, float z)  override;
	 DirectX::SimpleMath::Vector4 GetTranslation()  override;

	 void SetRotation(float pitch, float yaw, float roll)  override;
	 virtual void Rotate(float x, float y, float z)  override;
	 DirectX::SimpleMath::Vector3 GetRotation()  override;

	 void ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale)  override;
	 void RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation)  override;

	 void Draw(std::shared_ptr<Camera> camera)  override;
	 virtual void RenderGUI() override;
protected:
	void UpdateModelMatrix();
	DirectX::SimpleMath::Matrix GetRotationMatrix();
	DirectX::SimpleMath::Matrix modelMatrix;
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Vector4 translation;
	DirectX::SimpleMath::Vector3 rotation;
	int verticesCount;
	int indicesCount;

	// Inherited via IModel


	// Inherited via IModel
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
	std::shared_ptr<ShaderInfoSingleColorVs> shaderInfoSingleColorVs;


};