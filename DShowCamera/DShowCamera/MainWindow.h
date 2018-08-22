#pragma once
#include <memory>
#include "stdafx.h"
#include "stdint.h"
#include "DlgControl.h"
#include "Model.Entity.h"


namespace Window
{
    class MainWindow
    {
    public:        
        void Show();

    private:
        static Model::Entity::Moniter _moniter;
        static Control::DlgControl    _ctrlPreview;
        static Control::DlgControl    _ctrlStartBtn;
        static Control::DlgControl    _ctrlStopBtn;

        static std::vector<std::unique_ptr<Model::Entity::VideoCamera>> _cameras;
        static int32_t                _selectIdx;



        static LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    };
}