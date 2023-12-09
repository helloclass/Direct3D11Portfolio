#include "CreateWindows.h"

CreateWindows CreateWindows::_WINDOW;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    void (*_BUTTON_FUNC)();
    UINT32 _BUTTON_INDEX = 0;

    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    if (msg == WM_ACTIVATE)
    {
        if (LOWORD(wparam) == WA_INACTIVE)
        {
            // 백그라운드 상태면 입력 중지
            CreateWindows::getInstance().isPaused = true;
        }
        else
        {
            CreateWindows::getInstance().isPaused = false;

            // 화면이 활성화 되면서 컨트롤러의 권한을 얻는다.
            Controller::getInstance().SetWindow(
                &(CreateWindows::getInstance().GetHWND())
            );
        }
    }

    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);    //GetDC(hwnd);
        //TextOut(hdc, 940, 500, (L"Hello 안녕"), sizeof(L"Hello 안녕"));
        //rect.left = 0;
        //rect.top = 0;
        //rect.right = 1000;
        //rect.bottom = 500;
        //DrawText(hdc, L"안녕하세요 ㅎㅎ", sizeof(L"안녕하세요 ㅎㅎ"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        
        //SetBkColor(hdc, COLORREF(RGB(255, 0, 0)));
        //TextOut(hdc, 940, 500, L"Hello", 10);

        EndPaint(hwnd, &ps);	// == ReleaseDC(hwnd, hdc);

        break;
    case WM_COMMAND:
    {
        for (UI_Button* button : UIPool::getInstance().UI_BUTTON_LIST)
        {
            if (LOWORD(wparam) == button->HWND_BUTTON_ID)
                button->OnClicked();
        }

        for (UI_ListBox* listbox : UIPool::getInstance().UI_LIST_BOX_LIST)
        {
            if (LOWORD(wparam) == listbox->HWND_LIST_BOX_ID && HIWORD(wparam) == LBN_SELCHANGE)
            {
                int count = SendMessage(listbox->hwnd, LB_GETCOUNT, 0, 0);
                // go through the items and find the first selected one
                for (int i = 0; i < count; i++)
                {
                    // check if this item is selected or not..
                    if (SendMessage(listbox->hwnd, LB_GETSEL, i, 0) > 0)
                    {
                        // yes, we only want the first selected so break.
                        listbox->Selected = i;

                        if (listbox->name == "World_List")
                        {
                            int midIDX = -1;
                            std::string str = listbox->GetTextListBoxMessage();

                            // 오브젝트의 이름 규칙 : "Name" + "_" + "Index"
                            for (int i = 0; i < str.size(); i++)
                            {
                                if (str[i] == '_')
                                {
                                    midIDX = i;
                                    break;
                                }
                            }

                            if (midIDX == -1)
                                break;

                            std::unordered_map<std::string, Transform::transform*>::iterator iter;
                            std::unordered_map<std::string, Transform::transform*>::iterator endIter;

                            // Find Object
                            std::string ObjectDataName = str.substr(0, midIDX);
                            ObjectData * data = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.find(ObjectDataName)->second;

                            iter = data->transform->InstanceBuffer.begin();
                            endIter = data->transform->InstanceBuffer.end();

                            while (iter != endIter)
                            {
                                // Find Instance
                                if (!std::strcmp(iter->first.c_str(), str.c_str()))
                                {
                                    // Target 지정
                                    MemoryPool::getInstance().pickedInstance = iter->second;
                                    EditorHelperControllerPool::getInstance().data.SetIsSelected(true);

                                    CreateWindows* window = &CreateWindows::getInstance();

                                    HWNDPool::getInstance().TargetObjectName = ObjectDataName;
                                    HWNDPool::getInstance().TargetInstanceName = iter->first;

                                    HWNDPool::getInstance().PosXEdit.data = 
                                        &iter->second->LocalMatrix->Position[0];
                                    HWNDPool::getInstance().PosYEdit.data = 
                                        &iter->second->LocalMatrix->Position[1];
                                    HWNDPool::getInstance().PosZEdit.data = 
                                        &iter->second->LocalMatrix->Position[2];

                                    HWNDPool::getInstance().RotXEdit.data = 
                                        &iter->second->LocalMatrix->Rotation[0];
                                    HWNDPool::getInstance().RotYEdit.data = 
                                        &iter->second->LocalMatrix->Rotation[1];
                                    HWNDPool::getInstance().RotZEdit.data = 
                                        &iter->second->LocalMatrix->Rotation[2];

                                    HWNDPool::getInstance().ScaleXEdit.data = 
                                        &iter->second->LocalMatrix->Scale[0];
                                    HWNDPool::getInstance().ScaleYEdit.data = 
                                        &iter->second->LocalMatrix->Scale[1];
                                    HWNDPool::getInstance().ScaleZEdit.data = 
                                        &iter->second->LocalMatrix->Scale[2];

                                    HWNDPool::getInstance().PosXEdit.SetEditText (
                                        std::to_string(iter->second->LocalMatrix->Position[0])
                                    );
                                    HWNDPool::getInstance().PosYEdit.SetEditText(
                                        std::to_string(iter->second->LocalMatrix->Position[1])
                                    );
                                    HWNDPool::getInstance().PosZEdit.SetEditText(
                                        std::to_string(iter->second->LocalMatrix->Position[2])
                                    );

                                    HWNDPool::getInstance().RotXEdit.SetEditText(
                                        std::to_string(iter->second->LocalMatrix->Rotation[0])
                                    );
                                    HWNDPool::getInstance().RotYEdit.SetEditText(
                                        std::to_string(iter->second->LocalMatrix->Rotation[1])
                                    );
                                    HWNDPool::getInstance().RotZEdit.SetEditText(
                                        std::to_string(iter->second->LocalMatrix->Rotation[2])
                                    );

                                    HWNDPool::getInstance().ScaleXEdit.SetEditText(
                                        std::to_string(iter->second->LocalMatrix->Scale[0])
                                    );
                                    HWNDPool::getInstance().ScaleYEdit.SetEditText(
                                        std::to_string(iter->second->LocalMatrix->Scale[1])
                                    );
                                    HWNDPool::getInstance().ScaleZEdit.SetEditText(
                                        std::to_string(iter->second->LocalMatrix->Scale[2])
                                    );

                                }

                                iter++;
                            }
                        }
                    }
                }
            }
        }

        for (UI_Edit* edit : UIPool::getInstance().UI_EDIT_LIST)
        {
            if (LOWORD(wparam) == edit->HWND_EDIT_ID)
            {
                switch (HIWORD(wparam))
                {
                case EN_CHANGE:
                {
                    // 오브젝트가 선택되었을 때
                    if (MemoryPool::getInstance().pickedInstance && edit->data)
                    {
                        if (edit->name == "Pos_X" ||
                            edit->name == "Pos_Y" || 
                            edit->name == "Pos_Z" ||
                            edit->name == "Rot_X" ||
                            edit->name == "Rot_Y" ||
                            edit->name == "Rot_Z" ||
                            edit->name == "Scale_X" ||
                            edit->name == "Scale_Y" ||
                            edit->name == "Scale_Z")
                        {
                            // Transpose Editor에 담긴 값 그대로
                            // Physx에 업데이트 하여 반영한다.
                            MemoryPool::getInstance().pickedInstance->bIsDirtyInstance = true;
                        }
                    }

                    break;
                }
                }
            }
        }

        break;
    }
    case WM_LBUTTONDOWN:
        GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionLeftDown] = true;
        EditorHelperControllerPool::getInstance().data.ControlSwitch_MouseLeftDown(wparam);
        break;
    case WM_MBUTTONDOWN:
        GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionMiddleDown] = true;
        break;
    case WM_RBUTTONDOWN:
        GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionRightDown] = true;
        EditorHelperControllerPool::getInstance().data.ControlSwitch_MouseRightDown(wparam);
        break;
    case WM_LBUTTONUP:
        GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionLeftUp] = true;
        EditorHelperControllerPool::getInstance().data.ControlSwitch_MouseLeftUp(wparam);
        break;
    case WM_MBUTTONUP:
        GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionMiddleUp] = true;
        break;
    case WM_RBUTTONUP:
        GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionRightUp] = true;
        EditorHelperControllerPool::getInstance().data.ControlSwitch_MouseRightUp(wparam);
        break;
    case WM_MOUSEMOVE:
        break;
    case WM_MOUSEWHEEL:
        break;
    case WM_KEYDOWN:
    {
        EditorHelperControllerPool::getInstance().data.ControlSwitch_KeyDown(wparam);

        bool isDown = (msg == WM_KEYDOWN);
        if (wparam == VK_ESCAPE)
        {
            CreateWindows::getInstance().isQuit = true;
            DestroyWindow(hwnd);
        }
        else if (wparam == 'W')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamFwd] = isDown;
        else if (wparam == 'A')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamLeft] = isDown;
        else if (wparam == 'S')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamBack] = isDown;
        else if (wparam == 'D')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamRight] = isDown;
        else if (wparam == 'E')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionRaiseCam] = isDown;
        else if (wparam == 'Q')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionLowerCam] = isDown;
        else if (wparam == VK_UP)
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionLookUp] = isDown;
        else if (wparam == VK_LEFT)
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionTurnCamLeft] = isDown;
        else if (wparam == VK_DOWN)
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionLookDown] = isDown;
        else if (wparam == VK_RIGHT)
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionTurnCamRight] = isDown;

        break;
    }
    case WM_KEYUP:
    {
        EditorHelperControllerPool::getInstance().data.ControlSwitch_KeyUp(wparam);

        bool isUp = false;
        if (wparam == VK_ESCAPE)
        {
            CreateWindows::getInstance().isQuit = true;
            DestroyWindow(hwnd);
        }
        else if (wparam == 'W')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamFwd] = isUp;
        else if (wparam == 'A')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamLeft] = isUp;
        else if (wparam == 'S')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamBack] = isUp;
        else if (wparam == 'D')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionMoveCamRight] = isUp;
        else if (wparam == 'E')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionRaiseCam] = isUp;
        else if (wparam == 'Q')
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionLowerCam] = isUp;
        else if (wparam == VK_UP)
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionLookUp] = isUp;
        else if (wparam == VK_LEFT)
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionTurnCamLeft] = isUp;
        else if (wparam == VK_DOWN)
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionLookDown] = isUp;
        else if (wparam == VK_RIGHT)
            GlobalKeyVector::getInstance().global_KeyboardBehavior[GameActionTurnCamRight] = isUp;

        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        global_windowDidResize = true;
        CreateWindows::getInstance().OnSize(lparam);
        break;
    }
    case WM_NOTIFY:
    {
        CreateWindows::getInstance().OnNotify(lparam);
        break;
    }
    default:
        result = DefWindowProcW(hwnd, msg, wparam, lparam);
    }
    return result;
}

