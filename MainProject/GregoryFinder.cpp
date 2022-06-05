#include "GregoryFinder.h"
#include "Node.h"
#include <map>

bool GregoryFinder::FindFill(std::vector<std::vector<std::shared_ptr<Point>>> lines, std::vector<std::array<std::shared_ptr<Point>, 4>>& outResult)
{
    std::map<int, std::shared_ptr<Node>> nodesMap;

    int nodeId = 0;
    for (auto line : lines)
    {
        for (auto point : line)
        {
            if (!nodesMap.contains(point->id))
            {
                auto node = std::make_shared<Node>(point, nodeId++);
                nodesMap.emplace(point->id, node);
            }
        }
    }

    for (auto line : lines)
    {
        for (int i = 0; i < line.size(); i++)
        {
            auto point = line[i];
            auto nextPoint = line[(i+1)% line.size()];
            auto node = nodesMap[point->id];
            auto nextNode = nodesMap[nextPoint->id];

            node->AddNeighbor(nextNode);
            nextNode->AddNeighbor(node);
        }
    }

    std::shared_ptr<Node> startNode;
    for (auto [_, node] : nodesMap)
    {
        if (node->GetDegree() > 2)
        {
            startNode = node;
        }
    }
    if (!startNode)
    {
        return false;
    }
    outResult = std::vector<std::array<std::shared_ptr<Point>, 4>>(1);
    std::vector<bool> used(nodesMap.size());
    bool find = FindCycle(startNode->point->id, startNode, 0, outResult, used);
    return find;

}

bool GregoryFinder::FindCycle(int startNodeId, std::shared_ptr<Node> node, int steps, std::vector<std::array<std::shared_ptr<Point>, 4>>& outResult, std::vector<bool>& used)
{
  
    int resultSize = outResult.size();
    outResult[resultSize - 1][steps] = node->point;
    if (steps == 3)
    {
        if (node->point->id == startNodeId)
        {
            if (outResult.size() > 2)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        if (node->GetDegree() > 2)
        {
            outResult.push_back(std::array<std::shared_ptr<Point>, 4>());
            outResult[resultSize][0] = node->point;
        }
        else
        {
            used[node->id] = false;
            return false;
        }
    }

    if (used[node->id] == true)
    {
        return false;
    }

    used[node->id] = true;
    for (auto neighbor : node->neighbors)
    {
        int nextStep = steps == 3 ? 1 : steps + 1;
        if (FindCycle(startNodeId, neighbor, nextStep, outResult, used))
        {
            return true;
        }
    }
    used[node->id] = false;
    if (steps == 3)
    {
        outResult.pop_back();
    }
    return false;
    
}