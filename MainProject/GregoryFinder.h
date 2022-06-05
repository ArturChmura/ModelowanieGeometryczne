#pragma once
#include <vector>
#include <array>
#include <memory>
#include "Point.h"
#include "Node.h"
class GregoryFinder
{
public:
	static bool FindFill(std::vector<std::vector<std::shared_ptr<Point>>> lines, std::vector<std::array<std::shared_ptr<Point>, 4>>& outResult);

private:
	static bool FindCycle(int startNodeId, std::shared_ptr<Node> node, int steps, std::vector<std::array<std::shared_ptr<Point>, 4>>& outResult, std::vector<bool>& used);
};