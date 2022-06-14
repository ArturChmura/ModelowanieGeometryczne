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


	auto deleteModel = std::vector<std::shared_ptr<IModel>>();
	std::vector<std::shared_ptr<SingleGregoryPatch>> singlePatches;
	for (int i = 0; i < patchesSides.size(); i++)
		//for (int i = 0; i < 1; i++)
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

		std::array<Vector3 ,4> p
		{
			p3i[patch1index],
			patch1firstLine[3],
			pAvg,
			p3i[patch2index],
		};

		std::array<Vector3, 4> dU
		{
			0.5 * 3 * (p3i[patch1index] - t1i[patch1index]),
			0.5* 3 * (patch2firstLine[1] - patch2firstLine[0]),
			0.5 * 3 * (pAvg - p1i[patch1index]),
			0.5* BernstrinHelper::dU(patch2firstLine,0.5f),
		};

		std::array<Vector3, 4> dV
		{
			0.5* BernstrinHelper::dU(patch1firstLine,0.5f),
			0.5* 3 * (patch1firstLine[3] - patch1firstLine[2]),
			0.5 * 3 * (p1i[patch2index] - pAvg),
			0.5 * 3 * (t1i[patch2index] - p3i[patch2index]),
		};

		Vector3 e1[4]{ p3i[patch1index], p2i[patch1index],p1i[patch1index], pAvg };
		std::array<Vector3, 4> dUV
		{
			0.5 * BernstrinHelper::dUV(patch1firstLine,patch1secondLine, 0.5f),
			0.5 * BernstrinHelper::dUV(patch2firstLine,patch2secondLine, 0.0f),
			 Vector3(0,0,0),
			0.5* BernstrinHelper::dUV(patch2firstLine,patch2secondLine, 0.5f),
		};

		
		Vector3 e2[4]{ pAvg, p1i[patch2index],p2i[patch2index],p3i[patch2index] };
		std::array<Vector3, 4> dVU
		{
			0.5* BernstrinHelper::dUV(patch1firstLine,patch1secondLine, 0.5f),
			0.5* BernstrinHelper::dUV(patch1firstLine,patch1secondLine, 1.0f),
			 Vector3(0,0,0),
			0.5* BernstrinHelper::dUV(patch2firstLine,patch2secondLine, 0.5f),
		};

		auto singlePatch = std::make_shared<SingleGregoryPatch>(
			p,
			dU,
			dV,
			dUV,
			dVU
			);

		singlePatches.push_back(singlePatch);

		for (int i = 0; i < 4; i++)
		{
			auto p1 = std::make_shared<Point>(p[i]);
			auto p2 = std::make_shared<Point>(p[i] + 1.0f / 3 * dU[i]);
			std::vector<std::shared_ptr<Point>> vector = { p1,p2 };
			auto line = std::make_shared<BezierCurveInterpolating>(vector);
			line->ChangeColor({ 1,1,0 });
			//deleteModel.push_back(line);

			p1 = std::make_shared<Point>(p[i]);
			p2 = std::make_shared<Point>(p[i] + 1.0f/3*dV[i]);
			 vector = { p1,p2 };
			line = std::make_shared<BezierCurveInterpolating>(vector);
			line->ChangeColor({ 0,1,0 });
			deleteModel.push_back(line);

		/*	p1 = std::make_shared<Point>(p[i]);
			p2 = std::make_shared<Point>(p[i] + 1.0f/9*dUV[i]);
			 vector = { p1,p2 };
			line = std::make_shared<BezierCurveInterpolating>(vector);
			line->ChangeColor({ 1,0,0 });
			deleteModel.push_back(line);

			p1 = std::make_shared<Point>(p[i]);
			p2 = std::make_shared<Point>(p[i] + 1.0f / 9 * dVU[i]);
			 vector = { p1,p2 };
			line = std::make_shared<BezierCurveInterpolating>(vector);
			line->ChangeColor({ 0,1,1 });
			deleteModel.push_back(line);*/
		}
		
	}

	auto gregory = std::make_shared<GregoryPatch>(singlePatches);
	gregory->ChangeColor({ 1,0,1 });
	deleteModel.push_back(gregory);
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
