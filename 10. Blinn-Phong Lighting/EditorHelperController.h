#pragma once
#include "Camera.h"
#include "PhysicsObjectData.h"

class EditorHelperController
{
private:
    bool bIsSelected = false;

    bool bGlobalPivot = false;

	bool bIsGrab = false;
	bool bIsRotate = false;

	bool bIsAxisLockX = false;
	bool bIsAxisLockY = false;
	bool bIsAxisLockZ = false;

    POINT MouseOriginPoint;

    // ���콺�� ���� ��ġ�� ���� �󸶳� ���� ���������� �Ÿ��� ������
    // Modify�� Weight �� ������. (�Ÿ� �� ���Ͱ� �ƴ� Scale)
    float ModifyWeight = 0.0f;

    // ���� �� �ν��Ͻ��� ����Ѵ�.
    Instance PreviousInstance;

public:
    inline void SetIsSelected(bool selected)
    {
        bIsSelected = selected;
    }

public:
    void ControlSwitch_MouseLeftDown(WPARAM& key);
    void ControlSwitch_MouseLeftUp(WPARAM& key);
    void ControlSwitch_MouseRightDown(WPARAM& key);
    void ControlSwitch_MouseRightUp(WPARAM& key);
    void ControlSwitch_KeyDown(WPARAM& key);
    void ControlSwitch_KeyUp(WPARAM& key);

public:
    void Update();

};

class EditorHelperControllerPool {
private:
    static EditorHelperControllerPool instance;

public:
    static inline EditorHelperControllerPool& getInstance()
    {
        return instance;
    }

public:
    EditorHelperController data;
};