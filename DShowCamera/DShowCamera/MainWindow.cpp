#include "stdafx.h"
#include "MainWindow.h"
#include "Resource.h"
#include <iostream>
#include <memory>
#include <vector>

#include "dshow.h"
#include "EntryPoint.h"
#include "Func.h"
#include "Window.h"
#include "Model.Entity.h"

using namespace std;
using namespace Func;
using namespace Struct;
using namespace Model::Entity;

namespace Window
{
    const uint16_t MainWindow::kMAX_LOADSTRING = 100;
    const wchar_t* MainWindow::kSELF_NAME = L"MainWindow";

    // カメラデバイス一覧
    vector<unique_ptr<VideoCamera>> _cameras;
    int32_t                         _selectIdx = 0;

    // 各種ウィンドウハンドラ
    HWND _staticWnd = NULL;


    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="className"></param>
    /// <param name="title"></param>
    MainWindow::MainWindow(
        const HINSTANCE hInstance, 
        const wchar_t*  title
    )
    {
        _hInst = hInstance;
        _title = title;
        Register(_hInst);
    }

    /// <summary>
    /// デストラクタ
    /// </summary>
    MainWindow::~MainWindow()
    {

    }

    //
    //  関数: MyRegisterClass()
    //
    //  目的: ウィンドウ クラスを登録します。
    //
    ATOM MainWindow::Register(
        const HINSTANCE hInstance
    )
    {
        _wcex.cbSize         = sizeof(WNDCLASSEX);
        _wcex.style          = CS_HREDRAW | CS_VREDRAW;
        _wcex.lpfnWndProc    = WndProc;
        _wcex.cbClsExtra     = 0;
        _wcex.cbWndExtra     = 0;
        _wcex.hInstance      = _hInst;
        _wcex.hIcon          = LoadIcon(_hInst, MAKEINTRESOURCE(IDI_DSHOWCAMERA));
        _wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
        _wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
        _wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DSHOWCAMERA);
        _wcex.lpszClassName  = kSELF_NAME;
        _wcex.hIconSm        = LoadIcon(_wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassExW(&_wcex);
    }

    //
    //   関数: InitInstance(HINSTANCE, int)
    //
    //   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
    //
    //   コメント:
    //
    //        この関数で、グローバル変数でインスタンス ハンドルを保存し、
    //        メイン プログラム ウィンドウを作成および表示します。
    //
    BOOL MainWindow::InitInstance(
        const int nCmdShow
    )
    {
        // Formを作成.
        // 返り値のウィンドウハンドルは, Formのハンドル
        HWND hWnd = CreateWindowW(kSELF_NAME,           // クラス名
                                  _title,               // ウィンドウタイトル
                                  WS_OVERLAPPEDWINDOW,  // スタイル
                                  CW_USEDEFAULT,        // x
                                  0,                    // y
                                  800,                  // width
                                  600,                  // height
                                  nullptr,              // 親ウィンドウハンドル
                                  nullptr,              // メニュー
                                  _hInst,               // インスタンスハンドル
                                  nullptr);             // lpパラメータ

        //// Formにボタンを設置する.
        //CreateWindowW(TEXT("BUTTON"),                           // クラス名
        //              TEXT("Kitty"),                            // ボタンのキャプション名
        //              WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,    // スタイル
        //              0,                                        // x
        //              0,                                        // y
        //              100,                                      // width
        //              50,                                       // height
        //              hWnd,                                     // 親ウィンドウハンドル
        //              NULL,                                     // メニュー
        //              _hInst,                                   // インスタンスハンドル
        //              NULL);                                    // lpパラメータ

        // FormにStaticコントロールを設置する.
        _staticWnd = CreateWindowW(L"Static Control",                        // クラス名
                                   L"",                                      // ボタンのキャプション名
                                   WS_CHILD | WS_VISIBLE | SS_BLACKFRAME,    // スタイル
                                   0,                                        // x
                                   0,                                        // y
                                   640,                                      // width
                                   480,                                      // height
                                   hWnd,                                     // 親ウィンドウハンドル
                                   NULL,                                     // メニュー
                                   _hInst,                                   // インスタンスハンドル
                                   NULL);                                    // lpパラメータ


        if (!hWnd)
        {
            return FALSE;
        }

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

    //
    //  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
    //
    //  目的:    メイン ウィンドウのメッセージを処理します。
    //
    //  WM_COMMAND  - アプリケーション メニューの処理
    //  WM_PAINT    - メイン ウィンドウの描画
    //  WM_DESTROY  - 中止メッセージを表示して戻る
    //
    //
    LRESULT CALLBACK MainWindow::WndProc(
        HWND   hWnd, 
        UINT   message, 
        WPARAM wParam, 
        LPARAM lParam
    )
    {
        switch (message)
        {
        case WM_CREATE:
        {
            CoInitialize(NULL);
            // システムデバイスの列挙
            auto deviceEnum = Device::CreateEnum();

            // ビデオ入力モニカの列挙
            auto enumMoniker = Device::CreateEnumMoniker(deviceEnum);
            deviceEnum->Release();
            
            ULONG     fetched = 0;
            IMoniker* moniker;
            while (enumMoniker->Next(1, &moniker, &fetched) == S_OK)
            {   // デバイスを取得し, vectorに追加.
                _cameras.push_back(make_unique<VideoCamera>(moniker));
            }

            auto m = Moniter{ hWnd };
            _cameras[_selectIdx]->BeginProjectionTo(&m);

            moniker->Release();
            enumMoniker->Release();
            
            CoUninitialize();
            break;
        }
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_EXIT:
                return DestroyWindow(hWnd);
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
        {
            _cameras[_selectIdx]->EndProjectionTo();
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return TRUE;
    }
}