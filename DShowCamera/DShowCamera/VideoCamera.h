#pragma once
#include <string>
#include <vector>
#include <memory>
#include "VideoInfo.h"
#include "VideoGraph.h"
#include "VDSize.h"
#include "Image.h"
#include "Moniter.h"

namespace Model
{
    namespace Entity
    {
        class VideoCamera
        {
        public:
            std::wstring                        GetName();
            Struct::VDSize                      GetSize();
            std::double_t                       GetFramerate();
            std::uint32_t                       GetCompressionId();
            std::vector<ValueObject::VideoInfo> GetVideoInfos(); 
            std::shared_ptr<Image>              GetCurrentFrame();
            HRESULT                             SetResolution(AM_MEDIA_TYPE* type);
            HRESULT                             BeginProjectionTo(Moniter* moniter);
            HRESULT                             EndProjectionTo();          

            VideoCamera(IMoniker* cameraMoniker);
            ~VideoCamera();

        private:
            std::wstring                        _name;
            ValueObject::VideoInfo              _info;
            std::vector<ValueObject::VideoInfo> _videoInfos;
            Module::VideoGraph                  _graph;
            std::shared_ptr<Image>              _currentFrame;

            IBaseFilter*                        _comSrcFilter;
            IAMStreamConfig*                    _comAMStream;
            ISampleGrabber*                     _comSampleGrabber;
        };
    }
}