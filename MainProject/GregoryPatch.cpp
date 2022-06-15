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
		std::array<DirectX::SimpleMath::Vector3, 4> coef1;
		std::array<DirectX::SimpleMath::Vector3, 4> coef0;
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
		auto patch1first = GetHalf(patch1->firstLine, 1);
		auto patch1second = GetHalf(patch1->secondLine, 1);
		auto patch2first = GetHalf(patch2->firstLine, 0);
		auto patch2second = GetHalf(patch2->secondLine, 0);

		std::array<Vector3, 4> p
		{
			p3i[patch1index],
			patch1first[3],
			pAvg,
			p3i[patch2index],
		};

		std::array<Vector3, 4> dU
		{
			3 * (p3i[patch1index] - t1i[patch1index]),
			3 * (patch2first[1] - patch2first[0]),
			3 * (pAvg - p1i[patch1index]),
			BernstrinHelper::dU(patch2first,1.0f),
		};

		std::array<Vector3, 4> dV
		{
			BernstrinHelper::dU(patch1first,0.0f),
			3 * (patch1first[3] - patch1first[2]),
			3 * (p1i[patch2index] - pAvg),
			3 * (t1i[patch2index] - p3i[patch2index]),
		};

		Vector3 e1[4]{ p3i[patch1index], p2i[patch1index],p1i[patch1index], pAvg };
		std::array<Vector3, 4> dUV
		{
			BernstrinHelper::dUV(patch1first,patch1second, 0.0f),
			BernstrinHelper::dUV(patch2first,patch2second, 0.0f),
			 Vector3(0,0,0),
			BernstrinHelper::dUV(patch2first,patch2second, 1.0f),
		};


		Vector3 e2[4]{ pAvg, p1i[patch2index],p2i[patch2index],p3i[patch2index] };
		std::array<Vector3, 4> dVU
		{
			 BernstrinHelper::dUV(patch1first,patch1second, 0.0f),
			 BernstrinHelper::dUV(patch1first,patch1second, 1.0f),
			 Vector3(0,0,0),
			 BernstrinHelper::dUV(patch2first,patch2second, 1.0f),
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

std::array<DirectX::SimpleMath::Vector3, 4> GregoryPatch::GetHalf(std::array<std::shared_ptr<Point>, 4> points, bool half)
{

	std::array<DirectX::SimpleMath::Vector3, 4> points4
	{
		Vector3(points[0]->GetTranslation()),
		Vector3(points[1]->GetTranslation()),
		Vector3(points[2]->GetTranslation()),
		Vector3(points[3]->GetTranslation())
	};
	std::array<DirectX::SimpleMath::Vector3, 3> points3;
	for (int i = 0; i < 3; i++)
	{
		points3[i] = (points4[i] + points4[i + 1]) / 2;
	}

	std::array<DirectX::SimpleMath::Vector3, 2> points2;
	for (int i = 0; i < 2; i++)
	{
		points2[i] = (points3[i] + points3[i + 1]) / 2;
	}
	std::array<DirectX::SimpleMath::Vector3, 1> points1;
	for (int i = 0; i < 1; i++)
	{
		points1[i] = (points2[i] + points2[i + 1]) / 2;
	}
	std::array<DirectX::SimpleMath::Vector3, 4> result;
	if (!half)
	{
		result =
		{
			points4[0],
			points3[0],
			points2[0],
			points1[0]
		};
	}
	else
	{
		result =
		{
			points1[0],
			points2[1],
			points3[2],
			points4[3]
		};
	}
	return result;
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
		//for (int i = 0; i < 1; i++)
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

