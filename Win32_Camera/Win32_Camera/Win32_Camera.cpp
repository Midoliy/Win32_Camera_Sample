// Win32_Camera.cpp: アプリケーションのエントリ ポイントを定義します。
//

#pragma comment(lib,"Strmiids.lib") 

#include "stdafx.h"
#include "Win32_Camera.h"
#include <dshow.h>
#include "Camera.h"
#include <memory>
#include <string>
#include <comip.h>

#define MAX_LOADSTRING 100

typedef _com_ptr_t <_com_IIID<ICreateDevEnum, &__uuidof(ICreateDevEnum)>> ICreateDevEnumPtr;
typedef _com_ptr_t <_com_IIID<IEnumMoniker, &__uuidof(IEnumMoniker)>> IEnumMonikerPtr;

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。
	
	// COMの初期化
	// - COMの初期化を行わないと, 以降のCOM操作が実行できないため注意.
	if (FAILED(CoInitialize(NULL)))
	{
		MessageBox(NULL, L"COMの初期化に失敗しました", L"ERROR", MB_OK);
		return -1;
	}
	
	// 1. フィルタグラフを作成
	IGraphBuilder *pGraph = NULL;
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&pGraph);

	// 2. システムデバイス列挙子を作成.
	ICreateDevEnum *pDevEnum = NULL;
	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&pDevEnum);

	IEnumMoniker *pClassEnum = NULL;
	pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pClassEnum, 0);

	ULONG		 cFetched = 0;
	IMoniker	*pMoniker = NULL;
	IBaseFilter *pSrc	  = NULL;

	if (pClassEnum->Next(1, &pMoniker, &cFetched) == S_OK)
	{
		// 最初のモニカをフィルタオブジェクトにバインドする
		pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&pSrc);
		pMoniker->Release();
	}
	pClassEnum->Release();
	pDevEnum->Release();

	pGraph->AddFilter(pSrc, L"Video Capture");

	// 3. キャプチャビルダの作成.
	ICaptureGraphBuilder2 *pBuilder = NULL;
	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&pBuilder);

	// 4. ファイルライタフィルタの設定
	IBaseFilter		*pMux  = NULL;
	IFileSinkFilter *pSink = NULL;

	// ファイルへ
	pBuilder->SetOutputFileName(&MEDIASUBTYPE_Avi, L"C:\\src\\Win32_Camera_Sample\\output\\sample.avi", &pMux, &pSink);
	pBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSrc, NULL, pMux);
	REFERENCE_TIME rtStart = 50000000, rtStop = 80000000;
	pBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSrc, &rtStart, &rtStop, 0, 0);

	// ディスプレイへ
	pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pSrc, NULL, NULL);


	// 5. キャプチャ開始
	IMediaControl *pMediaControl;
	pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
	pMediaControl->Run();

	MessageBox(NULL, L"Click to stop.", L"DirectShow", MB_OK);

	// 6. 終了処理
	pSrc->Release();
	pMux->Release();
	pSink->Release();
	pMediaControl->Release();
	pBuilder->Release();
	pGraph->Release();
	CoUninitialize();



	//// システムデバイス列挙子の作成
	//ICreateDevEnum *pDevEnum = NULL;
	////ICreateDevEnum *pDevEnum = NULL;
	//CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&pDevEnum);
	//
	//// 列挙子の作成
	//IEnumMoniker *pClassEnum = NULL;
	//pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
	//if (pClassEnum == NULL)
	//{
	//	pDevEnum->Release();
	//	return -1;
	//}

	//ULONG        cFetched;
	//IMoniker    *pMoniker = NULL;
	//IBaseFilter *pbf      = NULL;
	//int          n		  = 0;
	//
	//while (pClassEnum->Next(1, &pMoniker, &cFetched) == S_OK)
	//{
	//	IPropertyBag *pP = NULL;
	//	VARIANT var;

	//	pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pP);
	//	var.vt = VT_BSTR;
	//	pP->Read(L"FriendlyName", &var, 0);

	//	// デバイス名を取得
	//	auto c = std::make_shared<Camera>();
	//	_tcscpy_s(c->name, sizeof(c->name)/sizeof(TCHAR), var.bstrVal);
	//	VariantClear(&var);

	//	// モニカをフィルタオブジェクトにバインド
	//	pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&pbf);
	//	

	//	// キャプチャグラフ作成
	//	ICaptureGraphBuilder2 *pCapture = NULL;
	//	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&pCapture);

	//	// ビデオフォーマットの取得
	//	IAMStreamConfig *pConfig = NULL;
	//	HRESULT hr = pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, 0, pbf, IID_IAMStreamConfig, (void**)&pConfig);	// インターフェースのポインタを取得

	//	int iCount = 0;
	//	int iSize  = 0;
	//	hr = pConfig->GetNumberOfCapabilities(&iCount, &iSize);

	//	if (iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))	// VIDEO_STREAM_CONFIG_CAPS構造体かサイズを確認
	//	{
	//		for (int iFormat = 0; iFormat < iCount; iFormat++)
	//		{
	//			VIDEO_STREAM_CONFIG_CAPS scc;
	//			AM_MEDIA_TYPE			*pmtConfig;
	//			hr = pConfig->GetStreamCaps(iFormat, &pmtConfig, (BYTE*)&scc);

	//			if (FAILED(hr))
	//			{
	//				continue;
	//			}

	//			if ((pmtConfig->majortype  == MEDIATYPE_Video)
	//			 && (pmtConfig->formattype == FORMAT_VideoInfo)
	//			 && (pmtConfig->cbFormat   >= sizeof(VIDEOINFOHEADER))
	//			 && (pmtConfig->pbFormat   != NULL))
	//			{
	//				VIDEOINFOHEADER  *pVih2 = (VIDEOINFOHEADER  *) pmtConfig->pbFormat;

	//				double ns		 = 100 * 1.0e-9;
	//				double frame	 = 1 / (double(pVih2->AvgTimePerFrame)*ns);
	//				auto   r		 = std::make_shared<Res>();
	//					   r->frame  = frame;
	//					   r->width  = pVih2->bmiHeader.biWidth;
	//					   r->height = pVih2->bmiHeader.biHeight;
	//				c->res.push_back(*r);
	//			}
	//		}
	//	}

	//	// リソースの開放
	//	pConfig->Release();
	//	pCapture->Release();

	//	std::string reses = "";
	//	for(auto res : c->res)
	//	{
	//		auto frame  = "frame : " + std::to_string(res.frame) + ", ";
	//		auto width  = "width : " + std::to_string(res.width) + ", ";
	//		auto height = "height: " + std::to_string(res.height) + "\n";

	//		reses.append(frame + width + height);
	//	}

	//	std::wstring stemp = std::wstring(reses.begin(), reses.end());
	//	MessageBox(NULL, stemp.c_str(), c->name, MB_OK);

	//	++n;
	//}

	//pDevEnum->Release();
	//pClassEnum->Release();



	CoUninitialize();








    // グローバル文字列を初期化しています。
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32CAMERA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーションの初期化を実行します:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32CAMERA));

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



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32CAMERA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32CAMERA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
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
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
