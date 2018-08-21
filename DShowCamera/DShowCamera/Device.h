#pragma once
#include "dshow.h"
#include <string>

namespace Func
{
    class Device
    {
    public:
        static ICreateDevEnum*  CreateEnum();
        static IEnumMoniker*    CreateEnumMoniker(ICreateDevEnum* deviceEnum);
        static IAMStreamConfig* GetAMStreamConfig(IBaseFilter* captureFilter);
        static std::wstring     GetName(IMoniker* moniker);
    };
}