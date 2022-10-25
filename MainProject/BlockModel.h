#pragma once
#include <memory>
#include <d3d11.h>
#include "dxptr.h"
#include "MeshInfo.h"
#include "Camera.h"
#include "SimpleMath.h"
#include "vsNormal.h"
#include "ICutter.h"


#include <limits>

class BlockModel
{
public:

	struct DrawLineResult
	{
		bool anyChanges;
		float maxHeightChange;
		float minHeightCutted = FLT_MAX;
	};

	BlockModel(float widthSize, float lengthSize, float heightSize, int gridWidthCount, int gridLengthCount, float minHeight);
	void InitializeMesh();
	DrawLineResult DrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end);
	void Draw(std::shared_ptr<Camera> camera);
	void SetCutter(std::shared_ptr<ICutter> cutter);
	bool IsError(float minCuttedHeight, std::string& outErrorMessage);
	void SetMinHeight(float minHeight);
	~BlockModel();

private:
	float* heightMap;
	mini::dx_ptr<ID3D11Texture2D> heightMapTexture;
	mini::dx_ptr<ID3D11ShaderResourceView> heightMapTextureSRV;

	float widthSize; 
	float lengthSize; 
	float heightSize; 
	float minHeight;
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


	mini::dx_ptr<ID3D11ShaderResourceView> colorTexture;
};