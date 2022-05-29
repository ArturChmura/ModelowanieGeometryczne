#pragma once
#include "VertexModel.h"
#include "DirectXMath.h"
#include <d3d11.h>
#include "SimpleMath.h"
#include <functional>
#include "Callback.h"

class Point : public IModel
{
public:
	Point(DirectX::SimpleMath::Vector3 location);


	// Inherited via IModel
	virtual void SetScale(float x, float y, float z) override;
	virtual void Scale(float x, float y, float z)  override;
	virtual DirectX::SimpleMath::Vector3 GetScale() override;
	virtual void SetTranslation(float x, float y, float z) override;
	virtual void Translate(float x, float y, float z) override;
	virtual DirectX::SimpleMath::Vector4 GetTranslation() override;
	virtual void SetRotation(float x, float y, float z) override;
	virtual void Rotate(float x, float y, float z)  override;
	virtual DirectX::SimpleMath::Vector3 GetRotation() override;
	virtual void ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale) override;
	virtual void RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation) override;
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	DirectX::SimpleMath::Vector4 translation;
	MeshInfo meshInfo;
	inline static std::vector<std::function<void(std::shared_ptr<Point>)>> onSelectCallback;
	virtual void OnSelect() override;
	inline static std::vector<std::function<void(std::shared_ptr<Point>)>> onAddedToSceneCallback;
	virtual void OnAddedToScene() override;
	Callback<void(std::shared_ptr<Point>)> onRemovedFromSceneCallback;
	void OnRemovedFromScene() override;

	std::vector<std::tuple<int, std::function<void(std::shared_ptr<Point>)>>> onModelChangeCallback;
private:
	DirectX::SimpleMath::Matrix GetModelMatrix();
	int verticesCount;
	int indicesCount;

	// Inherited via IModel
	virtual void RenderGUI() override;

	// Inherited via IModel
	void OnModelChange();

	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;


	// Inherited via IModel
	virtual void Accept(SerializationVisitor& visitor) override;


};