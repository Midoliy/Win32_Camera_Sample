#include "stdafx.h"
#include "VideoInfo.h"

namespace Model
{
    namespace ValueObject
    {
        using namespace std;
        const double_t VideoInfo::k100NanoSec = (100 * 1.0e-9);

        /// <summary>
        /// コンストラクタ
        /// </summary>
        VideoInfo::VideoInfo()
        {
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="videoInfo">ビデオヘッダ情報</param>
        VideoInfo::VideoInfo(
            const VIDEOINFOHEADER* videoInfo
            , AM_MEDIA_TYPE*   type
        ) : _framerate(1 / (double_t(videoInfo->AvgTimePerFrame) * k100NanoSec))
            , _width(videoInfo->bmiHeader.biWidth)
            , _height(videoInfo->bmiHeader.biHeight)
            , _compressionId(videoInfo->bmiHeader.biCompression)
            , _type(type)
        {
        }

        /// <summary>
        /// デストラクタ
        /// </summary>
        VideoInfo::~VideoInfo()
        {
        }

        /// <summary>
        /// 映像フレームレート
        /// </summary>
        /// <returns>fps</returns>
        double_t VideoInfo::GetFramerate()
        {
            return _framerate;
        }

        /// <summary>
        /// 映像の幅
        /// </summary>
        /// <returns>px</returns>
        uint32_t VideoInfo::GetWidth()
        {
            return _width;
        }

        /// <summary>
        /// 映像の高さ
        /// </summary>
        /// <returns>px</returns>
        uint32_t VideoInfo::GetHeight()
        {
            return _height;
        }

        /// <summary>
        /// 映像圧縮コーデック
        /// </summary>
        /// <returns>FOURCCコード</returns>
        uint32_t VideoInfo::GetCompressionId()
        {
            return _compressionId;
        }

        /// <summary>
        /// メディアタイプ情報
        /// </summary>
        /// <returns>メディアタイプ情報</returns>
        AM_MEDIA_TYPE* VideoInfo::GetType()
        {
            return _type;
        }

        /// <summary>
        /// Webカメラ情報から対応している解像度情報を取得する.
        /// </summary>
        /// <param name="streamConfig">Webカメラ情報</param>
        /// <returns>対応している解像度情報リスト</returns>
        vector<VideoInfo> VideoInfo::GetVideoInfoList(
            IAMStreamConfig* streamConfig
        )
        {
            int_fast32_t count = 0, size = 0;
            streamConfig->GetNumberOfCapabilities(&count, &size);

            vector<VideoInfo> list;
            if (size != sizeof(VIDEO_STREAM_CONFIG_CAPS))
            {
                return list;
            }

            for (int_fast16_t i = 0; i < count; i++)
            {
                VIDEO_STREAM_CONFIG_CAPS scc;
                AM_MEDIA_TYPE*           type;
                streamConfig->GetStreamCaps(i, &type, (byte*)&scc);

                if (type->majortype == MEDIATYPE_Video
                    && type->formattype == FORMAT_VideoInfo
                    && type->cbFormat >= sizeof(VIDEOINFOHEADER)
                    && type->pbFormat != NULL)
                {
                    auto info = (VIDEOINFOHEADER*)type->pbFormat;
                    auto vinfo = VideoInfo(info, type);
                    list.push_back(vinfo);
                }
            }

            return list;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        AM_MEDIA_TYPE VideoInfo::GetVideoMediaType()
        {
            AM_MEDIA_TYPE mediaType;
            {
                ZeroMemory(&mediaType, sizeof(mediaType));
                mediaType.majortype  = MEDIATYPE_Video;
                mediaType.subtype    = MEDIASUBTYPE_RGB32;
                mediaType.formattype = FORMAT_VideoInfo;
            }
            return mediaType;
        }
    }
}