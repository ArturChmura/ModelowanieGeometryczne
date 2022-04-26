#pragma once

#include "psConstColor.h"
#include "vsConstColor.h"

class LineShader
{
public:
	LineShader();
	VSConstColor vertexShader;
	PSConstColor pixelShader;
	void SetupRender();
};