CreateWindows::CreateWindows()
{

}

CreateWindows::~CreateWindows()
{
    int buttonSize = UIPool::getInstance().UI_BUTTON_LIST.size();
    int editSize = UIPool::getInstance().UI_EDIT_LIST.size();
    int listboxSize = UIPool::getInstance().UI_LIST_BOX_LIST.size();
    int trackbarSize = UIPool::getInstance().UI_TRACKBAR_LIST.size();

    for (int i = 0; i < buttonSize; i++)
        delete UIPool::getInstance().UI_BUTTON_LIST[i];
    for (int i = 0; i < editSize; i++)
        delete UIPool::getInstance().UI_EDIT_LIST[i];
    for (int i = 0; i < listboxSize; i++)
        delete UIPool::getInstance().UI_LIST_BOX_LIST[i];
    for (int i = 0; i < trackbarSize; i++)
        delete UIPool::getInstance().UI_TRACKBAR_LIST[i];
}

void CreateWindows::Init(const HINSTANCE& hInst)
{
    this->hMainInstance = hInst;
    this->isQuit    = false;

    WNDCLASSEXW winClass = {};
    winClass.cbSize = sizeof(WNDCLASSEXW);
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc = &WndProc;
    winClass.hInstance = hMainInstance;
    winClass.hIcon = LoadIconW(0, IDI_APPLICATION);
    winClass.hCursor = LoadCursorW(0, IDC_ARROW);
    winClass.lpszClassName = L"MyWindowClass";
    winClass.hIconSm = LoadIconW(0, IDI_APPLICATION);

    if (!RegisterClassExW(&winClass)) {
        MessageBoxA(0, "RegisterClassEx failed", "Fatal Error", MB_OK);
        throw std::runtime_error("Create Window Exception!!");
    }

    RECT initialRect = { 0, 0, 1500, 850 };
    AdjustWindowRectEx(&initialRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
    LONG initialWidth = initialRect.right - initialRect.left;
    LONG initialHeight = initialRect.bottom - initialRect.top;

    RECT initialCTRRect = { 0, 0, 350, 850 };
    AdjustWindowRectEx(&initialCTRRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
    LONG initialCTRWidth = initialCTRRect.right - initialCTRRect.left;
    LONG initialCTRHeight = initialCTRRect.bottom - initialCTRRect.top;

    MainHWND = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
        winClass.lpszClassName,
        L"10. Blinn-Phong Lighting",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        400, 50,
        initialWidth,
        initialHeight,
        0, 0, hMainInstance, 0);

    HWNDPool::getInstance().CTL_HWND = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
        winClass.lpszClassName,
        L"Controller",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        50, 50,
        initialCTRWidth,
        initialCTRHeight,
        0, 0, HWNDPool::getInstance().hControlInstance, 0);

    if (!MainHWND || !HWNDPool::getInstance().CTL_HWND) {
        MessageBoxA(0, "CreateWindowEx failed", "Fatal Error", MB_OK);
        throw std::runtime_error("Create Window Exception!!");
    }

#pragma region A_MENU

    HWNDPool::getInstance().HelloButton.CreateButton(
        HWNDPool::getInstance().CTL_HWND,
        0, "Button_01",
        L"Hello_Button",
        10, 60, 100, 30,
        _BUTTON_FUNC::Button01
    );

    HWNDPool::getInstance().ObjectListBox.CreateListBox(
        HWNDPool::getInstance().CTL_HWND,
        0, "Object_List",
        10, 120, 320, 120
    );

    HWNDPool::getInstance().ObjectListButton.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        0, "Object_List_Button",
        10, 270, 320, 30,
        nullptr
    );

    HWNDPool::getInstance().TextureListBox.CreateListBox(
        HWNDPool::getInstance().CTL_HWND,
        0, "Texture_List",
        10, 330, 320, 120
    );

    HWNDPool::getInstance().TextureListButton.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        0, "Texture_List_Button",
        10, 480, 320, 30,
        nullptr
    );

    HWNDPool::getInstance().HWNDs[0]["Button_01"] =
        &HWNDPool::getInstance().HelloButton;
    HWNDPool::getInstance().HWNDs[0]["Object_List"] =
        &HWNDPool::getInstance().ObjectListBox;
    HWNDPool::getInstance().HWNDs[0]["Object_List_Button"] =
        &HWNDPool::getInstance().ObjectListButton;
    HWNDPool::getInstance().HWNDs[0]["Texture_List"] =
        &HWNDPool::getInstance().TextureListBox;
    HWNDPool::getInstance().HWNDs[0]["Texture_List_Button"] =
        &HWNDPool::getInstance().TextureListButton;

