#include "GregoryFinder.h"
#include <map>

std::vector<std::vector<std::shared_ptr<Node>>> GregoryFinder::FindFill(std::vector<std::shared_ptr<BezierSurfaceC0>> surfaces)
{
	auto sides = std::vector<std::shared_ptr<PatchSide>>();
	int id = 0;
	for (auto surface : surfaces)
	{
		for (int i = 0; i < surface->verticalSlicesCount; i++)
		{
			auto leftSide = GetLeftSide(surface->singleSurfaces[i * surface->horizontalSlicesCount + 0]);
			auto rightSide = GetRightSide(surface->singleSurfaces[i * surface->horizontalSlicesCount + surface->horizontalSlicesCount - 1]);
			sides.push_back(leftSide);
			sides.push_back(rightSide);
		}

		for (int i = 0; i < surface->horizontalSlicesCount; i++)
		{
			auto topSide = GetTopSide(surface->singleSurfaces[i]);
			auto bottomSide = GetBottomSide(surface->singleSurfaces[(surface->verticalSlicesCount-1) * surface->horizontalSlicesCount + i ]);
			sides.push_back(topSide);
			sides.push_back(bottomSide);
		}
	}


	std::map<int, std::shared_ptr<Node>> nodeMap;

	for (auto side : sides)
	{
		std::shared_ptr<Node> nodes[2];
		for (int i = 0; i < 2; i++)
		{
			if (nodeMap.contains(side->pointIds[i]))
			{
				nodes[i] = nodeMap[side->pointIds[i]];
			}
			else
			{
				nodes[i] = std::make_shared<Node>();
				nodes[i]->pointId = side->pointIds[i];
				nodeMap.emplace(side->pointIds[i], nodes[i]);
			}
		}

		Edge edge01;
		edge01.value = side->patchId;
		edge01.begin = nodes[0];
		edge01.end = nodes[1];

		Edge edge10;
		edge10.value = side->patchId;
		edge10.begin = nodes[1];
		edge10.end = nodes[0];

		nodes[0]->AddEdge(edge01);
		nodes[1]->AddEdge(edge10);
	}

	cycleNumber = 0;
	cycles = std::vector<std::vector<Edge>>();

	for (auto [_, node] : nodeMap)
	{
		if (node->color == 0)
		{
			DFSCycle(node->edgesOut[0]);
		}
	}
	
	for (int i = 0; i < sides.size(); i++)
	{
		
	}

	std::vector<std::vector<std::shared_ptr<Node>>> result;
	for (auto cycle : cycles)
	{
		std::vector<std::shared_ptr<Node>> nodeCycle;
		for (auto edge : cycle)
		{
			nodeCycle.push_back(edge.end);
		}
		result.push_back(nodeCycle);
	}
	return result;

}

void GregoryFinder::DFSCycle(Edge e)
{
	auto u = e.end;
	auto p = e.begin;
	if (u->color == 2)
	{
		return;
	}

	if (u->color == 1)
	{
		cycles.push_back(std::vector<Edge>());
		Edge cur = p->par;
		cycles[cycleNumber].push_back(e);

		// backtrack the vertex which
		// are in the current cycle
		// thats found
		while (cur.begin != e.begin && cur.end != e.end)
		{
			cycles[cycleNumber].push_back(cur);
			cur = cur.begin->par;
		}
		cycleNumber++;
		return;
	}

	u->par = e;

	// partially visited.
	u->color = 1;

	// simple dfs on graph
	for(auto edgeOut : u->edgesOut)
	{
		if (edgeOut.value == e.value)
		{
			continue;
		}

		
		DFSCycle(edgeOut);
		
	}

	// completely visited.
	u->color = 2;
}

std::shared_ptr<PatchSide> GregoryFinder::GetLeftSide(std::shared_ptr<SingleBezierSurfaceC0> singleSurface)
{
	auto topLeftPoint = singleSurface->points[0][0];
	auto bottomLeftPoint = singleSurface->points[3][0];

	auto patchSide = std::make_shared<PatchSide>();
	patchSide->pointIds[0] = topLeftPoint->id;
	patchSide->pointIds[1] = bottomLeftPoint->id;
	patchSide->patchId = singleSurface->id;

	return patchSide;
}

std::shared_ptr < PatchSide> GregoryFinder::GetRightSide(std::shared_ptr<SingleBezierSurfaceC0> singleSurface)
{
	auto topRightPoint = singleSurface->points[0][3];
	auto bottomRightPoint = singleSurface->points[3][3];

	auto patchSide = std::make_shared<PatchSide>();
	patchSide->pointIds[0] = topRightPoint->id;
	patchSide->pointIds[1] = bottomRightPoint->id;
	patchSide->patchId = singleSurface->id;

	return patchSide;

}

std::shared_ptr < PatchSide> GregoryFinder::GetTopSide(std::shared_ptr<SingleBezierSurfaceC0> singleSurface)
{
	auto leftTopPoint = singleSurface->points[0][0];
	auto rightTopPoint = singleSurface->points[0][3];

	auto patchSide = std::make_shared<PatchSide>();
	patchSide->pointIds[0] = leftTopPoint->id;
	patchSide->pointIds[1] = rightTopPoint->id;
	patchSide->patchId = singleSurface->id;

	return patchSide;

}

std::shared_ptr < PatchSide> GregoryFinder::GetBottomSide(std::shared_ptr<SingleBezierSurfaceC0> singleSurface)
{
	auto leftBottomPoint = singleSurface->points[3][0];
	auto rightBottomPoint = singleSurface->points[3][3];

	auto patchSide = std::make_shared<PatchSide>();
	patchSide->pointIds[0] = leftBottomPoint->id;
	patchSide->pointIds[1] = rightBottomPoint->id;
	patchSide->patchId = singleSurface->id;

	return patchSide;

}
