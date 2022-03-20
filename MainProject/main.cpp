#include <memory>
#include "application.h"
#include <tchar.h>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
SIZE windowSize = { 1280,720 };
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	    ::RegisterClassEx(&wc);

	HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("CAD 2022 Pro"), WS_OVERLAPPEDWINDOW, 100, 100, windowSize.cx, windowSize.cy, NULL, NULL, wc.hInstance, NULL);

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    
    auto window = std::make_unique<DXDWindow>(hwnd, wc, windowSize);

	while (window->is_running())
	{
		window->render();
	}

    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
	return 0;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
