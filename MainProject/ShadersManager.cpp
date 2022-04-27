#include "ShadersManager.h"

void ShadersManager::LoadShaders()
{
	gsBezier = std::make_shared<GSBezier>();
	vsBezier = std::make_shared<VSBezier>();
	psColor = std::make_shared<PSColor>();
	psConstColor = std::make_shared<PSConstColor>();
	vsColor = std::make_shared<VSColor>();
	vsConstColor = std::make_shared<VSConstColor>();

}
