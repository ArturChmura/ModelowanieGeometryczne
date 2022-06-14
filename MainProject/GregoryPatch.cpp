#include "GregoryPatch.h"

GregoryPatch::GregoryPatch(std::vector<std::shared_ptr<SingleGregoryPatch>> singleSurfaces, std::string name)
	:IUnmovableModel(name)
{
	this->singleSurfaces = singleSurfaces;
}

void GregoryPatch::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<GregoryPatch>());
}

void GregoryPatch::Draw(std::shared_ptr<Camera> camera)
{
	for (int i = 0; i < singleSurfaces.size(); i++)
	{
		singleSurfaces[i]->Draw(camera);
	}
}

void GregoryPatch::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	for (auto single : singleSurfaces)
	{
		single->ChangeColor(color);
	}
}


