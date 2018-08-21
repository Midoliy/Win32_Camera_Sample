#include "stdafx.h"
#include "CameraDev.h"
#include <dshow.h>
#include <atlstr.h>  

namespace camera {

	CameraDev::CameraDev()
	{
		if (FAILED(CoInitialize(NULL)))
		{
			throw std::exception("COMイニシャライズに失敗しました.");
		}
	}

	CameraDev::~CameraDev()
	{
		CoUninitialize();
	}

	std::wstring CameraDev::GetName()
	{
		return CameraDev::name;
	}

	std::vector<Resolution> CameraDev::GetResolution()
	{
		return this->resolutions;
	}

	std::unique_ptr<ICreateDevEnum> CameraDev::createDevEnum()
	{
		CComPtr<ICreateDevEnum> dev_enum;
		CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&dev_enum);

		CComPtr<IEnumMoniker> enum_moniker;
		dev_enum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enum_moniker, 0);

		ULONG			     fetched = 0;
		CComPtr<IMoniker>	 moniker;
		CComPtr<IBaseFilter> src;

		while (enum_moniker->Next(1, &moniker, &fetched) == S_OK)
		{
			CComPtr<IPropertyBag> prop;
			VARIANT var;

			moniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&prop);
			var.vt = VT_BSTR;
			prop->Read(L"FriendlyName", &var, 0);

			// デバイス名を取得
			std::wstring name(var.bstrVal, SysStringLen(var.bstrVal));
			this->name = name;
			memcpy(this->moniker, moniker, sizeof(moniker));
		}

		auto pDevEnum = std::make_unique<ICreateDevEnum>();
		CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&pDevEnum);
	}
}