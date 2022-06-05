#include "SceneLoader.h"
#include <Serializer.h>
#include "SimpleMath.h"
#include "BezierCurveC0.h"
#include "BezierCurveC2.h"
#include "BezierCurveInterpolating.h"
#include "BezierSurfaceC0.h"
#include "BezierSurfaceC2.h"
#include "SingleBezierSurfaceC0.h"
#include "SingleBezierSurfaceC2.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void SceneLoader::LoadScene(std::shared_ptr<Scene> scene, std::filesystem::path path)
{
	MG1::SceneSerializer sceneSerializer;
	sceneSerializer.LoadScene(path);
	auto& deserializedScene = MG1::Scene::Get();
	scene->Clear();

	std::map<uint32_t, std::shared_ptr<Point>> points;

	for (auto& p : deserializedScene.points)
	{
		auto pos = p.position;
		auto point = std::make_shared<Point>(Vector3{pos.x, pos.y, pos.z});
		point->name = p.name;
		scene->AddModel(point);

		points.insert(std::make_pair(p.GetId(), point));
	}

	for (auto& t : deserializedScene.tori)
	{
		auto pos = t.position;
		auto rot = t.rotation;
		auto sc = t.scale;

		auto torus = std::make_shared<Torus>(t.largeRadius, t.smallRadius, t.samples.x, t.samples. y);
		torus->SetRotation(XMConvertToRadians(t.rotation.x), XMConvertToRadians(t.rotation.y), XMConvertToRadians(t.rotation.z));
		torus->SetTranslation(t.position.x, t.position.y, t.position.z);
		
		torus->SetScale(t.scale.x, t.scale.y, t.scale.z);

		torus->name = t.name;
		scene->AddModel(torus);
	}

	for (auto& b : deserializedScene.bezierC0)
	{
		std::vector<std::shared_ptr<Point>> controlPoints;

		std::transform(b.controlPoints.begin(), b.controlPoints.end(), std::back_inserter(controlPoints),
			[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
		);

		auto bezier = std::make_shared<BezierCurveC0>(controlPoints);

		bezier->name = b.name;

		scene->AddModel(bezier);
	}

	for (auto& b : deserializedScene.bezierC2)
	{
		std::vector<std::shared_ptr<Point>> controlPoints;

		std::transform(b.controlPoints.begin(), b.controlPoints.end(), std::back_inserter(controlPoints),
			[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
		);

		auto bezier = std::make_shared<BezierCurveC2>(controlPoints);

		bezier->name = b.name;

		scene->AddModel(bezier);
	}

	for (auto& b : deserializedScene.interpolatedC2)
	{
		std::vector<std::shared_ptr<Point>> controlPoints;

		std::transform(b.controlPoints.begin(), b.controlPoints.end(), std::back_inserter(controlPoints),
			[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
		);

		auto bezier = std::make_shared<BezierCurveInterpolating>(controlPoints);

		bezier->name = b.name;

		scene->AddModel(bezier);
	}


	for (auto& s : deserializedScene.surfacesC0)
	{
		std::vector<std::shared_ptr<SingleBezierSurfaceC0>> patches;
		std::map<int, std::shared_ptr<Point>> surfacePoints;
		for (auto& patch : s.patches)
		{
			std::vector<std::shared_ptr<Point>> controlPoints;

			std::transform(patch.controlPoints.begin(), patch.controlPoints.end(), std::back_inserter(controlPoints),
				[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
			);

			std::array<std::array<std::shared_ptr<Point>, 4>, 4> points;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					auto point = controlPoints[4 * i + j];
					points[i][j] = point;
					surfacePoints.emplace(std::pair(point->id, point));
				}
			}

			auto singleSurface = std::make_shared<SingleBezierSurfaceC0>(points, patch.samples.x, patch.samples.y);
			patches.push_back(singleSurface);
		}

		auto surface = std::make_shared<BezierSurfaceC0>(patches,s.name);

		surface->horizontalSlicesCount = s.size.x;
		surface->verticalSlicesCount = s.size.y;
		for (auto [_, point]: surfacePoints)
		{
			point->onRemovedFromSceneCallback.Add([scene = scene, surface = surface](std::shared_ptr<Point> p) {scene->RemoveModel(surface->id); }, surface->id);
		}

		scene->AddModel(surface);
	}

	for (auto& s : deserializedScene.surfacesC2)
	{
		std::vector<std::shared_ptr<SingleBezierSurfaceC2>> patches;
		std::map<int, std::shared_ptr<Point>> surfacePoints;

		for (auto& patch : s.patches)
		{
			std::vector<std::shared_ptr<Point>> controlPoints;

			std::transform(patch.controlPoints.begin(), patch.controlPoints.end(), std::back_inserter(controlPoints),
				[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
			);

			std::array<std::array<std::shared_ptr<Point>, 4>, 4> points;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					auto point = controlPoints[4 * i + j];
					points[i][j] = point;
					surfacePoints.emplace(std::pair(point->id, point));
				}
			}

			auto singleSurface = std::make_shared<SingleBezierSurfaceC2>(points, patch.samples.x, patch.samples.y);
			patches.push_back(singleSurface);
		}

		auto surface = std::make_shared<BezierSurfaceC2>(patches,s.name);
		surface->horizontalSlicesCount = s.size.x;
		surface->verticalSlicesCount = s.size.y;
		for (auto [_, point] : surfacePoints)
		{
			point->onRemovedFromSceneCallback.Add([scene = scene, surface = surface](std::shared_ptr<Point> p) {scene->RemoveModel(surface->id); }, surface->id);
		}
		scene->AddModel(surface);
	}
}
