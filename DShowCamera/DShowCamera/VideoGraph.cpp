#include "stdafx.h"
#include "VideoGraph.h"
#include <vcruntime_exception.h>

using namespace std;
using namespace Struct;
using namespace Model::Entity;

namespace Model
{
    namespace Module
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        VideoGraph::VideoGraph()
        {
            // Graph ビルダの生成.
            _comGraphBuilder = VideoGraph::CreateGraphBuilder();
            // CaptureGraph ビルダを生成.
            _comCaptureGraphBuilder = VideoGraph::CreateCaptureGraphBuilder(_comGraphBuilder);
            // graph 操作用の MediaControlを取得する.
            _comGraphBuilder->QueryInterface<IMediaControl>(&_comMediaControl);
            // 通常出力の出力先設定用COMを生成する.
            _comVideoWindow = CreateVideoWindow(_comGraphBuilder);
        }

        /// <summary>
        /// デストラクタ
        /// </summary>
        VideoGraph::~VideoGraph()
        {
            _comVideoWindow->Release();
            _comMediaControl->Release();
            _comCaptureGraphBuilder->Release();
            _comGraphBuilder->Release();
        }

        #pragma region static メソッド
        /// <summary>
        /// [COM] GraphBuilderを作成する.
        /// </summary>
        /// <returns>GraphBuilderインスタンス</returns>
        IGraphBuilder* VideoGraph::CreateGraphBuilder()
        {
            IGraphBuilder* comGraphBuilder = nullptr;
            {   // Graph ビルダの生成.
                auto hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID*)&comGraphBuilder);
                if (hr != S_OK) { std::exception(hr); }
            }
            return comGraphBuilder;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="comGraphBuilder"></param>
        /// <returns></returns>
        ICaptureGraphBuilder2* VideoGraph::CreateCaptureGraphBuilder(
            IGraphBuilder* comGraphBuilder
        )
        {
            ICaptureGraphBuilder2* comCaptureGraphBuilder = nullptr;
            {   // CaptureGraph ビルダを生成.
                auto hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (LPVOID*)&comCaptureGraphBuilder);
                if (hr != S_OK) { std::exception(hr); }

                // graph ビルダを captureGraph ビルダにセットする.
                hr = comCaptureGraphBuilder->SetFiltergraph(comGraphBuilder);
                if (hr != S_OK) { std::exception(hr); }
            }
            return comCaptureGraphBuilder;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="deviceMoniker"></param>
        /// <returns></returns>
        IBaseFilter* VideoGraph::CreateCaptureFilter(
            IMoniker* deviceMoniker
        )
        {
            IBaseFilter* comCaptureFilter = nullptr;
            {   // MonikerをキャプチャフィルタにBindする.
                auto hr = deviceMoniker->BindToObject(0, 0, IID_IBaseFilter, (LPVOID*)&comCaptureFilter);
                if (hr != S_OK) { std::exception(hr); }
            }
            return comCaptureFilter;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        IBaseFilter* VideoGraph::CreateGrabberFilter()
        {
            IBaseFilter* comGrabberFilter = nullptr;
            {   // SampleGrabber フィルタの生成.
                auto hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID*)&comGrabberFilter);
                if (hr != S_OK) { std::exception(hr); }
            }
            return comGrabberFilter;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="grabberFilter"></param>
        /// <returns></returns>
        ISampleGrabber* VideoGraph::CreateSampleGrabber(
            IBaseFilter* grabberFilter
        )
        {
            ISampleGrabber* comSampleGrabber = nullptr;
            {   // SampleGrabber フィルタから Grabber オブジェクトを取得.
                auto hr = grabberFilter->QueryInterface<ISampleGrabber>(&comSampleGrabber);
                if (hr != S_OK) { std::exception(hr); }
            }
            return comSampleGrabber;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="graphBuilder"></param>
        /// <returns></returns>
        IVideoWindow* VideoGraph::CreateVideoWindow(
            IGraphBuilder* graphBuilder
        )
        {
            IVideoWindow* comVideoWindow = nullptr;
            {
                auto hr = graphBuilder->QueryInterface(IID_IVideoWindow, (LPVOID*)&comVideoWindow);
                if (hr != S_OK) { std::exception(hr); }
            }
            return comVideoWindow;
        }
        #pragma endregion

        #pragma region メンバメソッド    
        /// <summary>
        /// グラフにフィルタを追加する.
        /// </summary>
        /// <param name="filter">追加するフィルタ</param>
        /// <param name="filterName">追加するフィルタの名前</param>
        /// <returns>追加結果</returns>
        HRESULT VideoGraph::AddFilter(
            IBaseFilter* filter,
            wstring      filterName
        )
        {
            return _comGraphBuilder->AddFilter(filter, filterName.c_str());
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="moniter"></param>
        /// <param name="videoSize"></param>
        /// <returns></returns>
        HRESULT VideoGraph::SetOutputMoniter(
            Moniter* moniter, 
            VDSize*  videoSize
        )
        {
            // 描画先ウィンドウの設定
            // 出力先モニタサイズを算出する.
            auto moniterRect   = moniter->GetRect();
            auto moniterWidth  = moniterRect.right  - moniterRect.left;
            auto moniterHeight = moniterRect.bottom - moniterRect.top;
            
            // 入力映像を出力先モニタサイズに合わせるための倍率を算出する.
            auto w = (double_t)moniterWidth  / videoSize->Width;
            auto h = (double_t)moniterHeight / videoSize->Height;
            auto ratio = w > h ? h : w;

            // 出力映像のサイズと出力位置を算出する.
            auto width  = (int32_t)(videoSize->Width  * ratio + 0.5);
            auto height = (int32_t)(videoSize->Height * ratio + 0.5);
            auto x = (moniterWidth  - width ) / 2;
            auto y = (moniterHeight - height) / 2;

            // 出力先への設定.
            HRESULT hr;
            hr = _comVideoWindow->put_Owner((OAHWND)moniter->GetHandler());
            if (hr != S_OK) { std::exception(hr); }
            hr = _comVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
            if (hr != S_OK) { std::exception(hr); }
            hr = _comVideoWindow->SetWindowPosition(x, y, width, height);
            if (hr != S_OK) { std::exception(hr); }
            hr = _comVideoWindow->put_Visible(OATRUE);
            if (hr != S_OK) { std::exception(hr); }

            return hr;
        }

        /// <summary>
        /// グラフを生成する.
        /// </summary>
        /// <param name="category"></param>
        /// <param name="type"></param>
        /// <param name="source"></param>
        /// <param name="compressor"></param>
        /// <param name="renderer"></param>
        /// <returns></returns>
        HRESULT VideoGraph::CreateRenderStream(
            const GUID*  category,
            const GUID*  type,
            IUnknown*    source,
            IBaseFilter* compressor,
            IBaseFilter* renderer
        )
        {
            return _comCaptureGraphBuilder->RenderStream(category, type, source, compressor, renderer);
        }

        /// <summary>
        /// 入力された映像を出力する.
        /// </summary>
        /// <returns></returns>
        HRESULT VideoGraph::Run()
        {
            return _comMediaControl->Run();
        }

        /// <summary>
        /// 入力された映像の出力を停止する.
        /// </summary>
        /// <returns></returns>
        HRESULT VideoGraph::Stop()
        {
            return _comMediaControl->Stop();
        }
        #pragma endregion
    }
}