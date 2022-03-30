#pragma once

#include "IModel.h"
#include "Point.h"
#include "ShaderInfoSingleColorVs.h"

class BezierCurve : public IModel
{
public:
	BezierCurve(std::vector<Point*> points);
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
	void OnRemovedFromScene() override;
	virtual void Draw(std::shared_ptr<Camera> camera) override;
	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
	virtual void RenderGUI() override;
 
	std::vector<Point*> points;
	int indicesCount;
	int verticesCount;
	float DeCasteljeu(std::vector<float> coefficients, float t);
	void UpdateVertices();
	void AddPoint(Point* point); 
	void RemovePoint(int pointId);
	std::shared_ptr<ShaderInfoSingleColorVs> shaderInfoSingleColorVs;
	MeshInfo meshInfo;
	std::vector<int> selectedIndexes;
	bool isAddingMode = false;
	void SetDrawPolygonChain(bool draw = true);
	void UpdateSlicesCount(std::shared_ptr<Camera> camera);

	std::vector<int> currentSlices;
	std::vector<int> desiredSlices; 
	void ResetDrawing();
private:
	bool drawPolygonChain = false;
	bool resetDrawing = true;

};