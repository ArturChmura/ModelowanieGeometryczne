#include "ShadersManager.h"

void ShadersManager::LoadShaders()
{
	gsBezier = std::make_shared<GSBezier>();
	vsBezier = std::make_shared<VSBezier>();

	gsBezierSurfaceC2 = std::make_shared<GSBezierSurfaceC2>();
	gsSurfaceBezier = std::make_shared<GSSurfaceBezier>();
	vsSurfaceBezier = std::make_shared<VSSurfaceBezier>();

	psColor = std::make_shared<PSColor>();
	psConstColor = std::make_shared<PSConstColor>();

	vsColor = std::make_shared<VSColor>();
	vsConstColor = std::make_shared<VSConstColor>();

	vsGregory = std::make_shared<VSGregory>();
}
