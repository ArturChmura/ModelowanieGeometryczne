#pragma once
#include <string>
#include "IGuiWindow.h"

class DebugWindow : public IGuiWindow
{
public:
	void Render();
	inline static std::string debugMessage;

};