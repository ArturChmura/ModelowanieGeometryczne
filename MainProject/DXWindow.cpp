#include "DXWindow.h"


DXDWindow::DXDWindow(HWND handle, WNDCLASSEX wc, SIZE size) 
    :mIsRunning(true)
{
    
        this->handle = handle;

        Width = size.cx;
        Height = size.cy;

        mSceneView = std::make_unique<SceneView>();
        mUICtx = std::make_unique<UIContext>(handle);
    
}

DXDWindow::~DXDWindow()
{
    mUICtx->end();
}

void DXDWindow::on_resize(int width, int height)
{
   /* Width = width;
    Height = height;

    mSceneView->resize(Width, Height);
    render();*/
}

void DXDWindow::on_scroll(double delta)
{
}

void DXDWindow::on_key(int key, int scancode, int action, int mods)
{
}

void DXDWindow::on_close()
{
    mIsRunning = false;
}

void DXDWindow::render()
{
    mUICtx->pre_render();

    mSceneView->Render();

    mUICtx->post_render();

    handle_input();
}

void DXDWindow::handle_input()
{

}


