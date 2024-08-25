#include <windows.h>
#include <iostream>

// 全局变量用于存储鼠标钩子的句柄
HHOOK g_hMouseHook = NULL;

// 获取窗口标题的辅助函数
std::string GetWindowTitle(HWND hwnd)
{
    char title[256];
    GetWindowText(hwnd, title, sizeof(title));
    return std::string(title);
}

// 获取当前鼠标焦点所在窗口
void TrackMouseFocus()
{
    POINT pt;
    GetCursorPos(&pt);

    HWND hwnd = WindowFromPoint(pt);

    if (hwnd)
    {
        HWND        hwndParent = GetAncestor(hwnd, GA_ROOTOWNER);
        std::string title      = GetWindowTitle(hwndParent);

        if (title.empty())
        {
            std::cout << "当前焦点不在任意窗口" << std::endl;
        }
        else
        {
            std::cout << "当前焦点窗口: " << title << std::endl;
        }
    }
}

// 鼠标钩子的回调函数
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        TrackMouseFocus();
    }
    return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

// 程序入口
int main()
{
    // 设置全局鼠标钩子
    g_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
    if (!g_hMouseHook)
    {
        std::cerr << "设置鼠标钩子失败!" << std::endl;
        return -1;
    }

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 卸载钩子
    UnhookWindowsHookEx(g_hMouseHook);

    return 0;
}
