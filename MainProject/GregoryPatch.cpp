#include "GregoryPatch.h"
#include "imgui.h"
#include "BernstrinHelper.h";

using namespace DirectX::SimpleMath;
GregoryPatch::GregoryPatch(std::vector<std::shared_ptr<PatchSide>> patchesSides, std::string name)
	:IUnmovableModel(name)
{
	this->singleSurfaces = singleSurfaces;
	this->patchesSides = patchesSides;
	UpdateVertices();

	for (auto patch : patchesSides)
	{
		for (auto point : patch->firstLine)
		{
			point->onModelChangeCallback.Add(
				[this](std::shared_ptr<Point> point) { this->ResetDrawing(); },
				id
			);
		}
		for (auto point : patch->secondLine)
		{
			point->onModelChangeCallback.Add(
				[this](std::shared_ptr<Point> point) { this->ResetDrawing(); },
				id
			);
		}
	}
}

void GregoryPatch::UpdateVertices()
{
	this->resetDrawing = false;
	std::vector<Vector3> t1i(patchesSides.size());
	std::vector<Vector3> p3i(patchesSides.size());
	std::vector<Vector3> p2i(patchesSides.size());
	std::vector<Vector3> qi(patchesSides.size());
	for (int i = 0; i < patchesSides.size(); i++)
	{
		Vector3 coef1[4];
		Vector3 coef0[4];
		for (int j = 0; j < 4; j++)
		{
			coef1[j] = Vector3(patchesSides[i]->secondLine[j]->GetTranslation());
			coef0[j] = Vector3(patchesSides[i]->firstLine[j]->GetTranslation());
		}
		t1i[i] = BernstrinHelper::DeCasteljeu(coef1, 0.5f, 4);
		p3i[i] = BernstrinHelper::DeCasteljeu(coef0, 0.5f, 4);
		p2i[i] = p3i[i] + (p3i[i] - t1i[i]);

		qi[i] = (3 * p2i[i] - p3i[i]) / 2.0f;
	}

	Vector3 pAvg;
	for (int i = 0; i < patchesSides.size(); i++)
	{
		pAvg += qi[i];
	}
	pAvg /= patchesSides.size();

	std::vector<Vector3> p1i(patchesSides.size());
	for (int i = 0; i < patchesSides.size(); i++)
	{
		p1i[i] = (2 * qi[i] + pAvg) / 3.0f;
	}


	singleSurfaces.clear();
	for (int i = 0; i < patchesSides.size(); i++)
	{
		int patch1index = i;
		int patch2index = (i + 1) % patchesSides.size();
		auto patch1 = patchesSides[patch1index];
		auto patch2 = patchesSides[patch2index];
		Vector3 patch1firstLine[4]
		{
			Vector3(patch1->firstLine[0]->GetTranslation()),
			Vector3(patch1->firstLine[1]->GetTranslation()),
			Vector3(patch1->firstLine[2]->GetTranslation()),
			Vector3(patch1->firstLine[3]->GetTranslation())
		};

		Vector3 patch2firstLine[4]
		{
			Vector3(patch2->firstLine[0]->GetTranslation()),
			Vector3(patch2->firstLine[1]->GetTranslation()),
			Vector3(patch2->firstLine[2]->GetTranslation()),
			Vector3(patch2->firstLine[3]->GetTranslation())
		};
		Vector3 patch1secondLine[4]
		{
			Vector3(patch1->secondLine[0]->GetTranslation()),
			Vector3(patch1->secondLine[1]->GetTranslation()),
			Vector3(patch1->secondLine[2]->GetTranslation()),
			Vector3(patch1->secondLine[3]->GetTranslation())
		};

		Vector3 patch2secondLine[4]
		{
			Vector3(patch2->secondLine[0]->GetTranslation()),
			Vector3(patch2->secondLine[1]->GetTranslation()),
			Vector3(patch2->secondLine[2]->GetTranslation()),
			Vector3(patch2->secondLine[3]->GetTranslation())
		};

		std::array<Vector3, 4> p
		{
			p3i[patch1index],
			patch1firstLine[3],
			pAvg,
			p3i[patch2index],
		};

		std::array<Vector3, 4> dU
		{
			0.5 * 3 * (p3i[patch1index] - t1i[patch1index]),
			0.5 * 3 * (patch2firstLine[1] - patch2firstLine[0]),
			0.5 * 3 * (pAvg - p1i[patch1index]),
			0.5 * BernstrinHelper::dU(patch2firstLine,0.5f),
		};

		std::array<Vector3, 4> dV
		{
			0.5 * BernstrinHelper::dU(patch1firstLine,0.5f),
			0.5 * 3 * (patch1firstLine[3] - patch1firstLine[2]),
			0.5 * 3 * (p1i[patch2index] - pAvg),
			0.5 * 3 * (t1i[patch2index] - p3i[patch2index]),
		};

		Vector3 e1[4]{ p3i[patch1index], p2i[patch1index],p1i[patch1index], pAvg };
		std::array<Vector3, 4> dUV
		{
			0.5 * BernstrinHelper::dUV(patch1firstLine,patch1secondLine, 0.5f),
			0.5 * BernstrinHelper::dUV(patch2firstLine,patch2secondLine, 0.0f),
			 Vector3(0,0,0),
			0.5 * BernstrinHelper::dUV(patch2firstLine,patch2secondLine, 0.5f),
		};


		Vector3 e2[4]{ pAvg, p1i[patch2index],p2i[patch2index],p3i[patch2index] };
		std::array<Vector3, 4> dVU
		{
			0.5 * BernstrinHelper::dUV(patch1firstLine,patch1secondLine, 0.5f),
			0.5 * BernstrinHelper::dUV(patch1firstLine,patch1secondLine, 1.0f),
			 Vector3(0,0,0),
			0.5 * BernstrinHelper::dUV(patch2firstLine,patch2secondLine, 0.5f),
		};

		auto singlePatch = std::make_shared<SingleGregoryPatch>(
			p,
			dU,
			dV,
			dUV,
			dVU
			);

		singleSurfaces.push_back(singlePatch);
	}
}

void GregoryPatch::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<GregoryPatch>());
}

void GregoryPatch::ResetDrawing()
{
	this->resetDrawing = true;
}

void GregoryPatch::Draw(std::shared_ptr<Camera> camera)
{
	if (resetDrawing)
	{
		UpdateVertices();
	}
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



void GregoryPatch::RenderGUI()
{
	IModel::RenderGUI();

	if (ImGui::Checkbox("Draw polygon chain", &drawPolygonChain))
	{
		for (auto single : singleSurfaces)
		{
			single->SetDrawPolygonChain(drawPolygonChain);
		}
	}

	if (
		ImGui::DragInt("Horizontal Slices Count", &horizontalSlicesCount, 1, 2, 255) ||
		ImGui::DragInt("Vertical Slices Count", &verticalSlicesCount, 1, 2, 255)
		)
	{
		for (auto single : singleSurfaces)
		{
			single->UpdateSlices(horizontalSlicesCount, verticalSlicesCount);
		}
		
	}
}

