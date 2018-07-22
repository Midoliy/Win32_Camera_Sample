#include "stdafx.h"
#include "CameraDev.h"
#include <dshow.h>
#include <atlstr.h>  
#include <memory>

using namespace camera;

CameraDev::CameraDev()
{
	/*if (FAILED(CoInitialize(NULL)))
	{
		throw std::exception("COMイニシャライズに失敗しました.");
	}*/
}

CameraDev::~CameraDev()
{
	//CoUninitialize();
}
//
//PTCHAR CameraDev::GetName()
//{
//	return CameraDev::name;
//}
//
//std::vector<Resolution> CameraDev::GetResolution()
//{
//	return CameraDev::res;
//}
//
//std::unique_ptr<ICreateDevEnum> createDevEnum()
//{
//	auto pDevEnum = std::make_unique<ICreateDevEnum>();
//	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&pDevEnum);
//}