#pragma endregion

#pragma region B_MENU

    HWNDPool::getInstance().InstantateButton.CreateButton(
        HWNDPool::getInstance().CTL_HWND,
        1, "Button_Inst",
        L"Instantiate",
        10, 60, 100, 30,
        _BUTTON_FUNC::InstantateButton
    );

    HWNDPool::getInstance().DestroyButton.CreateButton(
        HWNDPool::getInstance().CTL_HWND,
        1, "Button_Dest",
        L"Destroy",
        120, 60, 100, 30,
        _BUTTON_FUNC::DestroyButton
    );

    HWNDPool::getInstance().WorldListBox.CreateListBox(
        HWNDPool::getInstance().CTL_HWND,
        1, "World_List",
        10, 110, 320, 450
    );

    HWNDPool::getInstance().PosXEdit.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        1, "Pos_X",
        10, 570, 100, 30,
        nullptr
    );
    HWNDPool::getInstance().PosYEdit.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        1, "Pos_Y",
        120, 570, 100, 30,
        nullptr
    );
    HWNDPool::getInstance().PosZEdit.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        1, "Pos_Z",
        230, 570, 100, 30,
        nullptr
    );

    HWNDPool::getInstance().RotXEdit.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        1, "Rot_X",
        10, 610, 100, 30,
        nullptr
    );
    HWNDPool::getInstance().RotYEdit.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        1, "Rot_Y",
        120, 610, 100, 30,
        nullptr
    );
    HWNDPool::getInstance().RotZEdit.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        1, "Rot_Z",
        230, 610, 100, 30,
        nullptr
    );

    HWNDPool::getInstance().ScaleXEdit.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        1, "Scale_X",
        10, 650, 100, 30,
        nullptr
    );
    HWNDPool::getInstance().ScaleYEdit.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        1, "Scale_Y",
        120, 650, 100, 30,
        nullptr
    );
    HWNDPool::getInstance().ScaleZEdit.CreateEdit(
        HWNDPool::getInstance().CTL_HWND,
        1, "Scale_Z",
        230, 650, 100, 30,
        nullptr
    );

    // UINT Param = 10;

    //CreateWindows::getInstance().CreateTrackbar(
    //    1, "Track_Bar_Test",
    //    0, 60, 300, 30,
    //    0, 100, 0, 100,
    //    Param
    //);

    HWNDPool::getInstance().HWNDs[1]["Button_Inst"] =
        &HWNDPool::getInstance().InstantateButton;
    HWNDPool::getInstance().HWNDs[1]["Button_Dest"] =
        &HWNDPool::getInstance().DestroyButton;
    HWNDPool::getInstance().HWNDs[1]["World_List"] =
        &HWNDPool::getInstance().WorldListBox;

    HWNDPool::getInstance().HWNDs[1]["Pos_X"] =
        &HWNDPool::getInstance().PosXEdit;
    HWNDPool::getInstance().HWNDs[1]["Pos_Y"] =
        &HWNDPool::getInstance().PosYEdit;
    HWNDPool::getInstance().HWNDs[1]["Pos_Z"] =
        &HWNDPool::getInstance().PosZEdit;

    HWNDPool::getInstance().HWNDs[1]["Rot_X"] =
        &HWNDPool::getInstance().RotXEdit;
    HWNDPool::getInstance().HWNDs[1]["Rot_Y"] =
        &HWNDPool::getInstance().RotYEdit;
    HWNDPool::getInstance().HWNDs[1]["Rot_Z"] =
        &HWNDPool::getInstance().RotZEdit;

    HWNDPool::getInstance().HWNDs[1]["Scale_X"] =
        &HWNDPool::getInstance().ScaleXEdit;
    HWNDPool::getInstance().HWNDs[1]["Scale_Y"] =
        &HWNDPool::getInstance().ScaleYEdit;
    HWNDPool::getInstance().HWNDs[1]["Scale_Z"] =
        &HWNDPool::getInstance().ScaleZEdit;

