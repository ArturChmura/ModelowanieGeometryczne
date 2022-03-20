#pragma once
#include <string>
#include <memory>
#include "UIContext.h"
#include "SceneView.h"
class DXDWindow
{
public:

    DXDWindow(HWND handle, WNDCLASSEX wc, SIZE size);

    ~DXDWindow();

    void render();

    void handle_input();


    void on_scroll(double delta);

    void on_key(int key, int scancode, int action, int mods);

    void on_resize(int width, int height);

    void on_close();

    bool is_running() { return mIsRunning; }

    int Width;
    int Height;
    std::string Title;
    HWND handle;
private:
    std::unique_ptr<UIContext> mUICtx;
    std::unique_ptr<SceneView> mSceneView;

    bool mIsRunning;


};