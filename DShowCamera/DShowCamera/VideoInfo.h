#pragma once
#include <iostream>
#include <vector>
#include "dshow.h"

namespace Model
{
    namespace ValueObject
    {
        class VideoInfo
        {
        public:
            std::double_t        GetFramerate();
            std::uint32_t        GetWidth();
            std::uint32_t        GetHeight();
            std::uint32_t        GetCompressionId();
            AM_MEDIA_TYPE*       GetType();

            VideoInfo();
            VideoInfo(const VIDEOINFOHEADER* videoInfo, AM_MEDIA_TYPE* type);
            ~VideoInfo();

            static std::vector<VideoInfo> GetVideoInfoList(IAMStreamConfig* streamConfig);
            static AM_MEDIA_TYPE          GetVideoMediaType();

        private:
            std::double_t  _framerate;
            std::uint32_t  _width;
            std::uint32_t  _height;
            std::uint32_t  _compressionId;
            AM_MEDIA_TYPE* _type;

            static const double_t k100NanoSec;
        };
    }
}