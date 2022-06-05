#pragma once
#include <vector>
#include <memory>
#include "Point.h"


class Node
{
public:
	std::shared_ptr<Point> point;
	int id; 
	Node(std::shared_ptr<Point> point, int id);
	void AddNeighbor(std::shared_ptr<Node> neighbor);
	std::vector<std::shared_ptr<Node>> neighbors;
	int GetDegree();

};