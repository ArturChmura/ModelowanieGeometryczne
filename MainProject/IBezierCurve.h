#pragma once
#include "IModel.h"
#include "Point.h"


class IBezierCurve : public IModel
{
public:
	IBezierCurve(std::vector<std::shared_ptr<Point>> points, std::string name = "IBezierCurve");
	std::vector<std::shared_ptr<Point>> points;
	float DeCasteljeu(std::vector<float> coefficients, float t);
	virtual void UpdateVertices() = 0;
	void AddPoint(std::shared_ptr<Point> point);
	void RemovePoint(int pointId);
	void SetDrawPolygonChain(bool draw = true);
	void ResetDrawing();
	void OnRemovedFromScene() override;

	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
	std::vector<int> selectedIndexes;
	virtual void RenderGUI() override;
	// Inherited via IModel
	virtual void SetScale(float x, float y, float z) override;
	virtual DirectX::SimpleMath::Vector3 GetScale() override;
	virtual void ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale) override;
	virtual void SetTranslation(float x, float y, float z) override;
	virtual void Translate(float x, float y, float z) override;
	virtual DirectX::SimpleMath::Vector4 GetTranslation() override;
	virtual void SetRotation(float pitch, float yaw, float roll) override;
	virtual DirectX::SimpleMath::Vector3 GetRotation() override;
	virtual void RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation) override;

	std::shared_ptr<ShaderInfoSingleColorVs> shaderInfoSingleColorVs;
	MeshInfo meshInfo;
protected:
	bool resetDrawing = true;
	bool drawPolygonChain = false;
	bool isAddingMode = false;
};