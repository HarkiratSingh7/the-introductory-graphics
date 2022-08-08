#ifndef WINDOWSSCREEN_H
#define WINDOWSSCREEN_H

#include "windows.h"
#include <string>
#include <functional>

// Note: Only 1 WindowsScreen per Graphics Application
void Draw();

class WindowsScreen
{
public:
    WindowsScreen(int width, int height, std::string appName, HINSTANCE hInstance)
        : Width(width), Height(height), AppName(appName)
    {
        WNDCLASS wndClass;
        wndClass.style = CS_HREDRAW | CS_VREDRAW;
        wndClass.lpfnWndProc = WndProc;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = hInstance;
        wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = appName.c_str();

        if (!RegisterClass(&wndClass))
        {
            MessageBox(NULL, "Failed to register class.", "Error", MB_OK | MB_ICONERROR);
            exit(0);
        }
        int titleHeight = GetSystemMetrics(SM_CYCAPTION);
        hWnd = CreateWindow(appName.c_str(),
                            appName.c_str(),
                            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            Width,
                            Height + titleHeight,
                            NULL, NULL, hInstance, NULL);

        if (!hWnd)
        {
            MessageBox(NULL, "Failed to create window", "Error", MB_OK | MB_ICONERROR);
            exit(0);
        }

        hdc = GetDC(hWnd);
    }

    ~WindowsScreen()
    {
        ReleaseDC(hWnd, hdc);
    }

    void Show(int iCmdShow)
    {
        ShowWindow(hWnd, iCmdShow);
        UpdateWindow(hWnd);

        MSG msg;

        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_PAINT:
            Draw();
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

private:
    int Width{0}, Height{0};
    std::string AppName;
    HDC hdc{nullptr};
    HWND hWnd{nullptr};
    friend class Graphics;
};

#endif