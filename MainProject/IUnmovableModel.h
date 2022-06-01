#pragma once
#include "IModel.h"

class IUnmovableModel : public IModel
{
public:
	IUnmovableModel(std::string name = "unnamed");
	virtual bool IsMovable() override;
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
};