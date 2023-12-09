#include "DirectX.h"

#include "AnimationSequence.h"

#include "Item/SkyBox.h"
#include "Item/Fly.h"
#include "Item/Head.h"
#include "Item/Hammer.h"

#include "Item/Axis.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
    // Initialize Window
    CreateWindows::getInstance().Init(hInstance);
    PhysxPool::getInstance().data.Init();

#pragma region Initialize

    CreateWindows::getInstance().CreateTabControl();

    SkyBox* sky;
    Fly* fly;
    Head* head;
    Hammer* weapon;
    Axis* axis;
    Bone* bone;

    sky = new SkyBox;
    fly = new Fly;
    head = new Head;
    weapon = new Hammer;
    axis = new Axis;
    bone = new Bone;

    axis->Awake();
    sky->Awake();
    fly->Awake();
    head->Awake();
    weapon->Awake();
    bone->Awake();

    {
        // 이후에는 메모장에서 불러올것
        LightData* data = &MemoryPool::getInstance()._GLOBAL_LIGHT_POOL_;

        data->dirLightNum = 1;
        data->pointLightNum = 2;

        float intensity = 0.1f;

        data->dirLight[0].color = { intensity, intensity, intensity, 1.0f };
        data->dirLight[0].dirEye = { 10.0f, 2.0f, 0.0f, 0.0f };
        data->dirLight[0].ambientStrength = 0.1f;
        data->dirLight[0].specularStrength = 0.5f;
        data->dirLight[0].specularExponent = 10;

        data->pointLights[0].color = { 10.0f, 10.0f, 10.0f, 1.0f };
        data->pointLights[0].posEye = { 0.0f, -20.0f, 0.0f, 1.0f };
        data->pointLights[0].ambientStrength = 0.1f;
        data->pointLights[0].specularStrength = 0.7f;
        data->pointLights[0].specularExponent = 10;

        data->pointLights[1].color = { 10.0f, 10.0f, 10.0f, 1.0f };
        data->pointLights[1].posEye = { 0.0f, 20.0f, 0.0f, 1.0f };
        data->pointLights[1].ambientStrength = 0.1f;
        data->pointLights[1].specularStrength = 0.7f;
        data->pointLights[1].specularExponent = 10;
    }

    // Initialize DirectX
    _DirectXHelper.Init(CreateWindows::getInstance().GetHWND());

    AnimationSequence::getInstance().Init(std::wstring(L"AnimationSequence"));
    AnimationSequence::getInstance().SetTarget(fly);

#pragma region Update UI



#pragma endregion


    global_windowDidResize = true;

#pragma region Start

    // 카메라 마우스 컨트롤러를 메인스크린 HWND로 적용
    Controller::getInstance().SetWindow(
        const_cast<HWND*>(&CreateWindows::getInstance().GetHWND())
    );

    axis->Start();
    sky->Start();
    fly->Start();
    head->Start();
    weapon->Start();
    bone->Start();

    head->AttachTo(fly, "Neck");
    weapon->AttachTo(fly, "Hand_L");
    bone->SetTarget(fly, fly->getName() + "_0");

#pragma endregion

#pragma endregion

    while (!CreateWindows::getInstance().isQuit)
    {
        MSG msg = {};
        while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                CreateWindows::getInstance().isQuit = true;

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        Controller::getInstance().Update();
        CreateWindows::getInstance().Update();
        CameraPool::getInstance().getCamera().Update();

        if (PhysxPool::getInstance().IsPlay)
        {
            GlobalTime::getInstance().Update();
            PhysxPool::getInstance().data.Update();
        }
        else
        {
            GlobalTime::getInstance().Update();
            PhysxPool::getInstance().data.Update();

            EditorHelperControllerPool::getInstance().data.Update();
            AnimationSequence::getInstance().Update();
        }
        
        axis->Update();

        sky->Update();

        fly->Update();
        head->Update();
        weapon->Update();
        bone->Update();

        _DirectXHelper.Update();
    }

    axis->Destroy();
    sky->Destroy();
    fly->Destroy();
    head->Destroy();
    weapon->Destroy();
    bone->Destroy();

    delete sky;
    delete fly;
    delete weapon;
    delete axis;
    delete bone;

    _DirectXHelper.Destroy();

#ifdef DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

    _CrtDumpMemoryLeaks();
#endif

    return 0;
}












//////////////////////////////////////////////////////////////
// Documents
//////////////////////////////////////////////////////////////
/*

    CreatePMXModel 사용법
    //pmx::CreatePMXModel(
    //    objData1,
    //    "C:\\Users\\rainb\\Downloads\\test\\untitled.pmx"
    //);


*/