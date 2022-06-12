#include "GregoryFactory.h"

#include "CompositeModel.h"
#include "Point.h"
#include "BezierCurveInterpolating.h"
#include "BernstrinHelper.h";
#include <array>
using namespace DirectX::SimpleMath;
std::vector<std::shared_ptr<IModel>> GregoryFactory::CreateGregoryPatch(std::vector<std::shared_ptr<PatchSide>> patchesSides)
{
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
		t1i[i] = DeCastilljeu(coef1, 0.5f);
		p3i[i] = DeCastilljeu(coef0, 0.5f);
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

		Vector3 p[4]
		{
			p3i[patch1index],
			patch1firstLine[3],
			p3i[patch2index],
			pAvg
		};

		Vector3 dU[4]
		{
			3 * (p3i[patch1index] - t1i[patch1index]),
			3 * (patch2firstLine[1] - patch2firstLine[0]),
			BernstrinHelper::dU(patch2firstLine,0.5f),
			3 * (pAvg - p1i[patch1index])
		};

		Vector3 dV[4]
		{
			BernstrinHelper::dU(patch1firstLine,0.5f),
			3 * (patch1firstLine[3] - patch1firstLine[2]),
			3 * (t1i[patch2index] - p3i[patch2index]),
			3 * (p1i[patch2index] - pAvg)
		};

		Vector3 e1[4]{ p3i[patch1index], p2i[patch1index],p1i[patch1index], pAvg };
		Vector3 dUV[4]
		{
			BernstrinHelper::dUV(patch1firstLine, 0.5f),
			BernstrinHelper::dUV(patch2firstLine, 0.0f),
			BernstrinHelper::dUV(patch2firstLine, 0.5f),
			BernstrinHelper::dUV(e1, 1.0f),
		};


		Vector3 e2[4]{ pAvg, p1i[patch2index],p2i[patch2index],p3i[patch2index] };
		Vector3 dVU[4]
		{
			BernstrinHelper::dUV(patch1firstLine, 0.5f),
			BernstrinHelper::dUV(patch1firstLine, 1.0f),
			BernstrinHelper::dUV(patch2firstLine, 0.5f),
			BernstrinHelper::dUV(e2, 1.0f),
		};


	}

	auto deleteModel = std::vector<std::shared_ptr<IModel>>();
	for (int i = 0; i < patchesSides.size(); i++)
	{
		auto p3 = std::make_shared<Point>(p3i[i]);
		auto p2 = std::make_shared<Point>(p2i[i]);
		auto p1 = std::make_shared<Point>(p1i[i]);
		auto p0 = std::make_shared<Point>(pAvg);
		std::vector<std::shared_ptr<Point>> vector = { p3, p2, p1, p0 };
		auto line = std::make_shared<BezierCurveInterpolating>(vector);
		deleteModel.push_back(line);

		deleteModel.push_back(p3);
		deleteModel.push_back(p2);
		deleteModel.push_back(p1);
		deleteModel.push_back(p0);
	}

	return deleteModel;
}

DirectX::SimpleMath::Vector3 GregoryFactory::DeCastilljeu(DirectX::SimpleMath::Vector3 coef[4], float t)
{
	for (int i = 0; i < 3; i++)
	{
		int currentSize = 4 - i;
		for (int j = 0; j < currentSize - 1; j++)
		{
			coef[j] = coef[j] * (1.0f - t) + coef[j + 1] * t;
		}
	}
	Vector3 value = coef[0];

	return value;
}
