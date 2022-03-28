#pragma once
#include "VertexModel.h"
#include "DirectXMath.h"
#include "ShaderInfoSingleColorVs.h"
#include "SimpleMath.h"
#include <functional>

class Point : public IModel
{
public:
	Point(DirectX::SimpleMath::Vector4 translation);


	// Inherited via IModel
	virtual void SetScale(float x, float y, float z) override;
	virtual DirectX::SimpleMath::Vector3 GetScale() override;
	virtual void SetTranslation(float x, float y, float z) override;
	virtual void Translate(float x, float y, float z) override;
	virtual DirectX::SimpleMath::Vector4 GetTranslation() override;
	virtual void SetRotation(float x, float y, float z) override;
	virtual DirectX::SimpleMath::Vector3 GetRotation() override;
	virtual void ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale) override;
	virtual void RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation) override;
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	DirectX::SimpleMath::Vector4 translation;
	MeshInfo meshInfo;

	static std::vector<std::function<void(Point*)>> onSelectCallback;
	virtual void OnSelect() override;
	static std::vector<std::function<void(Point*)>> onAddedToSceneCallback;
	virtual void OnAddedToScene() override;

	std::vector<std::function<void()>> onModelChangeCallback;
private:
	DirectX::SimpleMath::Matrix GetModelMatrix();
	int verticesCount;
	int indicesCount;

	// Inherited via IModel
	virtual void RenderGUI() override;

	// Inherited via IModel
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
	void OnModelChange();

	std::shared_ptr<ShaderInfoSingleColorVs> shaderInfoSingleColorVs;

};