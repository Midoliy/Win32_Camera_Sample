#pragma once
#include <string>
#include "Qedit.h"
#include "dshow.h"
#include "Moniter.h"
#include "VDSize.h"

namespace Model
{
    using namespace Entity;
    using namespace Struct;

    namespace Module
    {
        class VideoGraph
        {
        public:
            static IGraphBuilder*         CreateGraphBuilder();
            static ICaptureGraphBuilder2* CreateCaptureGraphBuilder(IGraphBuilder* comGraphBuilder);
            static IBaseFilter*           CreateCaptureFilter(IMoniker* deviceMoniker);
            static IBaseFilter*           CreateGrabberFilter();
            static ISampleGrabber*        CreateSampleGrabber(IBaseFilter* grabberFilter);
            static IVideoWindow*          CreateVideoWindow(IGraphBuilder* graphBuilder);

            HRESULT AddFilter(IBaseFilter* filter, std::wstring filterName);
            HRESULT SetOutputMoniter(Moniter* moniter, VDSize* videoSize);
            HRESULT CreateRenderStream(const GUID* category, const GUID* type, IUnknown* source, IBaseFilter* compressor, IBaseFilter* renderer);
            HRESULT Run();
            HRESULT Stop();

            VideoGraph();
            ~VideoGraph();

        private:
            IGraphBuilder*         _comGraphBuilder;
            ICaptureGraphBuilder2* _comCaptureGraphBuilder;
            IMediaControl*         _comMediaControl;
            IVideoWindow*          _comVideoWindow;
        };
    }
}
