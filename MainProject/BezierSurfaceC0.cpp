#include "BezierSurfaceC0.h"
#include "imgui.h"
#include<algorithm>
#include<iterator>

using namespace DirectX::SimpleMath;


BezierSurfaceC0::BezierSurfaceC0(std::vector<std::shared_ptr<SingleBezierSurfaceC0>> singleSurfaces, int horizontalCount, int verticalCount, bool isWrapped, std::string name)
	:IBezierSurface(horizontalCount,  verticalCount, isWrapped,name)
{

	this->singleSurfaces = singleSurfaces;
}


void BezierSurfaceC0::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<BezierSurfaceC0>());
}

std::vector<std::shared_ptr<ISingleBezierSurface>> BezierSurfaceC0::GetSingleSurfaces()
{
	std::vector<std::shared_ptr<ISingleBezierSurface>> single(this->singleSurfaces.size());
	std::copy(this->singleSurfaces.begin(), this->singleSurfaces.end(), single.begin());
	return single;
}
