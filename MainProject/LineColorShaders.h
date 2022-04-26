#pragma once

#include "psConstColor.h"
#include "vsColor.h"
#include "psColor.h"

class LineColorShaders
{
public:
	LineColorShaders();
	VSColor vertexShader;
	PSColor pixelShader;
	void SetupRender();
};