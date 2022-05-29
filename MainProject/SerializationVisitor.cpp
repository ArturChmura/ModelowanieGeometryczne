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

void SerializationVisitor::Visit(IModel& model)
{
	model.Accept(*this);
}

void SerializationVisitor::Accept(Point& point)
{
	auto& scene = MG1::Scene::Get();
	auto p = MG1::Point();

	p.name = point.name;
	p.SetId(point.id);

	auto position = point.GetTranslation();

	p.position = MG1::Float3{ position.x, position.y, position.z };

	scene.points.push_back(p);
}

void SerializationVisitor::Accept(Torus& torus)
{
	auto& scene = MG1::Scene::Get();
	auto t = MG1::Torus();

	auto position = torus.GetTranslation();
	auto rotation = torus.GetRotation();
	auto scale = torus.GetScale();

	t.position = MG1::Float3{ position.x, position.y, position.z };
	t.rotation = MG1::Float3{ rotation.x, rotation.y, rotation.z };
	t.scale = MG1::Float3{ scale.x, scale.y, scale.z };

	t.name = torus.name;
	t.largeRadius = torus.GetBigRadius();
	t.smallRadius = torus.GetSmallRadius();
	t.samples = MG1::Uint2{ static_cast<uint32_t>(torus.GetSmallSlices()), static_cast<uint32_t>(torus.GetLargeSlices()) };

	t.SetId(torus.id);

	scene.tori.push_back(t);
}

void SerializationVisitor::Accept(BezierCurveC0& curve)
{
	auto& scene = MG1::Scene::Get();
	auto c = MG1::BezierC0();

	c.name = curve.name;
	c.SetId(curve.id);

	for (auto cp : curve.points)
	{
		c.controlPoints.push_back(MG1::PointRef(cp->id));
	}

	scene.bezierC0.push_back(c);
}

void SerializationVisitor::Accept(BezierCurveC2& curve)
{
	auto& scene = MG1::Scene::Get();
	auto c = MG1::BezierC2();

	c.name = curve.name;
	c.SetId(curve.id);

	for (auto cp : curve.points)
	{
		c.controlPoints.push_back(MG1::PointRef(cp->id));
	}

	scene.bezierC2.push_back(c);
}

void SerializationVisitor::Accept(BezierCurveInterpolating& curve)
{
	auto& scene = MG1::Scene::Get();
	auto c = MG1::InterpolatedC2();

	c.name = curve.name;
	c.SetId(curve.id);

	for (auto cp : curve.points)
	{
		c.controlPoints.push_back(MG1::PointRef(cp->id));
	}

	scene.interpolatedC2.push_back(c);
}

void SerializationVisitor::Accept(SingleBezierSurfaceC0& patch)
{
}

void SerializationVisitor::Accept(BezierSurfaceC0& surface)
{
	auto& scene = MG1::Scene::Get();
	auto s = MG1::BezierSurfaceC0();

	s.name = surface.name;
	s.SetId(surface.id);

	for (auto& patch : surface.singleSurfaces)
	{
		auto p = MG1::BezierPatchC0();
		p.name = patch->name;
		p.SetId(patch->id);

		p.samples = MG1::Uint2{ static_cast<uint32_t>(patch->verticalSlices), static_cast<uint32_t>(patch->horizontalSlices) };

		for (auto& pa : patch->points)
		{
			for (auto& cp : pa)
			{
				p.controlPoints.push_back(MG1::PointRef(cp->id));
			}
		}

		s.patches.push_back(p);
	}

	scene.surfacesC0.push_back(s);
}

void SerializationVisitor::Accept(SingleBezierSurfaceC2& patch)
{
}

void SerializationVisitor::Accept(BezierSurfaceC2& surface)
{
	auto& scene = MG1::Scene::Get();
	auto s = MG1::BezierSurfaceC0();

	s.name = surface.name;
	s.SetId(surface.id);

	for (auto& patch : surface.singleSurfaces)
	{
		auto p = MG1::BezierPatchC0();
		p.name = patch->name;
		p.SetId(patch->id);

		p.samples = MG1::Uint2{ static_cast<uint32_t>(patch->verticalSlices), static_cast<uint32_t>(patch->horizontalSlices) };

		for (auto& pa : patch->points)
		{
			for (auto& cp : pa)
			{
				p.controlPoints.push_back(MG1::PointRef(cp->id));
			}
		}

		s.patches.push_back(p);
	}

	scene.surfacesC0.push_back(s);
}

void SerializationVisitor::Accept(CompositeModel& surface)
{
}
