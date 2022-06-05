#include "GregoryPatch.h"

GregoryPatch::GregoryPatch(std::vector<std::array<std::shared_ptr<Point>, 4>> lines, std::string nama)
: IUnmovableModel(nama)
{
	this->lines = lines;
}

void GregoryPatch::Draw(std::shared_ptr<Camera> camera)
{
}

void GregoryPatch::Accept(AbstractModelVisitor& visitor)
{
}

void GregoryPatch::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
}
