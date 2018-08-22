#pragma once

namespace Control
{
    class DlgControl
    {
    public:
        RECT    GetRect();
        HWND    GetHandler();

        DlgControl();
        DlgControl(HWND parent, int32_t id);

    private:
        HWND    _parent;
        HWND    _self;
        int32_t _id;
    };
}