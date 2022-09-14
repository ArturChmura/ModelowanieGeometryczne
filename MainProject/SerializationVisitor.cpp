#include "SerializationVisitor.h"
#include <Serializer.h>
#include "IModel.h"
#include "Point.h"
#include "Torus.h"
#include "BezierCurveC0.h"
#include "BezierCurveC2.h"
#include "BezierCurveInterpolating.h"
#include "SingleBezierSurfaceC0.h"
#include "BezierSurfaceC0.h"
#include "SingleBezierSurfaceC2.h"
#include "BezierSurfaceC2.h"
#include "CompositeModel.h"

using namespace DirectX;


void SerializationVisitor::Accept(std::shared_ptr < Point> point)
{
	auto& scene = MG1::Scene::Get();
	auto p = MG1::Point();

	p.name = point->name;
	p.SetId(point->id);

	auto position = point->GetTranslation();

	p.position = MG1::Float3{ position.x, position.y, position.z };

	scene.points.push_back(p);
}

void SerializationVisitor::Accept(std::shared_ptr < Torus> torus)
{
	auto& scene = MG1::Scene::Get();
	auto t = MG1::Torus();

	auto position = torus->GetTranslation();
	auto rotation = torus->GetRotation();
	auto scale = torus->GetScale();
	
	t.position = MG1::Float3{ position.x, position.y, position.z };
	t.rotation = MG1::Float3{ XMConvertToDegrees(rotation.x), XMConvertToDegrees(rotation.y), XMConvertToDegrees(rotation.z) };
	t.scale = MG1::Float3{ scale.x, scale.y, scale.z };

	t.name = torus->name;
	t.largeRadius = torus->GetBigRadius();
	t.smallRadius = torus->GetSmallRadius();
	t.samples = MG1::Uint2{ static_cast<uint32_t>(torus->GetSmallSlices()), static_cast<uint32_t>(torus->GetLargeSlices()) };

	t.SetId(torus->id);

	scene.tori.push_back(t);
}

void SerializationVisitor::Accept(std::shared_ptr < BezierCurveC0> curve)
{
	auto& scene = MG1::Scene::Get();
	auto c = MG1::BezierC0();

	c.name = curve->name;
	c.SetId(curve->id);

	for (auto cp : curve->points)
	{
		c.controlPoints.push_back(MG1::PointRef(cp->id));
	}

	scene.bezierC0.push_back(c);
}

void SerializationVisitor::Accept(std::shared_ptr < BezierCurveC2> curve)
{
	auto& scene = MG1::Scene::Get();
	auto c = MG1::BezierC2();

	c.name = curve->name;
	c.SetId(curve->id);

	for (auto cp : curve->points)
	{
		c.controlPoints.push_back(MG1::PointRef(cp->id));
	}

	scene.bezierC2.push_back(c);
}

void SerializationVisitor::Accept(std::shared_ptr < BezierCurveInterpolating> curve)
{
	auto& scene = MG1::Scene::Get();
	auto c = MG1::InterpolatedC2();

	c.name = curve->name;
	c.SetId(curve->id);

	for (auto cp : curve->points)
	{
		c.controlPoints.push_back(MG1::PointRef(cp->id));
	}

	scene.interpolatedC2.push_back(c);
}

void SerializationVisitor::Accept(std::shared_ptr < SingleBezierSurfaceC0> patch)
{
}

void SerializationVisitor::Accept(std::shared_ptr < BezierSurfaceC0> surface)
{
	auto& scene = MG1::Scene::Get();
	auto s = MG1::BezierSurfaceC0();
	s.size = { (UINT)surface->horizontalSlicesCount, (UINT)surface->verticalSlicesCount };
	s.name = surface->name;
	s.SetId(surface->id);

	for (auto& patch : surface->singleSurfaces)
	{
		auto p = MG1::BezierPatchC0();
		p.name = patch->name;
		p.SetId(patch->id);

		p.samples = MG1::Uint2{ static_cast<uint32_t>(patch->verticalSlices), static_cast<uint32_t>(patch->horizontalSlices) };

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				auto point = patch->points[j][i]; // na odwrót, bo ustaliliœmy kolejnoœæ punktów wzd³u¿ U 
				p.controlPoints.push_back(MG1::PointRef(point->id));
			}
		}

		s.patches.push_back(p);
	}

	scene.surfacesC0.push_back(s);
}

void SerializationVisitor::Accept(std::shared_ptr < SingleBezierSurfaceC2> patch)
{
}

void SerializationVisitor::Accept(std::shared_ptr < BezierSurfaceC2> surface)
{
	auto& scene = MG1::Scene::Get();
	auto s = MG1::BezierSurfaceC2();
	s.size = { (UINT)surface->horizontalSlicesCount, (UINT)surface->verticalSlicesCount };
	s.name = surface->name;
	s.SetId(surface->id);

	for (auto& patch : surface->singleSurfaces)
	{
		auto p = MG1::BezierPatchC2();
		p.name = patch->name;
		p.SetId(patch->id);

		p.samples = MG1::Uint2{ static_cast<uint32_t>(patch->verticalSlices), static_cast<uint32_t>(patch->horizontalSlices) };

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				auto point = patch->points[j][i]; // na odwrót, bo ustaliliœmy kolejnoœæ punktów wzd³u¿ U 
				p.controlPoints.push_back(MG1::PointRef(point->id));
			}
		}

		s.patches.push_back(p);
	}

	scene.surfacesC2.push_back(s);
}

void SerializationVisitor::Accept(std::shared_ptr < CompositeModel> surface)
{
}

void SerializationVisitor::Accept(std::shared_ptr<GregoryPatch> surface)
{
}

void SerializationVisitor::Accept(std::shared_ptr<SingleGregoryPatch> surface)
{
}
