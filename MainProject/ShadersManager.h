#pragma once
#include "gsBezier.h"
#include "vsBezier.h"
#include "psColor.h"
#include "psConstColor.h"
#include "vsColor.h"
#include "vsConstColor.h"
#include <memory>
#include "gsSurfaceBezier.h"
#include "vsSurfaceBezier.h"

class ShadersManager
{
public:
	inline static std::shared_ptr<GSBezier> gsBezier;
	inline static std::shared_ptr<VSBezier> vsBezier;

	inline static std::shared_ptr<GSSurfaceBezier> gsSurfaceBezier;
	inline static std::shared_ptr<VSSurfaceBezier> vsSurfaceBezier;

	inline static std::shared_ptr<PSColor> psColor;
	inline static std::shared_ptr<PSConstColor> psConstColor;

	inline static std::shared_ptr<VSColor> vsColor;
	inline static std::shared_ptr<VSConstColor> vsConstColor;

	static void LoadShaders();
};