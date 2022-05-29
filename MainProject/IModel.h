#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Camera.h"
#include "dxDevice.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"
#include "MultipleInheritableEnableSharedFromThis.h"
#include "Named.h"
#include "SerializationVisitor.h"

class IModel: public inheritable_enable_shared_from_this<IModel>, public Named
{
public:
	IModel(std::string name = "unnamed");


	virtual void SetScale(float x, float y, float z) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	virtual DirectX::SimpleMath::Vector3 GetScale() = 0;
	virtual void ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale) = 0;

	virtual void SetTranslation(float x, float y, float z) = 0;
	virtual void Translate(float x, float y, float z) = 0;
	virtual DirectX::SimpleMath::Vector4 GetTranslation() = 0;

	virtual void SetRotation(float x, float y, float z) = 0;
	virtual void Rotate(float x, float y, float z) = 0;
	virtual DirectX::SimpleMath::Vector3 GetRotation() = 0;
	virtual void RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation) = 0;


	virtual void Draw(std::shared_ptr<Camera> camera) = 0;

	virtual void RenderGUI();
	virtual void OnSelect();
	virtual void OnDeselect();
	virtual void ChangeDefaultColor(DirectX::SimpleMath::Vector3 color);
	virtual void OnAddedToScene();
	virtual void OnRemovedFromScene();
	void SetVisible(bool visible);
	bool GetVisible();

	virtual std::shared_ptr<IModel> SelectFromScreenCoords(float x, float y, DirectX::SimpleMath::Matrix VP);

	virtual void Accept(SerializationVisitor& visitor) = 0;

protected:
	bool visible = true;
	DirectX::SimpleMath::Vector3 defaultColor = {1,1,1};
	DirectX::SimpleMath::Vector3 selectedColor = { 1.0f, 0.6f, 0.0f };
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) = 0;
};