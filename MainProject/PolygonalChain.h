#pragma once
#include <vector>
#include <memory>
#include "Camera.h"
#include "Point.h"
#include <d3d11.h>
#include "SimpleMath.h"

class PolygonalChain
{
public:
	PolygonalChain(std::vector<DirectX::SimpleMath::Vector3> points);
	void Draw(std::shared_ptr<Camera> camera);



private:
	MeshInfo meshInfo;
	int indexCount;
};