#pragma once

#include <vector>
#include <memory>
#include "Point.h"

struct Node;

struct Edge
{
	std::shared_ptr<Node> begin;
	std::shared_ptr<Node> end;
	int value;
	int patchSideId;
};

struct Node
{
	std::vector<Edge> edgesOut;
	int pointId;
	int color;
	Edge par;

	void AddEdge(Edge edge)
	{
		edgesOut.push_back(edge);
	}
};
