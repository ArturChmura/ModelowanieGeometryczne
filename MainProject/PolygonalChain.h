#pragma once
#include <vector>
#include <memory>
#include "Camera.h"
#include "Point.h"
#include <d3d11.h>
#include "SimpleMath.h"
#include "dxptr.h"

class PolygonalChain
{
public:
	static void Draw(std::shared_ptr<Camera> camera, std::vector<DirectX::SimpleMath::Vector3> points);
private:
	inline static int bufferSize = 0;
	inline static mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	inline static mini::dx_ptr<ID3D11Buffer> indexBuffer;
};