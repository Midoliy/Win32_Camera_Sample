#pragma once

#include "stdafx.h"
#include "stdint.h"

namespace Window
{
    class MainWindow
    {
    public:        
        BOOL InitInstance(const int nCmdShow);

        MainWindow(const HINSTANCE hInstance, const wchar_t *title);
        ~MainWindow();

    private:
        ATOM Register(const HINSTANCE hInstance);
        static LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        static const uint16_t kMAX_LOADSTRING;
        static const wchar_t* kSELF_NAME;


        HINSTANCE      _hInst;
        WNDCLASSEXW    _wcex;
        const wchar_t* _title;
    };
}