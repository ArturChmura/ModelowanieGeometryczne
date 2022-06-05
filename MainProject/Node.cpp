#include "Node.h"



Node::Node(std::shared_ptr<Point> point, int id)
{
	this->point = point;
	this->id = id;
}

void Node::AddNeighbor(std::shared_ptr<Node> neighbor)
{
	this->neighbors.push_back(neighbor);
}

int Node::GetDegree()
{
	return neighbors.size();
}
