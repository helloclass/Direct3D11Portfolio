#include "Camera.h"

CameraPool CameraPool::gCameraPool;

void Camera::Update()
{
    DeltaTime = GlobalTime::getInstance().dt;

    // Calc Delta Mouse Point
    HandleUpdate();

    if (GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamFwd])
        MoveLocalForward();
    if (GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamBack])
        MoveLocalBackward();
    if (GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamLeft])
        MoveLocalLeft();
    if (GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamRight])
        MoveLocalRight();
    if (GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionRaiseCam])
        MoveLocalUp();
    if (GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionLowerCam])
        MoveLocalDown();

    if (IsMouseRClicked)
    {
        POINT DeltaPoint = Controller::getInstance().DeltaPoint;

        GlobalPitch(-Controller::getInstance().DeltaPoint.y);
        LocalYaw(-Controller::getInstance().DeltaPoint.x);
    }
}

void Camera::HandleUpdate()
{
     if (GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionRightDown])
    {
         GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionRightDown] = false;
        IsMouseRClicked = true;
    }
    if (GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionRightUp])
    {
        GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionRightUp] = false;
        IsMouseRClicked = false;
    }
}