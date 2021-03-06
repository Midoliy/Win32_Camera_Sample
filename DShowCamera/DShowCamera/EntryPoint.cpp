// DShowCamera.cpp: アプリケーションのエントリ ポイントを定義します。
//

#include <atlbase.h>
#include <vector>
#include <string>
#include <memory>

#include "stdafx.h"
#include "Resource.h"
#include "MainWindow.h"

using namespace Window;


/// <summary>
/// エントリポイント
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。    

   
    CoInitialize(NULL);
    {   // MainWindowを生成.
        MainWindow mainWindow;
        mainWindow.Show();
    }
    CoUninitialize();


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DSHOWCAMERA));
    MSG msg;
    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    

    return (int) msg.wParam;
}

