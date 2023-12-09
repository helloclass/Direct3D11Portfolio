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

    // 마우스가 이전 위치에 비해 얼마나 많이 움직였는지 거리를 측도로
    // Modify의 Weight 가 지정됨. (거리 즉 벡터가 아닌 Scale)
    float ModifyWeight = 0.0f;

    // 변형 전 인스턴스를 기록한다.
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