#include "stdafx.h"
#include "Device.h"

using namespace std;

namespace Func
{
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    ICreateDevEnum* Device::CreateEnum()
    {
        ICreateDevEnum* deviceEnum;
        CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&deviceEnum);
        return deviceEnum;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="deviceEnum"></param>
    /// <returns></returns>
    IEnumMoniker* Device::CreateEnumMoniker(ICreateDevEnum* deviceEnum)
    {
        IEnumMoniker* enumMoniker;
        deviceEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enumMoniker, 0);
        return enumMoniker;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="moniker"></param>
    /// <returns></returns>
    IAMStreamConfig* Device::GetAMStreamConfig(IBaseFilter* captureFilter)
    {
        ICaptureGraphBuilder2* captureBulider;
        {
            CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void**)&captureBulider);
        }

        IAMStreamConfig* config;
        {
            captureBulider->FindInterface(&PIN_CATEGORY_CAPTURE, 0, captureFilter, IID_IAMStreamConfig, (void**)&config);
            captureBulider->Release();
        }
        return config;
    }
    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="moniker"></param>
    /// <returns></returns>
    wstring Device::GetName(IMoniker* moniker)
    {
        IPropertyBag* prop;
        {
            moniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&prop);
        }

        VARIANT var;
        {
            var.vt = VT_BSTR;
            prop->Read(L"FriendlyName", &var, 0);
            prop->Release();
        }

        auto name = wstring(var.bstrVal, SysStringLen(var.bstrVal));
        
        return name;
    }

}