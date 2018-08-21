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
            
            Moniter(HWND hWnd);

        private:
            const HWND _hWnd;
            const HDC  _devContext;
            const HDC  _memContext;

            RECT _rect;
            std::unique_ptr<Image> _img;
        };
    }
}