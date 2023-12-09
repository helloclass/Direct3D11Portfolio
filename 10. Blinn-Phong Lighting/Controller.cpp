#include "Controller.h"

Controller Controller::instance;

void Controller::SetWindow(const HWND* hwnd)
{
    MAIN_SCREEN_HWMD = const_cast<HWND*>(hwnd);

    GetCursorPos(&PreviousPoint);
    ScreenToClient(*MAIN_SCREEN_HWMD, &PreviousPoint);
}

void Controller::Update()
{
    if (!MAIN_SCREEN_HWMD)
        return;

    POINT ScreenPoint;

    GetCursorPos(&ScreenPoint);
    ScreenToClient(*MAIN_SCREEN_HWMD, &ScreenPoint);

    DeltaPoint.x = ScreenPoint.x - PreviousPoint.x;
    DeltaPoint.y = ScreenPoint.y - PreviousPoint.y;

    PreviousPoint = ScreenPoint;
}