#pragma endregion
}

void CreateWindows::Update()
{
    // 움직임은 나중에 Physx 연동 되면
    if (isMoveFlag)
    {
        // Forward, Up, Right를 얻을 수 있다
        float4x4 cubeModelViewMat = CameraPool::getInstance().getCamera().getViewMat();

        MemoryPool::getInstance().pickedInstance;
    }

    else if (isRotatebleFlag)
    {
        MemoryPool::getInstance().pickedInstance;
    }

    else if (isScaleFlag)
    {
        MemoryPool::getInstance().pickedInstance;
    }

    for (SubWindow* subWindow : CreateWindows::getInstance()._SubWindows)
    {
        subWindow->Update();
    }
}

const HWND& CreateWindows::GetHWND()
{
    return MainHWND;
}

void CreateWindows::CreateTabControl()
{
    RECT rcClient;
    HWND hwndTab;
    TCITEM tie;
    int i;

    std::array<std::array<TCHAR, 256>, _TAB_COUNT> achTemp = {
        L"Object",
        L"World",
        L"C",
        L"D",
        L"E",
    };  // Temporary buffer for strings.

    // Get the dimensions of the parent window's client area, and 
    // create a tab control child window of that size. Note that g_hInst
    // is the global instance handle.
    GetClientRect(HWNDPool::getInstance().CTL_HWND, &rcClient);
    HWNDPool::getInstance().Tab_CTL_HWND = CreateWindow(
        WC_TABCONTROL,
        L"",
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
        0,
        0,
        rcClient.right,
        rcClient.bottom,
        HWNDPool::getInstance().CTL_HWND,
        NULL,
        HWNDPool::getInstance().hControlInstance,
        NULL);

    if (HWNDPool::getInstance().Tab_CTL_HWND == NULL)
    {
        return;
    }

    // Add tabs for each day of the week. 
    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;

    for (i = 0; i < _TAB_COUNT; i++)
    {
        tie.pszText = (LPWSTR)(achTemp[i].data());

        if (TabCtrl_InsertItem(HWNDPool::getInstance().Tab_CTL_HWND, i, &tie) == -1)
        {
            DestroyWindow(HWNDPool::getInstance().Tab_CTL_HWND);
            return;
        }
    }

    return;
}

