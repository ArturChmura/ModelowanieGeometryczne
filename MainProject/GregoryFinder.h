#pragma once
#include <vector>
#include <array>
#include <memory>
#include "Point.h"
#include "PatchSide.h"
#include <array>
#include "BezierSurfaceC0.h"
#include "Node.h"

class GregoryFinder
{
public:
	static std::vector<std::vector<std::shared_ptr<Node>>> FindFill(std::vector<std::shared_ptr<BezierSurfaceC0>> surfaces);

	static void DFSCycle(Edge e);

	static std::shared_ptr<PatchSide> GetLeftSide(std::shared_ptr<SingleBezierSurfaceC0> singleSurface);
	static std::shared_ptr<PatchSide> GetRightSide(std::shared_ptr<SingleBezierSurfaceC0> singleSurface);
	static std::shared_ptr<PatchSide> GetTopSide(std::shared_ptr<SingleBezierSurfaceC0> singleSurface);
	static std::shared_ptr<PatchSide> GetBottomSide(std::shared_ptr<SingleBezierSurfaceC0> singleSurface);

public:
	inline static std::vector<std::vector<Edge>> cycles;
	inline static int cycleNumber;
};