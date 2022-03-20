#pragma once
#include "RenderContext.h"
#include <memory>
#include <wtypes.h>
#include "d3d11.h"
#include "dxDevice.h"
class UIContext : RenderContext
{
public:
    UIContext(HWND handle);
    
    ~UIContext() {}

    void pre_render() override;

    void post_render() override;

    void end() override;
};