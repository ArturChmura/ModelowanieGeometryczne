#include "ToolPaths.h"
#include "SimpleMath.h"
#include "PolygonalChain.h"

using namespace DirectX::SimpleMath;

void ToolPaths::Draw(std::shared_ptr<Camera> camera)
{
	auto points = std::vector<Vector3>
	{
		{0, 0, 0}, { 0,0,1 }
	};
	PolygonalChain::Draw(camera, points);
}
