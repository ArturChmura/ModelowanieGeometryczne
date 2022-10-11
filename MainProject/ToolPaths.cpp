#include "ToolPaths.h"
#include "SimpleMath.h"
#include "PolygonalChain.h"

using namespace DirectX::SimpleMath;

ToolPaths::ToolPaths(std::vector<std::tuple<double, double, double>> points)
{
	this->points = points;
}

void ToolPaths::Draw(std::shared_ptr<Camera> camera)
{
	auto vectors = std::vector<Vector3>();
	for (auto [x, y, z] : points)
	{
		vectors.push_back(Vector3(x, y, z));
	}
	
	PolygonalChain::Draw(camera, vectors);
}