HRESULT CreateWindows::OnSize(LPARAM lParam)
{
    RECT rc;

    if (HWNDPool::getInstance().Tab_CTL_HWND == NULL)
        return E_INVALIDARG;

    // Resize the tab control to fit the client are of main window.
    if (!SetWindowPos(HWNDPool::getInstance().Tab_CTL_HWND, HWND_TOP, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), SWP_SHOWWINDOW))
        return E_FAIL;

    return S_OK;
}

BOOL CreateWindows::OnNotify(LPARAM lParam)
{
    UINT i = 0;
    std::unordered_map<std::string, UI_HWND*>::iterator iter, endIter;
    TCHAR achTemp[256]; // temporary buffer for strings

    switch (((LPNMHDR)lParam)->code)
    {
    case TCN_SELCHANGING:
    {
        // Return FALSE to allow the selection to change.
        //return FALSE;
    }

    case TCN_SELCHANGE:
    {
        int iPage = TabCtrl_GetCurSel(HWNDPool::getInstance().Tab_CTL_HWND);

        // Note that g_hInst is the global instance handle.
        LoadString(
            HWNDPool::getInstance().hControlInstance,
            iPage,
            achTemp,
            sizeof(achTemp) / sizeof(achTemp[0])
        );
        LRESULT result = SendMessage(HWNDPool::getInstance().CTL_HWND, WM_SETTEXT, 0,
            (LPARAM)achTemp);

        for (; i < 6; i++)
        {
            // 만일 iPage와 버튼의 classID가 같다면
            if (i == iPage)
            {
                iter = HWNDPool::getInstance().HWNDs[i].begin();
                endIter = HWNDPool::getInstance().HWNDs[i].end();

                while (iter != endIter)
                {
                    iter->second->setVisible(true);
                    iter++;
                }
            }
            // 다르다면
            else
            {
                iter = HWNDPool::getInstance().HWNDs[i].begin();
                endIter = HWNDPool::getInstance().HWNDs[i].end();

                while (iter != endIter)
                {
                    iter->second->setVisible(false);
                    iter++;
                }
            }
        }

        break;
    }
    }
    return TRUE;
}