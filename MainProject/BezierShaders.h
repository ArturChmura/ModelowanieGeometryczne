#pragma once

#include "vsBezier.h"
#include "gsBezier.h"
#include "psConstColor.h"

class BezierShaders
{
public:
	BezierShaders();
	VSBezier vertexShader;
	GSBezier geometryShader;
	PSConstColor pixelShader;
	void SetupRender();
};