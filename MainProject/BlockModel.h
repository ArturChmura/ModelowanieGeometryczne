#pragma once
#include <memory>
#include <d3d11.h>
#include "dxptr.h"
#include "MeshInfo.h"
#include "Camera.h"
#include "SimpleMath.h"
#include "vsNormal.h"
#include "ICutter.h"

class BlockModel
{
public:
	BlockModel(float widthSize, float lengthSize, float heightSize, int gridWidthCount, int gridLengthCount);
	void DrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end);
	void Draw(std::shared_ptr<Camera> camera);
	void SetCutter(std::shared_ptr<ICutter> cutter);
	~BlockModel();

private:
	float* heightMap;

	float widthSize; 
	float lengthSize; 
	float heightSize; 

	int gridWidthCount; 
	int gridLengthCount;

	MeshInfo meshInfo;
	std::vector<std::tuple<int,int,float>> cutterOffsets;
	int verticesCount;
	int indicesCount;

	std::pair<int, int> GetCoordinatesFromPosition(DirectX::SimpleMath::Vector2 position);
	DirectX::SimpleMath::Vector2 GetPositionFromCoordinates(std::pair<int, int> coordinates);

	bool resetDrawing;

	void ResetMesh();
	std::vector<VSNormalIn> vertices;
};