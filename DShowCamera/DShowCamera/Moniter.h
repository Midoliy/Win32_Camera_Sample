#pragma once
#include <memory>
#include "Image.h"

namespace Model
{
    namespace Entity
    {
        class Moniter
        {
        public:
            HWND    GetHandler();
            RECT    GetRect();
            HRESULT InputImage(std::shared_ptr<Image> img);
            HRESULT OutpuImage();
            BOOL    UpdateWindowRect();

            Moniter();
            Moniter(HWND hWnd);

        private:
            HWND _hWnd;
            HDC  _devContext;
            HDC  _memContext;

            RECT _rect;
            std::unique_ptr<Image> _img;
        };
    }
}