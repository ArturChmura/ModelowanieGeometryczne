#pragma once
#include "IModel.h"
#include <map>
#include "Coursor3d.h"
class CompositeModel : public IModel
{
public:
	CompositeModel();
	std::map<int,std::shared_ptr<IModel>> modelsMap;
	void AddModel(std::shared_ptr<IModel> model);
	void RemoveModel(std::shared_ptr<IModel> model);

	void UpdateCentroidPosition();


	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Vector3 rotation;
	DirectX::SimpleMath::Vector3 centroidPosition;
	std::shared_ptr<Coursor3d> centroidCoursor;

	DirectX::XMFLOAT3 imguiScale;
	DirectX::XMFLOAT3 imguiTranslation;
	// Inherited via IModel
	virtual void SetScale(float x, float y, float z) override;

	virtual DirectX::SimpleMath::Vector3 GetScale() override;


	virtual void SetTranslation(float x, float y, float z) override;


	virtual void Translate(float x, float y, float z) override;


	virtual DirectX::SimpleMath::Vector3 GetTranslation() override;


	virtual void SetRotation(float pitch, float yaw, float roll) override;


	virtual DirectX::SimpleMath::Vector3 GetRotation() override;


	virtual void ScaleFromPoint(DirectX::XMFLOAT3 point, DirectX::XMFLOAT3 scale) override;


	virtual void RotateFromPoint(DirectX::XMFLOAT3 globalPoint, DirectX::XMFLOAT3 ratation) override;


	virtual void Draw(std::shared_ptr<Camera> camera) override;

	virtual void RenderGUI() override;


	// Inherited via IModel
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;


};