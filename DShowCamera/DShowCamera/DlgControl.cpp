#include "stdafx.h"
#include "DlgControl.h"

namespace Control
{
    DlgControl::DlgControl()
        : _parent(NULL)
        , _id    (NULL)
        , _self  (NULL)
    {
    }

    DlgControl::DlgControl(
        HWND    parent,
        int32_t id
    ) : _parent(parent)
        , _id  (id)
        , _self(GetDlgItem(parent, id))
    {
    }

    RECT DlgControl::GetRect()
    {
        RECT rect;
        GetWindowRect(_self, &rect);
        return rect;
    }

    HWND DlgControl::GetHandler()
    {
        return _self;
    }
}