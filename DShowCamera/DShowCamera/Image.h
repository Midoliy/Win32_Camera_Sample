#pragma once
#include "stdafx.h"
#include "VDSize.h"
#include <vector>

namespace Model
{
    namespace Entity
    {
        class Image
        {
        public:
            std::vector<BYTE> Data;

            const Struct::VDSize*   Size();
            void              SetSize(uint32_t width, uint32_t height);
            BITMAPINFOHEADER* GetBmpInfoHeader();
            BITMAPINFO*       GetBmpInfo();

            Image();
            ~Image();

        private:
            Struct::VDSize   _size;
            BITMAPINFOHEADER _bmpInfoHeader;
            BITMAPINFO       _bmpInfo;
        };
    }
}