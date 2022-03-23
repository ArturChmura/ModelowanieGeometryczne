#pragma once
#include <DirectXMath.h>
#include <string>
#include "ShaderInfo.h"
#include <memory>
#include "Camera.h"
#include "dxDevice.h"

class IModel
{
public:
	IModel();

	void ChangeName(std::string name);
	std::string name;
	int id;

	virtual void SetScale(float x, float y, float z) = 0;
	virtual DirectX::XMFLOAT3 GetScale() = 0;

	virtual void SetTranslation(float x, float y, float z) = 0;
	virtual void Translate(float x, float y, float z) = 0;
	virtual DirectX::XMFLOAT3 GetTranslation() = 0;

	virtual void SetRotation(float x, float y, float z) = 0;
	virtual DirectX::XMFLOAT3 GetRotation() = 0;

	virtual void ScaleFromPoint(DirectX::XMFLOAT3 point, DirectX::XMFLOAT3 scale) = 0;
	virtual void RotateFromPoint(DirectX::XMFLOAT3 globalPoint, DirectX::XMFLOAT3 ratation) = 0;

	virtual void Draw(std::shared_ptr<Camera> camera) = 0;

	virtual void RenderGUI();

	ShaderInfo shaderInfo;
protected:
};