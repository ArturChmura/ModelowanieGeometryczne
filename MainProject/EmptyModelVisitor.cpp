#include "EmptyModelVisitor.h"

void EmptyModelVisitor::Accept(std::shared_ptr < Point> point)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr < Torus> torus)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr < BezierCurveC0> curve)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr < BezierCurveC2> curve)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr < BezierCurveInterpolating> curve)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr < SingleBezierSurfaceC0> patch)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr < BezierSurfaceC0> surface)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr < SingleBezierSurfaceC2> patch)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr < BezierSurfaceC2> surface)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr < CompositeModel> surface)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr<GregoryPatch> surface)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr<SingleGregoryPatch> surface)
{
}

void EmptyModelVisitor::Accept(std::shared_ptr<StraightCurveInterpolating> curve)
{
}
