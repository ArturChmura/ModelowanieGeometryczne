#include "BezierSurfaceC2.h"
#include<algorithm> 
#include<iterator> 

using namespace DirectX::SimpleMath;

BezierSurfaceC2::BezierSurfaceC2(std::vector<std::shared_ptr<SingleBezierSurfaceC2>> singleSurfaces, int horizontalCount, int verticalCount, bool isWrapped, std::string name)
	:IBezierSurface(horizontalCount, verticalCount, isWrapped, name)
{

	this->singleSurfaces = singleSurfaces;
}


void BezierSurfaceC2::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<BezierSurfaceC2>());
}

std::vector<std::shared_ptr<ISingleBezierSurface>> BezierSurfaceC2::GetSingleSurfaces()
{
	std::vector<std::shared_ptr<ISingleBezierSurface>> single(this->singleSurfaces.size());
	std::copy(this->singleSurfaces.begin(), this->singleSurfaces.end(), single.begin());
	return single;
}
