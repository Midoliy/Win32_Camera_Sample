#include "stdafx.h"
#include "Moniter.h"

using namespace Struct;
using namespace std;

namespace Model
{
    namespace Entity
    {

        Moniter::Moniter(
            HWND hWnd
        ) : _hWnd(hWnd)
          , _devContext(GetDC(_hWnd))
          , _memContext(CreateCompatibleDC(_devContext))
        {
            // ウィンドウサイズを取得する.
            GetWindowRect(_hWnd, &_rect);
        }

        HWND Moniter::GetHandler()
        {
            return _hWnd;
        }

        RECT Moniter::GetRect()
        {
            return _rect;
        }

        HRESULT Moniter::InputImage(
            shared_ptr<Image> img
        )
        {
            if (img == nullptr)
            {
                return E_POINTER;
            }

            if (img->Size()->Width == 0 || img->Size()->Height == 0)
            {
                return E_INVALIDARG;
            }

            return E_FAIL;

        }

        HRESULT Moniter::OutpuImage()
        {
            return E_FAIL;
        }

        BOOL Moniter::UpdateWindowRect()
        {
            // ウィンドウサイズを再取得する.
            return GetWindowRect(_hWnd, &_rect);
        }
    }
}