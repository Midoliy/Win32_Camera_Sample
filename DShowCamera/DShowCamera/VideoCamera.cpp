#include "stdafx.h"
#include "VideoCamera.h"
#include "Qedit.h"
#include "Device.h"

using namespace std;
using namespace Func;
using namespace Struct;

namespace Model
{
    using namespace ValueObject;
    using namespace Module;

    namespace Entity
    {
        VideoCamera::VideoCamera(
            IMoniker* cameraMoniker
        ) : _name(Device::GetName(cameraMoniker))
        {
            // カメラモニカからキャプチャフィルタを作成する.
            _comSrcFilter = VideoGraph::CreateCaptureFilter(cameraMoniker);
            //  SampleGrabber フィルタを作成する.
            auto comGrabberFilter = VideoGraph::CreateGrabberFilter();

            // 解像度リストの取得.
            _comAMStream = Device::GetAMStreamConfig(_comSrcFilter);
            _videoInfos  = VideoInfo::GetVideoInfoList(_comAMStream);

            // 解像度を初期化.
            _info = _videoInfos[0];
            auto type = _info.GetType();
            SetResolution(type);

            // グラフにフィルタを追加する.
            _graph.AddFilter(_comSrcFilter, L"DeviceCaptureFilter");
            _graph.AddFilter(comGrabberFilter, L"Sample Grabber");

            // SampleGrabber フィルタから Grabber オブジェクトを作成.
            _comSampleGrabber = VideoGraph::CreateSampleGrabber(comGrabberFilter);
            {   // SampleGrabber フィルタを接続するフォーマットを指定.
                // この指定の仕方により SampleGrabber フィルタの挿入箇所が変わる.
                // 下記の指定方法だと, 画面出力の寸前でサンプルを取得するようになる.
                auto mediaType = VideoInfo::GetVideoMediaType();
                _comSampleGrabber->SetMediaType(&mediaType);
                _comSampleGrabber->SetBufferSamples(TRUE);
            }

            // レンダラーストリームを生成する.
            _graph.CreateRenderStream(&PIN_CATEGORY_CAPTURE, NULL, _comSrcFilter, NULL, comGrabberFilter);
            _graph.CreateRenderStream(&PIN_CATEGORY_PREVIEW, NULL, _comSrcFilter, NULL, NULL);

            comGrabberFilter->Release();
        }

        VideoCamera::~VideoCamera()
        {
            _comSampleGrabber->Release();
            _comAMStream->Release();
            _comSrcFilter->Release();
        }

        wstring VideoCamera::GetName()
        {
            return _name;
        }

        VDSize VideoCamera::GetSize()
        {
            return VDSize{ _info.GetWidth(), _info.GetHeight() };
        }

        double_t VideoCamera::GetFramerate()
        {
            return _info.GetFramerate();
        }

        uint32_t VideoCamera::GetCompressionId()
        {
            return _info.GetCompressionId();
        }

        vector<VideoInfo> VideoCamera::GetVideoInfos()
        {
            return _videoInfos;
        }

        shared_ptr<Image> VideoCamera::GetCurrentFrame()
        {
            HRESULT hr;

            do {
                long bufSize;
                {   // 現在のフレームのデータサイズを取得する.
                    hr = _comSampleGrabber->GetCurrentBuffer(&bufSize, nullptr);
                    if (FAILED(hr)) { break; }
                }

                // バッファサイズを調整
                unique_ptr<BYTE[]> buf { new BYTE[bufSize] };
                {   // 現在のフレームを取得
                    hr = _comSampleGrabber->GetCurrentBuffer(&bufSize, (long*)buf.get());
                    if (FAILED(hr)) { break; }
                }

                // 現在のフレームを格納するvectorのサイズを調整
                _currentFrame->Data.clear();
                _currentFrame->Data.resize(bufSize);

                // 取得した現在のフレームをメンバ変数にコピー
                memcpy(&_currentFrame->Data[0], buf.get(), bufSize);
                _currentFrame->SetSize(_info.GetWidth(), _info.GetHeight());

            } while (false);

            return _currentFrame;
        }

        HRESULT VideoCamera::SetResolution(
            AM_MEDIA_TYPE* type
        )
        {
            return _comAMStream->SetFormat(type);
        }

        HRESULT VideoCamera::BeginProjectionTo(
            Moniter* moniter
        )
        {
            auto size = VideoCamera::GetSize();
            _graph.SetOutputMoniter(moniter, &size);
            return _graph.Run();
        }

        HRESULT VideoCamera::EndProjectionTo()
        {
            return _graph.Stop();
        }
    }
}
