#include "stdafx.h"
#include "MainWindow.h"
#include "Resource.h"
#include <iostream>
#include <vector>

#include "dshow.h"
#include "EntryPoint.h"
#include "Func.h"
#include "Window.h"

using namespace std;
using namespace Func;
using namespace Struct;
using namespace Control;
using namespace Model::Entity;

namespace Window
{
    // カメラデバイス一覧
    vector<unique_ptr<VideoCamera>> MainWindow::_cameras;
    int32_t                         MainWindow::_selectIdx;

    // 映像出力先インスタンス
    Moniter MainWindow::_moniter;

    // ダイアログコントロール一覧
    DlgControl MainWindow::_ctrlPreview;
    DlgControl MainWindow::_ctrlStartBtn;
    DlgControl MainWindow::_ctrlStopBtn;

    /// <summary>
    /// メインダイアログを表示する.
    /// </summary>
    void MainWindow::Show()
    {
        DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, (DLGPROC)WndProc);
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
            case WM_INITDIALOG:
            {
                // 変数の初期化
                _selectIdx = 0;

                // ダイアログコントロールの初期化
                _ctrlPreview  = DlgControl{ hWnd, IDC_PREVIEW };
                _ctrlStartBtn = DlgControl{ hWnd, ID_START    };
                _ctrlStopBtn  = DlgControl{ hWnd, ID_STOP     };

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

                _moniter = Moniter{ _ctrlPreview.GetHandler() };

                moniker->Release();
                enumMoniker->Release();

                return TRUE;
            }
            case WM_COMMAND:
            {
                int wmId = LOWORD(wParam);
                // 選択されたメニューの解析:
                switch (wmId)
                {
                    case ID_START:
                    {
                        _cameras[_selectIdx]->BeginProjectionTo(&_moniter);
                        return TRUE;
                    }
                    case ID_STOP:
                    {
                        _cameras[_selectIdx]->EndProjectionTo();
                        return TRUE;
                    }
                    default:
                    {
                        return DefWindowProc(hWnd, message, wParam, lParam);
                    }
                }
            }
            case WM_DESTROY:
            {
                _cameras[_selectIdx]->EndProjectionTo();
                PostQuitMessage(0);
                return TRUE;
            }
            default:
            {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }        
    }
}