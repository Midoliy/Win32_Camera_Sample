#include "stdafx.h"
#include "Image.h"

using namespace Struct;

namespace Model
{
    namespace Entity
    {
        Image::Image()
        {
            // _bmpInfoHeaderの初期化
            ZeroMemory(&_bmpInfoHeader, sizeof(BITMAPINFOHEADER));
            _bmpInfoHeader.biSize     = sizeof(BITMAPINFOHEADER);
            _bmpInfoHeader.biWidth    = 0;
            _bmpInfoHeader.biHeight   = 0;
            _bmpInfoHeader.biPlanes   = 1;
            _bmpInfoHeader.biBitCount = 32;
            
            // _bmpInfoの初期化
            _bmpInfo.bmiHeader = _bmpInfoHeader;

        }

        Image::~Image()
        {

        }

        const VDSize* Image::Size()
        {
            return &_size;
        }

        void Image::SetSize(
            uint32_t width,
            uint32_t height)
        {
            _size = VDSize{ width, height };
            
            // BITMAPINFOの更新
            _bmpInfoHeader.biWidth  = width;
            _bmpInfoHeader.biHeight = height;
            _bmpInfo.bmiHeader = _bmpInfoHeader;
        }

        BITMAPINFOHEADER* Image::GetBmpInfoHeader()
        {
            return &_bmpInfoHeader;
        }

        BITMAPINFO* Image::GetBmpInfo()
        {
            return &_bmpInfo;
        }
    }
}