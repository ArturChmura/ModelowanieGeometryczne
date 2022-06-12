#include "GregoryPatch.h"

GregoryPatch::GregoryPatch(std::vector<std::shared_ptr<SingleGregoryPatch>> singleSurfaces, std::string name)
	:IBezierSurface(5,5, false, name)
{
	this->singleSurfaces = singleSurfaces;
}

void GregoryPatch::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<GregoryPatch>());
}

std::vector<std::shared_ptr<ISingleBezierSurface>> GregoryPatch::GetSingleSurfaces()
{
	std::vector<std::shared_ptr<ISingleBezierSurface>> single(this->singleSurfaces.size());
	std::copy(this->singleSurfaces.begin(), this->singleSurfaces.end(), single.begin());
	return single;
}
