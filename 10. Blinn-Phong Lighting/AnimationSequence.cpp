#include "AnimationSequence.h"

// Make to Singleton AnimationSequence
AnimationSequence AnimationSequence::instance;
AnimSequenceKeyVector AnimSequenceKeyVector::instance;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace _ANIMATION_BUTTON_FUNC {

    //////////////////////////////////////////////////////
    // 몽타주, 블랜드 스페이스 파일을 저장할 경우
    // 이름에 대한 인덱스를 누적하기 위한 전역변수
    //////////////////////////////////////////////////////
    static UINT _MONTAGE_INDEX_ACCUMULATION = 0;
    static UINT _BLEND_SPACE_INDEX_ACCUMULATION = 0;
    static UINT _COMPOSITE_INDEX_ACCUMULATION = 0;

#pragma region Sequence_Function

    // 시퀀스용 뼈대에 델타 키 추가 / 제거
    static void AddKeyButtonFunc()
    {

    }
    // 시퀀스용 델타 적용된 애니메이션 저장
    static void SaveKeyButtonFunc()
    {

    }

#pragma endregion

#pragma region Montage_Function

    // 몽타주 저장 버튼
    static void SaveMontageButtonFunc(ObjectData* obj)
    {
        std::string DirPath =
            "MetaData\\" + obj->objectName + "\\Montage\\";
        std::string Path;

        // 파일 오픈 또는 생성
        std::ofstream MontageFile;

        CreateFolders(DirPath);

        for (std::pair<std::string, Montage> mont : obj->Montages)
        {
            Path = DirPath + mont.second.mName + ".mtg";

            MontageFile.open(Path, std::ios::out | std::ios::binary);
            if (!MontageFile.is_open())
            {
                MessageBoxA(0, "Create Montage File Failed", "Warning", MB_OK);
                return;
            }

            std::string UintToString;

            for (Montage::SequenceClip& clip : mont.second.mSequenceList)
            {
                // 시퀀스 이름 저장
                MontageFile.write(clip.mSequence->mName.c_str(), sizeof(char*) * clip.mSequence->mName.size());
                // 시퀀스 시작 프레임 저장
                UintToString = std::to_string(clip.mClipStartFrame);
                MontageFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
                // 시퀀스 끝 프레임 저장
                UintToString = std::to_string(clip.mClipEndFrame);
                MontageFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
            }

            MontageFile.close();
        }
    }

    // 몽타주 생성버튼
    static void CreateMontageButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().SequenceListBox.Selected)
        {
            Montage newMontage;
            UINT SequenceIndex = AnimationSequence::getInstance().SequenceListBox.Selected;
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // UI ListBox에서 선택한 인수와 실제 시퀀스의 개수가 다를 경우
            if (Target->Sequences.size() <= SequenceIndex)
                throw std::runtime_error("설계 미스! 시퀀시 리스트 개수와 실제 시퀀시의 개수가 다름");

            newMontage.mName = "Montage_" + std::to_string(_MONTAGE_INDEX_ACCUMULATION++);
            newMontage.PushSequence(Target->Sequences[SequenceIndex]);

            // 몽타주 추가
            Target->Montages[newMontage.mName] = newMontage;

            // 몽타주 리스트에도 텍스트 추가
            AnimationSequence::getInstance().MontageListBox.AppendListBoxMessage(newMontage.mName);

            // 몽타주 자동저장
            SaveMontageButtonFunc(Target);
        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected Sequence!!",
                "WARNING",
                MB_OK);
        }
    }
    // 몽타주 제거버튼
    static void DestroyMontageButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().MontageListBox.Selected)
        {
            UINT SequenceIndex = AnimationSequence::getInstance().MontageListBox.Selected;
            std::string SelectedName = AnimationSequence::getInstance().MontageListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // 타겟에 저장되어있는 몽타주 없에고
            Target->Montages.erase(SelectedName);
            // UI List의 몽타주 제거하고
            AnimationSequence::getInstance().MontageListBox.DeleteListBoxMessage(SequenceIndex);

            // like weaked ptr
            Target->SetAsset(Target->Sequences[0]);
        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected Montage!!",
                "WARNING",
                MB_OK);
        }
    }
    // 몽타주에서 시퀀스 넣기
    static void PushMontageButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().MontageListBox.Selected)
        {
            if (AnimationSequence::getInstance().SequenceListBox.Selected == -1)
                return;

            UINT SequenceIndex = AnimationSequence::getInstance().SequenceListBox.Selected;
            std::string SelectedName = AnimationSequence::getInstance().MontageListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            Target->Montages[SelectedName].mBlendFrame = AnimationSequence::getInstance().pMontageBlendTime;
            Target->Montages[SelectedName].PushSequence(Target->Sequences[SequenceIndex]);

        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected Montage!!",
                "WARNING",
                MB_OK);
        }
    }
    // 몽타주에서 시퀀스 빼기
    static void PopMontageButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().MontageListBox.Selected)
        {
            if (AnimationSequence::getInstance().SequenceListBox.Selected == -1)
                return;

            std::string SelectedName = AnimationSequence::getInstance().MontageListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            Target->Montages[SelectedName].PopSequence();

        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected Montage!!",
                "WARNING",
                MB_OK);
        }
    }

#pragma endregion

#pragma region BlendSpace_Function

    // 블랜드 스페이스 저장 버튼
    static void SaveBlendSpaceButtonFunc(ObjectData* obj)
    {
        std::string DirPath =
            "MetaData\\" + obj->objectName + "\\BlendSpace\\";
        std::string Path;

        // 파일 오픈 또는 생성
        std::ofstream BlendSpaceFile;
        CreateFolders(DirPath);

        for (std::pair<std::string, BlendSpace> space : obj->BlendSpaces)
        {
            Path = DirPath + space.second.mName + ".bsp";

            BlendSpaceFile.open(Path, std::ios::out | std::ios::binary);
            if (!BlendSpaceFile.is_open())
            {
                MessageBoxA(0, "Create BlendSpace File Failed", "Warning", MB_OK);
                return;
            }

            std::string UintToString;

            for (BlendSpace::SequenceClip& clip : space.second.mClips)
            {
                // 시퀀스 이름 저장
                BlendSpaceFile.write(clip.mSequence->mName.c_str(), sizeof(char*) * clip.mSequence->mName.size());
                // 시퀀스 시작 프레임 저장
                UintToString = std::to_string(clip.mPositionX);
                BlendSpaceFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
                // 시퀀스 끝 프레임 저장
                UintToString = std::to_string(clip.mPositionY);
                BlendSpaceFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
            }

            BlendSpaceFile.close();
        }
    }

    // 블랜드 스페이스 생성버튼
    static void CreateBlendSpaceButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().SequenceListBox.Selected)
        {
            BlendSpace newBlendSpace;
            UINT SequenceIndex = AnimationSequence::getInstance().SequenceListBox.Selected;
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // UI ListBox에서 선택한 인수와 실제 시퀀스의 개수가 다를 경우
            if (Target->Sequences.size() <= SequenceIndex)
                throw std::runtime_error("설계 미스! 시퀀시 리스트 개수와 실제 시퀀시의 개수가 다름");

            newBlendSpace.mName = "BlendSpace_" + std::to_string(_BLEND_SPACE_INDEX_ACCUMULATION++);
            newBlendSpace.PushSequence(
                Target->Sequences[SequenceIndex],
                AnimationSequence::getInstance().pBlendSpacePositionY,
                AnimationSequence::getInstance().pBlendSpacePositionX
            );

            // 블랜드 스페이스 추가
            Target->BlendSpaces[newBlendSpace.mName] = newBlendSpace;

            // 블랜드 스페이스 리스트에도 텍스트 추가
            AnimationSequence::getInstance().BlendSpaceListBox.AppendListBoxMessage(newBlendSpace.mName);

            // 블랜드 스페이스 자동저장
            SaveBlendSpaceButtonFunc(Target);
        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected Sequence!!",
                "WARNING",
                MB_OK);
        }
    }

    // 블랜드 스페이스 제거버튼
    static void DestroyBlendSpaceButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().BlendSpaceListBox.Selected)
        {
            UINT SequenceIndex = AnimationSequence::getInstance().BlendSpaceListBox.Selected;
            std::string SelectedName = AnimationSequence::getInstance().BlendSpaceListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // 타겟에 저장되어있는 블랜드 스페이스 없에고
            Target->BlendSpaces.erase(SelectedName);
            // UI List의 블랜드 스페이스 제거하고
            AnimationSequence::getInstance().BlendSpaceListBox.DeleteListBoxMessage(SequenceIndex);

            // like weaked ptr
            Target->SetAsset(Target->Sequences[0]);
        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected BlendSpace!!",
                "WARNING",
                MB_OK);
        }
    }

    // 블랜드 스페이스에 시퀀스 넣기
    static void PushBlendSpaceButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().BlendSpaceListBox.Selected)
        {
            if (AnimationSequence::getInstance().SequenceListBox.Selected == -1)
                return;

            UINT SequenceIndex = AnimationSequence::getInstance().SequenceListBox.Selected;
            std::string SelectedName = AnimationSequence::getInstance().BlendSpaceListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            Target->BlendSpaces[SelectedName].PushSequence(
                Target->Sequences[SequenceIndex],
                AnimationSequence::getInstance().pBlendSpacePositionY,
                AnimationSequence::getInstance().pBlendSpacePositionX
            );
        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected BlendSpace!!",
                "WARNING",
                MB_OK);
        }
    }

    // 블랜드 스페이스에서 시퀀스 빼기
    static void PopBlendSpaceButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().BlendSpaceListBox.Selected)
        {
            if (AnimationSequence::getInstance().SequenceListBox.Selected == -1)
                return;

            std::string SelectedName = AnimationSequence::getInstance().BlendSpaceListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            Target->BlendSpaces[SelectedName].PopSequence();

        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected BlendSpace!!",
                "WARNING",
                MB_OK);
        }
    }
#pragma endregion

#pragma region Composite_Function

    // 컴포지트 저장 버튼
    static void SaveCompositeButtonFunc(ObjectData* obj)
    {
        std::string DirPath =
            "MetaData\\" + obj->objectName + "\\Composite\\";
        std::string Path;

        // 파일 오픈 또는 생성
        std::ofstream CompositeFile;
        CreateFolders(DirPath);

        for (std::pair<std::string, Composite> space : obj->Composites)
        {
            Path = DirPath + space.second.mName + ".com";

            CompositeFile.open(Path, std::ios::out | std::ios::binary);
            if (!CompositeFile.is_open())
            {
                MessageBoxA(0, "Create Composite File Failed", "Warning", MB_OK);
                return;
            }

            std::string UintToString;

            for (Composite::SequenceClip& clip : space.second.mClips)
            {
                // 시퀀스 이름 저장
                CompositeFile.write(clip.mSequence->mName.c_str(), sizeof(char*) * clip.mSequence->mName.size());
                // 시퀀스 시작 프레임 저장
                UintToString = std::to_string(clip.mPositionX);
                CompositeFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
                // 시퀀스 끝 프레임 저장
                UintToString = std::to_string(clip.mPositionY);
                CompositeFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
            }

            CompositeFile.close();
        }
    }

    // 블랜드 스페이스 생성버튼
    static void CreateCompositeButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().SequenceListBox.Selected)
        {
            Composite newComposite;
            UINT SequenceIndex = AnimationSequence::getInstance().SequenceListBox.Selected;
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // UI ListBox에서 선택한 인수와 실제 시퀀스의 개수가 다를 경우
            if (Target->Sequences.size() <= SequenceIndex)
                throw std::runtime_error("설계 미스! 시퀀시 리스트 개수와 실제 시퀀시의 개수가 다름");

            newComposite.mName = "Composite_" + std::to_string(_COMPOSITE_INDEX_ACCUMULATION++);
            newComposite.PushSequence(
                Target->Sequences[SequenceIndex],
                AnimationSequence::getInstance().pCompositePositionY,
                AnimationSequence::getInstance().pCompositePositionX
            );

            // 컴포지트 추가
            Target->Composites[newComposite.mName] = newComposite;

            // 컴포지트 리스트에도 텍스트 추가
            AnimationSequence::getInstance().CompositeListBox.AppendListBoxMessage(newComposite.mName);

            // 컴포지트 자동저장
            SaveCompositeButtonFunc(Target);
        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected Sequence!!",
                "WARNING",
                MB_OK);
        }
    }

    // 블랜드 스페이스 제거버튼
    static void DestroyCompositeButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().CompositeListBox.Selected)
        {
            UINT SequenceIndex = AnimationSequence::getInstance().CompositeListBox.Selected;
            std::string SelectedName = AnimationSequence::getInstance().CompositeListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // 타겟에 저장되어있는 컴포지트 없에고
            Target->Composites.erase(SelectedName);
            // UI List의 컴포지트 제거하고
            AnimationSequence::getInstance().CompositeListBox.DeleteListBoxMessage(SequenceIndex);

            // like weaked ptr
            Target->SetAsset(Target->Sequences[0]);
        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected Composite!!",
                "WARNING",
                MB_OK);
        }
    }

    // 블랜드 스페이스에 시퀀스 넣기
    static void PushCompositeButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().CompositeListBox.Selected)
        {
            if (AnimationSequence::getInstance().SequenceListBox.Selected == -1)
                return;

            UINT SequenceIndex = AnimationSequence::getInstance().SequenceListBox.Selected;
            std::string SelectedName = AnimationSequence::getInstance().CompositeListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            Target->Composites[SelectedName].PushSequence(
                Target->Sequences[SequenceIndex],
                AnimationSequence::getInstance().pCompositePositionY,
                AnimationSequence::getInstance().pCompositePositionX
            );
        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected Composite!!",
                "WARNING",
                MB_OK);
        }
    }

    // 블랜드 스페이스에서 시퀀스 빼기
    static void PopCompositeButtonFunc()
    {
        // 시퀸스가 선택 되어있는 상태 일 때 
        if (0 <= AnimationSequence::getInstance().CompositeListBox.Selected)
        {
            if (AnimationSequence::getInstance().SequenceListBox.Selected == -1)
                return;

            std::string SelectedName = AnimationSequence::getInstance().CompositeListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            Target->Composites[SelectedName].PopSequence();
        }
        else
        {
            MessageBoxA(
                AnimationSequence::getInstance().getTab_CTL_HWND(),
                "Please Selected Composite!!",
                "WARNING",
                MB_OK);
        }
    }
#pragma endregion

}

BOOL OnNotify(LPARAM lParam)
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
        tabPage =
            TabCtrl_GetCurSel(AnimationSequence::getInstance().getTab_CTL_HWND());

        // Note that g_hInst is the global instance handle.
        LoadString(
            AnimationSequence::getInstance().getCTL_HWND_INSTANCE(),
            tabPage,
            achTemp,
            sizeof(achTemp) / sizeof(achTemp[0])
        );
        LRESULT result = SendMessage(
            AnimationSequence::getInstance().getTab_CTL_HWND(), WM_SETTEXT, 0,
            (LPARAM)achTemp);

        for (; i < 6; i++)
        {
            // 만일 iPage와 버튼의 classID가 같다면
            if (i == tabPage)
            {
                iter = AnimationSequence::getInstance().HWNDs[i].begin();
                endIter = AnimationSequence::getInstance().HWNDs[i].end();

                while (iter != endIter)
                {
                    iter->second->setVisible(true);
                    iter++;
                }
            }
            // 다르다면
            else
            {
                iter = AnimationSequence::getInstance().HWNDs[i].begin();
                endIter = AnimationSequence::getInstance().HWNDs[i].end();

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

LRESULT CALLBACK SubWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    void (*_BUTTON_FUNC)();
    UINT32 _BUTTON_INDEX = 0;

    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;

    if (msg == WM_ACTIVATE)
    {
        if (LOWORD(wparam) == WA_INACTIVE)
        {
            // 백그라운드 상태면 입력 중지
            bIsPaused = true;
            return result;
        }
        else
        {
            bIsPaused = false;

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
    case WM_ACTIVATE:
        if (LOWORD(wparam) == WA_INACTIVE)
        {
            // 백그라운드 상태면 입력 중지
            bIsPaused = true;
            return result;
        }
        else
        {
            bIsPaused = false;

            // 컨트롤러의 권한을 가져온다.
            Controller::getInstance().SetWindow(
                &(AnimationSequence::getInstance().getHWND())
            );
        }
        break;
    case WM_PAINT:
    case WM_DISPLAYCHANGE:
        hdc = BeginPaint(hwnd, &ps);    //GetDC(hwnd);

        EndPaint(hwnd, &ps);
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

                bIsListBoxUpdate = listbox;

                // 리스트 박스의 아이템 개수를 가져와서
                int count = SendMessage(listbox->hwnd, LB_GETCOUNT, 0, 0);
                // 클릭된 아이템을 찾는다
                for (int i = 0; i < count; i++)
                {
                    // 해당 아이템이 선택이 된건지 아닌지 확인한다
                    if (SendMessage(listbox->hwnd, LB_GETSEL, i, 0) > 0)
                    {
                        listbox->Selected = i;
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
                    //if (edit->data)
                    //    *edit->data = std::stof(edit->GetEditText());

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

        if (wparam == 'W')
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
        OnNotify(lparam);
        break;
    }
    default:
        result = DefWindowProcW(hwnd, msg, wparam, lparam);
    }
    return result;
}

AnimationSequence::AnimationSequence()
{

}

AnimationSequence::~AnimationSequence()
{
    ImGui::DestroyContext();
    //ImGui_ImplWin32_Shutdown();
}

void AnimationSequence::Init(std::wstring WindowName)
{
#pragma region Window

    WNDCLASSEXW winClass = {};
    winClass.cbSize = sizeof(WNDCLASSEXW);
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc = &SubWndProc;
    winClass.hInstance = hInstance;
    winClass.hIcon = LoadIconW(0, IDI_APPLICATION);
    winClass.hCursor = LoadCursorW(0, IDC_ARROW);
    winClass.lpszClassName = WindowName.c_str();
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

    hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
        winClass.lpszClassName,
        WindowName.c_str(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        400, 50,
        initialWidth,
        initialHeight,
        0, 0, hInstance, 0);

    CTL_HWND = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
        winClass.lpszClassName,
        L"AnimController",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        50, 50,
        initialCTRWidth,
        initialCTRHeight,
        0, 0, HWNDPool::getInstance().hControlInstance, 0);

    if (!hwnd || !CTL_HWND) {
        MessageBoxA(0, "CreateWindowEx failed", "Fatal Error", MB_OK);
        throw std::runtime_error("Create Window Exception!!");
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);

    // Create Tap
    RECT rcClient;
    HWND hwndTab;
    TCITEM tie;

    std::array<std::array<TCHAR, 256>, _TAB_COUNT> achTemp = {
        L"Sequence",
        L"Montage",
        L"BlendSpace",
        L"Composite",
        L"Morph",
    };  // Temporary buffer for strings.

    // Get the dimensions of the parent window's client area, and 
    // create a tab control child window of that size. Note that g_hInst
    // is the global instance handle.
    GetClientRect(hwnd, &rcClient);
    Tab_CTL_HWND = CreateWindow(
        WC_TABCONTROL,
        L"",
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
        0,
        0,
        rcClient.right,
        rcClient.bottom,
        CTL_HWND,
        NULL,
        hInstance,
        NULL);

    if (Tab_CTL_HWND == NULL)
    {
        return;
    }

    // Add tabs for each day of the week. 
    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;

    for (int i = 0; i < _TAB_COUNT; i++)
    {
        tie.pszText = (LPWSTR)(achTemp[i].data());

        if (TabCtrl_InsertItem(Tab_CTL_HWND, i, &tie) == -1)
        {
            DestroyWindow(Tab_CTL_HWND);
            return;
        }
    }

#pragma endregion

#pragma region ANIMATION_SEQUENCE_MENU

    KeyButton.CreateButton(
        CTL_HWND,
        0, "KeyButton",
        L"Key",
        10, 60, 100, 30,
        _ANIMATION_BUTTON_FUNC::AddKeyButtonFunc
    );

    SaveButton.CreateButton(
        CTL_HWND,
        0, "SaveButton",
        L"Save",
        120, 60, 100, 30,
        _ANIMATION_BUTTON_FUNC::SaveKeyButtonFunc
    );

    BonesListBox.CreateListBox(
        CTL_HWND,
        0, "BonesListBox",
        10, 110, 320, 450
    );

    HWNDs[0]["KeyButton"] = &KeyButton;
    HWNDs[0]["SaveButton"] = &SaveButton;
    HWNDs[0]["BonesListBox"] = &BonesListBox;

#pragma endregion

#pragma region ANIMATION_MONTAGE_MENU
    
    SequenceListBox.CreateListBox(
        CTL_HWND,
        1, "SequenceListBox",
        10, 60, 320, 100
    );

    MontageListBox.CreateListBox(
        CTL_HWND,
        1, "MontageListBox",
        10, 170, 320, 100
    );

    CreateMontage.CreateButton(
        CTL_HWND,
        1, "CreateMontage",
        L"Create",
        10, 280, 100, 30,
        _ANIMATION_BUTTON_FUNC::CreateMontageButtonFunc
    );

    DestroyMontage.CreateButton(
        CTL_HWND,
        1, "DestroyMontage",
        L"Destroy",
        120, 280, 100, 30,
        _ANIMATION_BUTTON_FUNC::DestroyMontageButtonFunc
    );

    MontageSeqListBox.CreateListBox(
        CTL_HWND,
        1, "MontageSeqListBox",
        10, 320, 320, 100
    );

    BlendTime.CreateEdit(
        CTL_HWND,
        1, "BlendTime",
        10, 430, 320, 20,
        &pMontageBlendTime
    );

    PushMontage.CreateButton(
        CTL_HWND,
        1, "PushMontage",
        L"Push",
        10, 460, 100, 30,
        _ANIMATION_BUTTON_FUNC::PushMontageButtonFunc
    );

    PopMontage.CreateButton(
        CTL_HWND,
        1, "PopMontage",
        L"Pop",
        120, 460, 100, 30,
        _ANIMATION_BUTTON_FUNC::PopMontageButtonFunc
    );

    HWNDs[1]["SequenceListBox"] = &SequenceListBox;
    HWNDs[1]["MontageListBox"] = &MontageListBox;

    HWNDs[1]["CreateMontage"] = &CreateMontage;
    HWNDs[1]["DestroyMontage"] = &DestroyMontage;

    HWNDs[1]["MontageSeqListBox"] = &MontageSeqListBox;

    HWNDs[1]["BlendTime"] = &BlendTime;

    HWNDs[1]["PushMontage"] = &PushMontage;
    HWNDs[1]["PopMontage"] = &PopMontage;

#pragma endregion

#pragma region ANIMATION_BLEND_SPACE_MENU

    SequenceListBox1.CreateListBox(
        CTL_HWND,
        2, "SequenceListBox1",
        10, 60, 320, 100
    );

    BlendSpaceListBox.CreateListBox(
        CTL_HWND,
        2, "BlendSpaceListBox",
        10, 170, 320, 100
    );

    CreateBlendSpace.CreateButton(
        CTL_HWND,
        2, "CreateBlendSpace",
        L"Create",
        10, 280, 100, 30,
        _ANIMATION_BUTTON_FUNC::CreateBlendSpaceButtonFunc
    );

    DestroyBlendSpace.CreateButton(
        CTL_HWND,
        2, "DestroyBlendSpace",
        L"Destroy",
        120, 280, 100, 30,
        _ANIMATION_BUTTON_FUNC::DestroyBlendSpaceButtonFunc
    );

    BlendSpaceSeqListBox.CreateListBox(
        CTL_HWND,
        2, "MontageSeqListBox",
        10, 320, 320, 100
    );

    BlendPositionY.CreateEdit(
        CTL_HWND,
        2, "BlendPositionY",
        10, 430, 155, 20,
        &pBlendSpacePositionY
    );

    BlendPositionX.CreateEdit(
        CTL_HWND,
        2, "BlendPositionX",
        175, 430, 155, 20,
        &pBlendSpacePositionX
    );

    PushBlendSpace.CreateButton(
        CTL_HWND,
        2, "PushBlendSpace",
        L"Push",
        10, 460, 100, 30,
        _ANIMATION_BUTTON_FUNC::PushBlendSpaceButtonFunc
    );

    PopBlendSpace.CreateButton(
        CTL_HWND,
        2, "PopBlendSpace",
        L"Pop",
        120, 460, 100, 30,
        _ANIMATION_BUTTON_FUNC::PopBlendSpaceButtonFunc
    );

    HWNDs[2]["SequenceListBox1"] = &SequenceListBox1;
    HWNDs[2]["BlendSpaceListBox"] = &BlendSpaceListBox;

    HWNDs[2]["CreateBlendSpace"] = &CreateBlendSpace;
    HWNDs[2]["DestroyBlendSpace"] = &DestroyBlendSpace;

    HWNDs[2]["BlendSpaceSeqListBox"] = &BlendSpaceSeqListBox;

    HWNDs[2]["BlendPositionY"] = &BlendPositionY;
    HWNDs[2]["BlendPositionX"] = &BlendPositionX;

    HWNDs[2]["PushBlendSpace"] = &PushBlendSpace;
    HWNDs[2]["PopBlendSpace"] = &PopBlendSpace;

#pragma endregion

#pragma region ANIMATION_COMPOSITE_MENU

    SequenceListBox2.CreateListBox(
        CTL_HWND,
        3, "SequenceListBox2",
        10, 60, 320, 100
    );

    CompositeListBox.CreateListBox(
        CTL_HWND,
        3, "CompositeListBox",
        10, 170, 320, 100
    );

    CreateComposite.CreateButton(
        CTL_HWND,
        3, "CreateComposite",
        L"Create",
        10, 280, 100, 30,
        _ANIMATION_BUTTON_FUNC::CreateCompositeButtonFunc
    );

    DestroyComposite.CreateButton(
        CTL_HWND,
        3, "DestroyComposite",
        L"Destroy",
        120, 280, 100, 30,
        _ANIMATION_BUTTON_FUNC::DestroyCompositeButtonFunc
    );

    CompositeSeqListBox.CreateListBox(
        CTL_HWND,
        3, "CompositeSeqListBox",
        10, 320, 320, 100
    );

    CompositePositionY.CreateEdit(
        CTL_HWND,
        2, "CompositePositionY",
        10, 430, 155, 20,
        &pCompositePositionY
    );

    CompositePositionX.CreateEdit(
        CTL_HWND,
        2, "CompositePositionX",
        175, 430, 155, 20,
        &pCompositePositionX
    );

    PushComposite.CreateButton(
        CTL_HWND,
        3, "PushComposite",
        L"Push",
        10, 460, 100, 30,
        _ANIMATION_BUTTON_FUNC::PushCompositeButtonFunc
    );

    PopComposite.CreateButton(
        CTL_HWND,
        3, "PopComposite",
        L"Pop",
        120, 460, 100, 30,
        _ANIMATION_BUTTON_FUNC::PopCompositeButtonFunc
    );

    HWNDs[3]["SequenceListBox2"] = &SequenceListBox2;
    HWNDs[3]["CompositeListBox"] = &CompositeListBox;

    HWNDs[3]["CreateComposite"] = &CreateComposite;
    HWNDs[3]["DestroyComposite"] = &DestroyComposite;

    HWNDs[3]["CompositeSeqListBox"] = &CompositeSeqListBox;

    HWNDs[3]["CompositePositionY"] = &CompositePositionY;
    HWNDs[3]["CompositePositionX"] = &CompositePositionX;

    HWNDs[3]["PushComposite"] = &PushComposite;
    HWNDs[3]["PopComposite"] = &PopComposite;

#pragma endregion

#pragma region ANIMATION_MORPH_MENU

    MorphListBox.CreateListBox(
        CTL_HWND,
        4, "CreateListBox",
        10, 60, 600, 100
    );

#pragma endregion

#pragma region DirectX

    DXGI_SWAP_CHAIN_DESC1 d3d11SwapChainDesc = {};
    d3d11SwapChainDesc.Width = 0; // use window width
    d3d11SwapChainDesc.Height = 0; // use window height
    d3d11SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    d3d11SwapChainDesc.SampleDesc.Count = 1;
    d3d11SwapChainDesc.SampleDesc.Quality = 0;
    d3d11SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    d3d11SwapChainDesc.BufferCount = 2;
    d3d11SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    d3d11SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    d3d11SwapChainDesc.Flags = 0;

    HRESULT hResult = CreateWindows::getInstance().dxgiFactory->CreateSwapChainForHwnd(
        CreateWindows::getInstance().d3d11Device,
        hwnd, 
        &d3d11SwapChainDesc, 0, 0, &d3d11SwapChain);
    assert(SUCCEEDED(hResult));

    ID3D11Texture2D* d3d11FrameBuffer;
    hResult = d3d11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11FrameBuffer);
    assert(SUCCEEDED(hResult));

    hResult = CreateWindows::getInstance().d3d11Device->CreateRenderTargetView(d3d11FrameBuffer, 0, &renderBufferView);
    assert(SUCCEEDED(hResult));

    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
    d3d11FrameBuffer->GetDesc(&depthBufferDesc);

    d3d11FrameBuffer->Release();

    depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    hResult = CreateWindows::getInstance().d3d11Device->CreateTexture2D(&depthBufferDesc, NULL, &depthBuffer);
    assert(SUCCEEDED(hResult));

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

    CreateWindows::getInstance().d3d11Device->CreateDepthStencilView(depthBuffer, NULL, &depthBufferView);

    depthBuffer->Release();

    CreateWindows::getInstance()._SubWindows.push_back(this);

    ImGui_ImplDX11_Init(
        CreateWindows::getInstance().d3d11Device,
        CreateWindows::getInstance().d3d11DeviceContext
    );

#pragma endregion

    Pivot = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_["Axis"];
    Bone = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_["Bone"];

}

void AnimationSequence::Update()
{
    if (bIsPaused)
        return;

    AnimationUpdate();

    // Frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 탭 페이지에 따라 IMGUI UI 변형
    switch (tabPage)
    {
    case 0:
        AnimationSequenceUpdate();
        break;
    case 1:
        AnimationMontageUpdate();
        break;
    case 2:
        AnimationBlendSpaceUpdate();
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    default:
        break;
    }

    // Render
    ImGui::Render();

    CreateWindows::getInstance().d3d11DeviceContext->ClearRenderTargetView(
        renderBufferView, backgroundColor);
    CreateWindows::getInstance().d3d11DeviceContext->ClearDepthStencilView(
        depthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    CreateWindows::getInstance().d3d11DeviceContext->OMSetRenderTargets(
        1, &renderBufferView, depthBufferView);

#pragma region Mouse Picking

    RECT WindowSize = {};
    POINT MousePos = {};
    GetWindowRect(hwnd, &WindowSize);
    GetCursorPos(&MousePos);

    ScreenToClient(hwnd, &MousePos);

    float w = (MousePos.x / (float)(WindowSize.right - WindowSize.left)) * 2.0f - 1.0f;
    float h = (MousePos.y / (float)(WindowSize.bottom - WindowSize.top)) * 2.0f - 1.0f;
    float z = 1.0f - pow(w * w + h * h, 0.5);

    float4x4 View = CameraPool::getInstance().getCamera().getViewMat();

    DirectX::XMMATRIX toLocal = {
        View.m[0][0], View.m[1][0], View.m[2][0], View.m[3][0],
        View.m[0][1], View.m[1][1], View.m[2][1], View.m[3][1],
        View.m[0][2], View.m[1][2], View.m[2][2], View.m[3][2],
        View.m[0][3], View.m[1][3], View.m[2][3], View.m[3][3],
    };

    DirectX::XMVECTOR transposeMat = DirectX::XMMatrixDeterminant(toLocal);
    toLocal = DirectX::XMMatrixInverse(&transposeMat, toLocal);

    // toLocal = DirectX::XMMatrixMultiply(toLocal, Persp);

    DirectX::XMVECTOR rayOrigin = { 0.0f, 0.0f, 0.0f, 1.0f };
    DirectX::XMVECTOR rayDir = { w + 0.0001f, h + 0.0001f, -z + 0.0001f, 1.0f };

    // World Position
    rayOrigin = DirectX::XMVector3TransformCoord(rayOrigin, toLocal);
    rayDir = DirectX::XMVector3TransformNormal(rayDir, toLocal);

    rayDir = DirectX::XMVector3Normalize(rayDir);

#pragma endregion

#pragma region UpdateObjectList

    // 타겟 데이터만 보일 수 있도록 필터를 적용
    std::vector<ObjectData*> objDatas;

    objDatas.push_back(MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_["DefaultSkybox"]);

    if (target)
        objDatas.push_back(target);

    //objDatas.push_back(MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_["Axis"]);
    //objDatas.push_back(MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_["Bone"]);

    objDatas.push_back(Pivot);
    objDatas.push_back(Bone);

#pragma endregion

    int submeshIDX = 0;

    std::vector<Instance> instBufferDatas;

    float4x4 cubeModelViewMat = {};
    float4x4 inverseModelViewMat = {};
    float3x3 cubeNormalMat = {};

    float4x4 perspectiveMat = {};

#pragma region UpdateBuffer

    cubeModelViewMat =
        CameraPool::getInstance().getCamera().getViewMat();
    inverseModelViewMat =
        CameraPool::getInstance().getCamera().getInvViewMat();
    cubeNormalMat =
        float4x4ToFloat3x3(transpose(inverseModelViewMat));

    // Get window dimensions
    int windowWidth, windowHeight;
    float windowAspectRatio;
    {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        windowWidth = clientRect.right - clientRect.left;
        windowHeight = clientRect.bottom - clientRect.top;
        windowAspectRatio = (float)windowWidth / (float)windowHeight;
    }

    perspectiveMat =
        makePerspectiveMat(
            windowAspectRatio,
            degreesToRadians(60.0f),
            0.1f,
            1000.f
        );

#pragma endregion

    //Bone = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_["Bone"];

    // 마우스 클릭 시 터치된 오브젝트 출력
    if (AnimSequenceKeyVector::getInstance().MouseBehavior[MouseActionLeftDown])
    {
        bool isCatch = false;
        float dist = 0.0f;

        float minDist = 987654321.0f;
        std::string minName = "";

        DirectX::BoundingBox rootBox, bb_buffer;

        rootBox = Bone->collider;

        for (const std::pair<std::string, Transform::transform*>& inst : Bone->transform->InstanceBuffer)
        {
            bb_buffer = rootBox;
            bb_buffer.Center.x += inst.second->LocalMatrix->Position[0];
            bb_buffer.Center.y += inst.second->LocalMatrix->Position[1];
            bb_buffer.Center.z += inst.second->LocalMatrix->Position[2];

            isCatch =
                bb_buffer.Intersects(
                    rayOrigin, rayDir, dist
                );

            if (isCatch && dist < minDist)
            {
                minDist = dist;
                minName = inst.first;
            }
        }

        // 클릭해서 오브젝트를 잡았을 경우
        if (!minName.empty())
        {
            //MemoryPool::getInstance().pickedInstance =
            //    &objIter->second->transpose.instBuffer[minName];
        }
    }

    for (ObjectData* objIter : objDatas)
    {
        if (!objIter)
            continue;

        submeshIDX = 0;
        instBufferDatas.clear();

        if (objIter->bIsEnableDepthStencil)
            CreateWindows::getInstance().d3d11DeviceContext->OMSetDepthStencilState(CreateWindows::getInstance().depthStencilEnableState, 0);
        else
            CreateWindows::getInstance().d3d11DeviceContext->OMSetDepthStencilState(CreateWindows::getInstance().depthStencilDisableState, 0);

        // Update InstanceBuffer
        {
            D3D11_MAPPED_SUBRESOURCE mappedSubresource;
            HRESULT res = CreateWindows::getInstance().d3d11DeviceContext->Map(
                objIter->InstanceBuffer,
                0,
                D3D11_MAP_WRITE_DISCARD,
                0,
                &mappedSubresource
            );

            for (const std::pair<std::string, Transform::transform*>& inst : objIter->transform->InstanceBuffer)
            {
                if (inst.second)
                    instBufferDatas.push_back(*inst.second->LocalMatrix);
            }

            memcpy(
                mappedSubresource.pData,
                instBufferDatas.data(),
                sizeof(Instance) * objIter->transform->InstanceCount
            );

            CreateWindows::getInstance().d3d11DeviceContext->Unmap(objIter->InstanceBuffer, 0);
        }

        for (; submeshIDX < objIter->SubmeshCount; submeshIDX++)
        {
            if (target && target->VertexBuffer.size() <= submeshIDX)
                throw std::runtime_error("메쉬는 반드시 Sperated By Material이여야 함.");

            // 애니메이션 업데이트
            if (target && 0 < target->GetAsset()->mAnimBuffer.size())
            {
                D3D11_MAPPED_SUBRESOURCE mappedSubresource;

                HRESULT hr = CreateWindows::getInstance().d3d11DeviceContext->Map(
                    target->VertexBuffer[submeshIDX],
                    0,
                    D3D11_MAP_WRITE_DISCARD,
                    0,
                    &mappedSubresource
                );


                UINT VertSize = target->meshDatas[submeshIDX].Vertices.size() * sizeof(Vertex);

                memcpy(
                    mappedSubresource.pData,
                    target->GetAsset()->mAnimBuffer[submeshIDX].data(),
                    VertSize
                );

                CreateWindows::getInstance().d3d11DeviceContext->Unmap(target->VertexBuffer[submeshIDX], 0);
            }


            ID3D11Buffer* bufferPointers[2] = {
                    objIter->VertexBuffer[submeshIDX],
                    objIter->InstanceBuffer,
            };

            unsigned int bufferStrides[2] = {
                    objIter->Stride,
                    sizeof(Instance),
            };

            unsigned int bufferOffsets[2] = {
                0,
                0,
            };

            CreateWindows::getInstance().d3d11DeviceContext->IASetVertexBuffers(
                0,
                2,
                bufferPointers,
                bufferStrides,
                bufferOffsets
            );
            CreateWindows::getInstance().d3d11DeviceContext->IASetIndexBuffer(
                objIter->IndexBuffer[submeshIDX],
                DXGI_FORMAT_R32_UINT,
                objIter->IndicesOffset
            );

            {

                CreateWindows::getInstance().d3d11DeviceContext->IASetInputLayout(CreateWindows::getInstance().blinnPhongInputLayout);
                CreateWindows::getInstance().d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                Shader* sd =
                    MemoryPool::getInstance()._GLOBAL_SHADER_POOL[
                        objIter->ShaderID
                    ];

                CreateWindows::getInstance().d3d11DeviceContext->VSSetShader(
                    sd->VertexShader,
                    nullptr, 0
                );
                CreateWindows::getInstance().d3d11DeviceContext->PSSetShader(
                    sd->PixelShader,
                    nullptr, 0
                );

                MaterialUpdate(objIter, submeshIDX);

                // Update vertex shader constant buffer
                {
                    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
                    CreateWindows::getInstance().d3d11DeviceContext->Map(
                        objIter->ObjectVSConstantBuffer,
                        0,
                        D3D11_MAP_WRITE_DISCARD,
                        0,
                        &mappedSubresource
                    );

                    BlinnPhongVSConstants* constants =
                        (BlinnPhongVSConstants*)(mappedSubresource.pData);

                    constants->modelViewProj = cubeModelViewMat * perspectiveMat;
                    constants->modelView = cubeModelViewMat;
                    constants->normalMatrix = cubeNormalMat;

                    CreateWindows::getInstance().d3d11DeviceContext->Unmap(
                        objIter->ObjectVSConstantBuffer, 0
                    );

                    CreateWindows::getInstance().d3d11DeviceContext->VSSetConstantBuffers(
                        0, 1, &objIter->ObjectVSConstantBuffer
                    );
                    CreateWindows::getInstance().d3d11DeviceContext->PSSetConstantBuffers(
                        0, 1, &objIter->ObjectPSConstantBuffer
                    );
                }

                // Update pixel shader constant buffer
                {
                    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
                    CreateWindows::getInstance().d3d11DeviceContext->Map(
                        objIter->ObjectPSConstantBuffer,
                        0,
                        D3D11_MAP_WRITE_DISCARD,
                        0,
                        &mappedSubresource
                    );

                    LightData* gLightData = &MemoryPool::getInstance()._GLOBAL_LIGHT_POOL_;

                    memcpy(
                        mappedSubresource.pData,
                        gLightData,
                        sizeof(LightData)
                    );


                    CreateWindows::getInstance().d3d11DeviceContext->Unmap(
                        objIter->ObjectPSConstantBuffer, 0);
                }

                CreateWindows::getInstance().d3d11DeviceContext->DrawIndexedInstanced(
                    objIter->NumIndices,
                    objIter->transform->InstanceCount,
                    0,
                    0,
                    0
                );
            }
        } // submeshIDX

    } // objIter

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    d3d11SwapChain->Present(1, 0);

}

void AnimationSequence::AnimationUpdate()
{
    UINT submeshIDX = 0;
    UINT lClusterIDX = 0;

    FbxSkin* lSkinDeformer;

    UINT lClusterCount = 0;
    FbxCluster* lCluster;
    std::string lClusterName;
    Tree<Skeleton>* TargetBone;
    Transform::transform* BoneInstance;

    fbxsdk::FbxAMatrix lVertexTransformMatrix;
    int lVertexIndexCount;

    int* mIndices = nullptr;
    double* mWeights = nullptr;

    int i(0), j(0), k(0);
    int VertSize;

    fbxsdk::FbxVector4* vertices;
    std::vector<fbxsdk::FbxAMatrix> lClusterDeformation;

    fbxsdk::FbxVector4 result;

    Tree<Skeleton>*             targetSkeleton;
    Transform::transform*       targetInstance;

    Sequence*       assetSequence;
    Montage*        assetMontage;
    BlendSpace*     assetBlendSpace;
    Composite*      assetComposite;

    /**
    *
    * 선택된 뼈대에 대한 리액션
    * 
    * 만일 뼈대 중 하나가 클릭 되었다면
    * Pivot의 위치를 옮기고, 
    * 
    * 만일 Global 트리거가 false라면
    * Pivot은 뼈대의 회전값 만큼 회전하여 나타난다.
    * 
    */
    if (0 <= BonesListBox.Selected)
    {
        // Pivot 위치 옮기기
        //ObjectData* Pivot = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_["Axis"];
        std::string BoneName = BonesListBox.GetTextListBoxMessage();

        if (!Bone->transform->InstanceBuffer.count(BoneName))
            throw std::runtime_error("선택된 뼈를 찾을 수 없음");

        Transform::transform* pivotInst = Pivot->transform->InstanceBuffer.begin()->second;
        float3 Position;

        Bone->transform->InstanceBuffer[BoneName]->GetLocalPosition(Position);
        pivotInst->SetPosition(Position);

        // 로컬 회전값에 맞춰 피벗을 회전
        if (!GlobalPivot)
        {
            Tree<Skeleton>* TargetBone = target->BoneHierarchy.Find(BoneName);

            if (TargetBone)
            {
                FbxVector4 rotation = TargetBone->Item.TranslationMatrix.toFbxAMatrix().GetR();

                pivotInst->SetRotation(
                    rotation.mData[0],
                    rotation.mData[1],
                    rotation.mData[2]
                );
            }
        }
        else
        {
            pivotInst->SetRotation(
                0.0f,
                0.0f,
                0.0f
            );
        }
    }

    // 애니메이션을 업데이트 한다.
    if (target && target->objType == ObjectType::FBX_SKINNED_TYPE && target->CurrentAsset)
    {
        if (target->CurrentAsset->mType == AnimationAsset::AnimationAssetType::SEQUENCE)
        {
            assetSequence = (Sequence*)target->GetAsset();

            assetSequence->mFrame = frame;
            assetSequence->mAnimBuffer.resize(target->SubmeshCount);

            for (submeshIDX = 0; submeshIDX < target->SubmeshCount; submeshIDX++)
            {
                VertSize = target->meshDatas[submeshIDX].Vertices.size();

                assetSequence->mAnimBuffer[submeshIDX].clear();
                lClusterDeformation.clear();
                assetSequence->mAnimBuffer[submeshIDX].resize(VertSize);
                lClusterDeformation.resize(VertSize);

                vertices = target->meshFBXDatas[submeshIDX]->GetControlPoints();
                lSkinDeformer =
                    (FbxSkin*)target->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

                lClusterCount =
                    lSkinDeformer->GetClusterCount();

                for (lClusterIDX = 0; lClusterIDX < lClusterCount; lClusterIDX++)
                {
                    lCluster = lSkinDeformer->GetCluster(lClusterIDX);
                    lClusterName = std::string(lCluster->GetNameOnly());

                    targetSkeleton = target->BoneHierarchy.Find(lClusterName);

                    assetSequence->GetAnimationFBXMatrix(submeshIDX, lClusterIDX, lVertexTransformMatrix);

#pragma region 델타 적용

                    //TargetBone = target->BoneHierarchy.Find(lClusterName);

                    //if (TargetBone)
                    //{
                    //    fbxsdk::FbxAMatrix WeightMat = TargetBone->Item.DeltaWeightMatrix.toFbxAMatrix();

                    //    lVertexTransformMatrix = 
                    //        lVertexTransformMatrix * WeightMat;

                    //    for (int matIter = 0; matIter < 4; matIter++)
                    //    {
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][0] =
                    //            lVertexTransformMatrix.mData[matIter].mData[0];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][1] =
                    //            lVertexTransformMatrix.mData[matIter].mData[1];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][2] =
                    //            lVertexTransformMatrix.mData[matIter].mData[2];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][3] =
                    //            lVertexTransformMatrix.mData[matIter].mData[3];
                    //    }

                    //}

#pragma endregion

                    mIndices = lCluster->GetControlPointIndices();
                    lVertexIndexCount = lCluster->GetControlPointIndicesCount();

                    if (!mIndices)
                        continue;

                    mWeights = lCluster->GetControlPointWeights();

                    for (k = 0; k < lVertexIndexCount; ++k)
                    {
                        for (i = 0; i < 4; i++)
                            for (j = 0; j < 4; j++)
                                lClusterDeformation[mIndices[k]][i][j] +=
                                lVertexTransformMatrix[i][j] * mWeights[k];
                    }

                    if (Bone)
                    {
                        BoneInstance = Bone->transform->InstanceBuffer[lClusterName];

                        if (BoneInstance)
                        {
                            FbxVector4 BonePosition =
                                targetSkeleton->Item.TranslationMatrix.toFbxAMatrix().MultT(vertices[mIndices[0]]);

                            BoneInstance->SetPosition(
                                BonePosition.mData[0],
                                BonePosition.mData[1],
                                BonePosition.mData[2]
                            );

                        }
                    }

                } // ClusterCount

                for (i = 0; i < VertSize; i++)
                {
                    result = lClusterDeformation[i].MultT(vertices[i]);
                    result = result / target->WeightAccumulation[submeshIDX][i];

                    memcpy(
                        &assetSequence->mAnimBuffer[submeshIDX][i],
                        &target->meshDatas[submeshIDX].Vertices[i],
                        sizeof(Vertex)
                    );

                    assetSequence->mAnimBuffer[submeshIDX][i].Position[0] =
                        result.mData[0];
                    assetSequence->mAnimBuffer[submeshIDX][i].Position[1] =
                        result.mData[1];
                    assetSequence->mAnimBuffer[submeshIDX][i].Position[2] =
                        result.mData[2];
                }

                int indexSize = target->meshDatas[submeshIDX].Indices32.size();
                Vertex* vertices = assetSequence->mAnimBuffer[submeshIDX].data();
                uint32* indices = target->meshDatas[submeshIDX].Indices32.data();
                float3 V1, V2, V3;

                for (int i = 0; i < indexSize - 2; i += 3)
                {
                    V1.x = vertices[indices[i]].Position[0] -
                        vertices[indices[i + 1]].Position[0];
                    V1.y = vertices[indices[i]].Position[1] -
                        vertices[indices[i + 1]].Position[1];
                    V1.z = vertices[indices[i]].Position[2] -
                        vertices[indices[i + 1]].Position[2];

                    V2.x = vertices[indices[i]].Position[0] -
                        vertices[indices[i + 2]].Position[0];
                    V2.y = vertices[indices[i]].Position[1] -
                        vertices[indices[i + 2]].Position[1];
                    V2.z = vertices[indices[i]].Position[2] -
                        vertices[indices[i + 2]].Position[2];

                    V3 = cross(V1, V2);
                    V3 = normalise(V3);

                    vertices[indices[i]].Normal[0] = V3.x;
                    vertices[indices[i]].Normal[1] = V3.y;
                    vertices[indices[i]].Normal[2] = V3.z;

                    vertices[indices[i + 1]].Normal[0] = V3.x;
                    vertices[indices[i + 1]].Normal[1] = V3.y;
                    vertices[indices[i + 1]].Normal[2] = V3.z;

                    vertices[indices[i + 2]].Normal[0] = V3.x;
                    vertices[indices[i + 2]].Normal[1] = V3.y;
                    vertices[indices[i + 2]].Normal[2] = V3.z;
                }
            } // SubmeshCount
        }
        else if (target->CurrentAsset->mType == AnimationAsset::AnimationAssetType::MONTAGE)
        {
            assetMontage = (Montage*)target->GetAsset();

            assetMontage->mFrame = frame;
            assetMontage->mAnimBuffer.resize(target->SubmeshCount);

            for (submeshIDX = 0; submeshIDX < target->SubmeshCount; submeshIDX++)
            {
                VertSize = target->meshDatas[submeshIDX].Vertices.size();

                assetMontage->mAnimBuffer[submeshIDX].clear();
                lClusterDeformation.clear();
                assetMontage->mAnimBuffer[submeshIDX].resize(VertSize);
                lClusterDeformation.resize(VertSize);

                vertices = target->meshFBXDatas[submeshIDX]->GetControlPoints();
                lSkinDeformer =
                    (FbxSkin*)target->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

                lClusterCount =
                    lSkinDeformer->GetClusterCount();

                for (lClusterIDX = 0; lClusterIDX < lClusterCount; lClusterIDX++)
                {
                    lCluster = lSkinDeformer->GetCluster(lClusterIDX);
                    lClusterName = std::string(lCluster->GetNameOnly());

                    targetSkeleton = target->BoneHierarchy.Find(lClusterName);

                    assetMontage->GetAnimationFBXMatrix(submeshIDX, lClusterIDX, lVertexTransformMatrix);

#pragma region 델타 적용

                    //TargetBone = target->BoneHierarchy.Find(lClusterName);

                    //if (TargetBone)
                    //{
                    //    fbxsdk::FbxAMatrix WeightMat = TargetBone->Item.DeltaWeightMatrix.toFbxAMatrix();

                    //    lVertexTransformMatrix = 
                    //        lVertexTransformMatrix * WeightMat;

                    //    for (int matIter = 0; matIter < 4; matIter++)
                    //    {
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][0] =
                    //            lVertexTransformMatrix.mData[matIter].mData[0];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][1] =
                    //            lVertexTransformMatrix.mData[matIter].mData[1];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][2] =
                    //            lVertexTransformMatrix.mData[matIter].mData[2];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][3] =
                    //            lVertexTransformMatrix.mData[matIter].mData[3];
                    //    }

                    //}

#pragma endregion

                    mIndices = lCluster->GetControlPointIndices();
                    lVertexIndexCount = lCluster->GetControlPointIndicesCount();

                    if (!mIndices)
                        continue;

                    mWeights = lCluster->GetControlPointWeights();

                    for (k = 0; k < lVertexIndexCount; ++k)
                    {
                        for (i = 0; i < 4; i++)
                            for (j = 0; j < 4; j++)
                                lClusterDeformation[mIndices[k]][i][j] +=
                                lVertexTransformMatrix[i][j] * mWeights[k];
                    }

                    if (Bone)
                    {
                        BoneInstance = Bone->transform->InstanceBuffer[lClusterName];

                        if (BoneInstance)
                        {
                            FbxVector4 BonePosition =
                                targetSkeleton->Item.TranslationMatrix.toFbxAMatrix().MultT(vertices[mIndices[0]]);

                            BoneInstance->SetPosition(
                                BonePosition.mData[0],
                                BonePosition.mData[1],
                                BonePosition.mData[2]
                            );
                        }
                    }

                } // ClusterCount

                for (i = 0; i < VertSize; i++)
                {
                    result = lClusterDeformation[i].MultT(vertices[i]);
                    result = result / target->WeightAccumulation[submeshIDX][i];

                    memcpy(
                        &assetMontage->mAnimBuffer[submeshIDX][i],
                        &target->meshDatas[submeshIDX].Vertices[i],
                        sizeof(Vertex)
                    );

                    assetMontage->mAnimBuffer[submeshIDX][i].Position[0] =
                        result.mData[0];
                    assetMontage->mAnimBuffer[submeshIDX][i].Position[1] =
                        result.mData[1];
                    assetMontage->mAnimBuffer[submeshIDX][i].Position[2] =
                        result.mData[2];
                }

                int indexSize = target->meshDatas[submeshIDX].Indices32.size();
                Vertex* vertices = assetMontage->mAnimBuffer[submeshIDX].data();
                uint32* indices = target->meshDatas[submeshIDX].Indices32.data();
                float3 V1, V2, V3;

                for (int i = 0; i < indexSize - 2; i += 3)
                {
                    V1.x = vertices[indices[i]].Position[0] -
                        vertices[indices[i + 1]].Position[0];
                    V1.y = vertices[indices[i]].Position[1] -
                        vertices[indices[i + 1]].Position[1];
                    V1.z = vertices[indices[i]].Position[2] -
                        vertices[indices[i + 1]].Position[2];

                    V2.x = vertices[indices[i]].Position[0] -
                        vertices[indices[i + 2]].Position[0];
                    V2.y = vertices[indices[i]].Position[1] -
                        vertices[indices[i + 2]].Position[1];
                    V2.z = vertices[indices[i]].Position[2] -
                        vertices[indices[i + 2]].Position[2];

                    V3 = cross(V1, V2);
                    V3 = normalise(V3);

                    vertices[indices[i]].Normal[0] = V3.x;
                    vertices[indices[i]].Normal[1] = V3.y;
                    vertices[indices[i]].Normal[2] = V3.z;

                    vertices[indices[i + 1]].Normal[0] = V3.x;
                    vertices[indices[i + 1]].Normal[1] = V3.y;
                    vertices[indices[i + 1]].Normal[2] = V3.z;

                    vertices[indices[i + 2]].Normal[0] = V3.x;
                    vertices[indices[i + 2]].Normal[1] = V3.y;
                    vertices[indices[i + 2]].Normal[2] = V3.z;
                }
            } // SubmeshCount
        }
        else if (target->CurrentAsset->mType == AnimationAsset::AnimationAssetType::BLENDSPACE)
        {
            assetBlendSpace = (BlendSpace*)target->GetAsset();

            assetBlendSpace->mFrame = frame;
            assetBlendSpace->mAnimBuffer.resize(target->SubmeshCount);

            for (submeshIDX = 0; submeshIDX < target->SubmeshCount; submeshIDX++)
            {
                VertSize = target->meshDatas[submeshIDX].Vertices.size();

                assetBlendSpace->mAnimBuffer[submeshIDX].clear();
                lClusterDeformation.clear();
                assetBlendSpace->mAnimBuffer[submeshIDX].resize(VertSize);
                lClusterDeformation.resize(VertSize);

                vertices = target->meshFBXDatas[submeshIDX]->GetControlPoints();
                lSkinDeformer =
                    (FbxSkin*)target->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

                lClusterCount =
                    lSkinDeformer->GetClusterCount();

                for (lClusterIDX = 0; lClusterIDX < lClusterCount; lClusterIDX++)
                {
                    lCluster = lSkinDeformer->GetCluster(lClusterIDX);
                    lClusterName = std::string(lCluster->GetNameOnly());

                    targetSkeleton = target->BoneHierarchy.Find(lClusterName);

                    assetBlendSpace->GetAnimationFBXMatrix(submeshIDX, lClusterIDX, lVertexTransformMatrix);

#pragma region 델타 적용

                    //TargetBone = target->BoneHierarchy.Find(lClusterName);

                    //if (TargetBone)
                    //{
                    //    fbxsdk::FbxAMatrix WeightMat = TargetBone->Item.DeltaWeightMatrix.toFbxAMatrix();

                    //    lVertexTransformMatrix = 
                    //        lVertexTransformMatrix * WeightMat;

                    //    for (int matIter = 0; matIter < 4; matIter++)
                    //    {
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][0] =
                    //            lVertexTransformMatrix.mData[matIter].mData[0];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][1] =
                    //            lVertexTransformMatrix.mData[matIter].mData[1];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][2] =
                    //            lVertexTransformMatrix.mData[matIter].mData[2];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][3] =
                    //            lVertexTransformMatrix.mData[matIter].mData[3];
                    //    }

                    //}

#pragma endregion

                    mIndices = lCluster->GetControlPointIndices();
                    lVertexIndexCount = lCluster->GetControlPointIndicesCount();

                    if (!mIndices)
                        continue;

                    mWeights = lCluster->GetControlPointWeights();

                    for (k = 0; k < lVertexIndexCount; ++k)
                    {
                        for (i = 0; i < 4; i++)
                            for (j = 0; j < 4; j++)
                                lClusterDeformation[mIndices[k]][i][j] +=
                                lVertexTransformMatrix[i][j] * mWeights[k];
                    }

                    if (Bone)
                    {
                        BoneInstance = Bone->transform->InstanceBuffer[lClusterName];

                        if (BoneInstance)
                        {
                            FbxVector4 BonePosition =
                                targetSkeleton->Item.TranslationMatrix.toFbxAMatrix().MultT(vertices[mIndices[0]]);

                            BoneInstance->SetPosition(
                                BonePosition.mData[0],
                                BonePosition.mData[1],
                                BonePosition.mData[2]
                            );

                        }
                    }

                } // ClusterCount

                for (i = 0; i < VertSize; i++)
                {
                    result = lClusterDeformation[i].MultT(vertices[i]);
                    result = result / target->WeightAccumulation[submeshIDX][i];

                    memcpy(
                        &assetBlendSpace->mAnimBuffer[submeshIDX][i],
                        &target->meshDatas[submeshIDX].Vertices[i],
                        sizeof(Vertex)
                    );

                    assetBlendSpace->mAnimBuffer[submeshIDX][i].Position[0] =
                        result.mData[0];
                    assetBlendSpace->mAnimBuffer[submeshIDX][i].Position[1] =
                        result.mData[1];
                    assetBlendSpace->mAnimBuffer[submeshIDX][i].Position[2] =
                        result.mData[2];
                }

                int indexSize = target->meshDatas[submeshIDX].Indices32.size();
                Vertex* vertices = assetBlendSpace->mAnimBuffer[submeshIDX].data();
                uint32* indices = target->meshDatas[submeshIDX].Indices32.data();
                float3 V1, V2, V3;

                for (int i = 0; i < indexSize - 2; i += 3)
                {
                    V1.x = vertices[indices[i]].Position[0] -
                        vertices[indices[i + 1]].Position[0];
                    V1.y = vertices[indices[i]].Position[1] -
                        vertices[indices[i + 1]].Position[1];
                    V1.z = vertices[indices[i]].Position[2] -
                        vertices[indices[i + 1]].Position[2];

                    V2.x = vertices[indices[i]].Position[0] -
                        vertices[indices[i + 2]].Position[0];
                    V2.y = vertices[indices[i]].Position[1] -
                        vertices[indices[i + 2]].Position[1];
                    V2.z = vertices[indices[i]].Position[2] -
                        vertices[indices[i + 2]].Position[2];

                    V3 = cross(V1, V2);
                    V3 = normalise(V3);

                    vertices[indices[i]].Normal[0] = V3.x;
                    vertices[indices[i]].Normal[1] = V3.y;
                    vertices[indices[i]].Normal[2] = V3.z;

                    vertices[indices[i + 1]].Normal[0] = V3.x;
                    vertices[indices[i + 1]].Normal[1] = V3.y;
                    vertices[indices[i + 1]].Normal[2] = V3.z;

                    vertices[indices[i + 2]].Normal[0] = V3.x;
                    vertices[indices[i + 2]].Normal[1] = V3.y;
                    vertices[indices[i + 2]].Normal[2] = V3.z;
                }
            } // SubmeshCount
        }
        // 만일 타겟의 애님에셋이 Composite 형식이라면
        else if (target->CurrentAsset->mType == AnimationAsset::AnimationAssetType::COMPOSITE)
        {
            // 애님 에셋을 Composite 형으로 가져와서
            assetComposite = (Composite*)target->GetAsset();

            // 현재 프레임과 애니메이션 버퍼를 최신화 하고
            assetComposite->mFrame = frame;
            assetComposite->mAnimBuffer.resize(target->SubmeshCount);

            for (submeshIDX = 0; submeshIDX < target->SubmeshCount; submeshIDX++)
            {
                VertSize = target->meshDatas[submeshIDX].Vertices.size();

                assetComposite->mAnimBuffer[submeshIDX].clear();
                lClusterDeformation.clear();
                assetComposite->mAnimBuffer[submeshIDX].resize(VertSize);
                lClusterDeformation.resize(VertSize);

                // Default Tranlation의 버텍스 포인트를 가져온다. (클러스터 단위로)
                vertices = target->meshFBXDatas[submeshIDX]->GetControlPoints();
                lSkinDeformer =
                    (FbxSkin*)target->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

                lClusterCount =
                    lSkinDeformer->GetClusterCount();

                // 클러스터(군집)에서 디폼 즉 변형 요소(ex> 뼈대) 만큼 반복
                for (lClusterIDX = 0; lClusterIDX < lClusterCount; lClusterIDX++)
                {
                    lCluster = lSkinDeformer->GetCluster(lClusterIDX);
                    lClusterName = std::string(lCluster->GetNameOnly());

                    // 현 뼈대에 대한 트리를 찾는다.
                    // 컴포지트를 구현하기 위해 중심이 되는 뼈대 상 하위를 여기서 결정한다.
                    targetSkeleton = target->BoneHierarchy.Find(lClusterName);

                    // 블렌딩을 할 것인지 말 것인지 즉 현 뼈대가 기준 뼈대의 상위인지 하위인지를 결정하고 최신화 한다.
                    assetComposite->mIsBlending = targetSkeleton->IsParent(assetComposite->mPivotBoneName);

                    // Composite이 적용된 다음 뼈대(디포머) 매트릭스를 읽어 온다. 
                    assetComposite->GetAnimationFBXMatrix(submeshIDX, lClusterIDX, lVertexTransformMatrix);

#pragma region 델타 적용

                    //TargetBone = target->BoneHierarchy.Find(lClusterName);

                    //if (TargetBone)
                    //{
                    //    fbxsdk::FbxAMatrix WeightMat = TargetBone->Item.DeltaWeightMatrix.toFbxAMatrix();

                    //    lVertexTransformMatrix = 
                    //        lVertexTransformMatrix * WeightMat;

                    //    for (int matIter = 0; matIter < 4; matIter++)
                    //    {
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][0] =
                    //            lVertexTransformMatrix.mData[matIter].mData[0];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][1] =
                    //            lVertexTransformMatrix.mData[matIter].mData[1];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][2] =
                    //            lVertexTransformMatrix.mData[matIter].mData[2];
                    //        targetSkeleton->Item.TranslationMatrix.m[matIter][3] =
                    //            lVertexTransformMatrix.mData[matIter].mData[3];
                    //    }

                    //}

#pragma endregion

                    // 뼈대(디포머) 블렌딩을 위해 인덱스를 얻어온다.
                    mIndices = lCluster->GetControlPointIndices();
                    lVertexIndexCount = lCluster->GetControlPointIndicesCount();

                    if (!mIndices)
                        continue;

                    // 블렌딩 가중치 습득
                    mWeights = lCluster->GetControlPointWeights();

                    // 각 버텍스가 디포머를 두 개 이상 가지고 있을 경우 다음과 같이 블렌딩 한다.
                    for (k = 0; k < lVertexIndexCount; ++k)
                    {
                        for (i = 0; i < 4; i++)
                            for (j = 0; j < 4; j++)
                                lClusterDeformation[mIndices[k]][i][j] +=
                                lVertexTransformMatrix[i][j] * mWeights[k];
                    }

                    // 타겟의 뼈대 위치를 표시해 주는 빨간 점
                    if (Bone)
                    {
                        BoneInstance = Bone->transform->InstanceBuffer[lClusterName];

                        if (BoneInstance)
                        {
                            FbxVector4 BonePosition =
                                targetSkeleton->Item.TranslationMatrix.toFbxAMatrix().MultT(vertices[mIndices[0]]);

                            BoneInstance->SetPosition(
                                BonePosition.mData[0],
                                BonePosition.mData[1],
                                BonePosition.mData[2]
                            );

                        }
                    }

                } // ClusterCount

                for (i = 0; i < VertSize; i++)
                {
                    result = lClusterDeformation[i].MultT(vertices[i]);
                    result = result / target->WeightAccumulation[submeshIDX][i];

                    memcpy(
                        &assetComposite->mAnimBuffer[submeshIDX][i],
                        &target->meshDatas[submeshIDX].Vertices[i],
                        sizeof(Vertex)
                    );

                    assetComposite->mAnimBuffer[submeshIDX][i].Position[0] =
                        result.mData[0];
                    assetComposite->mAnimBuffer[submeshIDX][i].Position[1] =
                        result.mData[1];
                    assetComposite->mAnimBuffer[submeshIDX][i].Position[2] =
                        result.mData[2];
                }

                int indexSize = target->meshDatas[submeshIDX].Indices32.size();
                Vertex* vertices = assetComposite->mAnimBuffer[submeshIDX].data();
                uint32* indices = target->meshDatas[submeshIDX].Indices32.data();
                float3 V1, V2, V3;

                for (i = 0; i < indexSize - 2; i += 3)
                {
                    V1.x = vertices[indices[i]].Position[0] -
                        vertices[indices[i + 1]].Position[0];
                    V1.y = vertices[indices[i]].Position[1] -
                        vertices[indices[i + 1]].Position[1];
                    V1.z = vertices[indices[i]].Position[2] -
                        vertices[indices[i + 1]].Position[2];

                    V2.x = vertices[indices[i]].Position[0] -
                        vertices[indices[i + 2]].Position[0];
                    V2.y = vertices[indices[i]].Position[1] -
                        vertices[indices[i + 2]].Position[1];
                    V2.z = vertices[indices[i]].Position[2] -
                        vertices[indices[i + 2]].Position[2];

                    V3 = cross(V1, V2);
                    V3 = normalise(V3);

                    vertices[indices[i]].Normal[0] = V3.x;
                    vertices[indices[i]].Normal[1] = V3.y;
                    vertices[indices[i]].Normal[2] = V3.z;

                    vertices[indices[i + 1]].Normal[0] = V3.x;
                    vertices[indices[i + 1]].Normal[1] = V3.y;
                    vertices[indices[i + 1]].Normal[2] = V3.z;

                    vertices[indices[i + 2]].Normal[0] = V3.x;
                    vertices[indices[i + 2]].Normal[1] = V3.y;
                    vertices[indices[i + 2]].Normal[2] = V3.z;
                }
            } // SubmeshCount
        }

        // 타겟의 각 관절 위치로 Bone을 이동시킨다
        if (Bone && Pivot)
        {
            targetInstance  = Bone->transform->InstanceBuffer[BonesListBox.GetTextListBoxMessage()];
            BoneInstance    = Pivot->transform->InstanceBuffer.begin()->second;

            if (targetInstance && BoneInstance)
            {
                float3 Position;

                targetInstance->GetLocalPosition(Position);
                BoneInstance->SetPosition(Position);
            }
        }
    }

    // 애니메이션 플레이 버튼이 켜져 있다면 자동 플레이
    if (target && AnimPlay)
    {
        frame = (frame + 1) < maxFrame ? (frame + 1) : 0;
    }
}

void AnimationSequence::MaterialUpdate(ObjectData* objIter, UINT submeshIDX)
{
#pragma region Update Material

        ID3D11ShaderResourceView* srv = nullptr;

        // 텍스쳐 슬롯을 채울 때 마다 값이 하나씩 누적된다.
        UINT AccumulateSlotIndex = 0;
        // 머테리얼에서 텍스쳐 이터레이터용 변수
        UINT TextureIndex = 0;
        Material* mat = nullptr;

        // 서브메쉬 기준으로 나누어 머테리얼을 수정한다.
        AccumulateSlotIndex = 0;
        TextureIndex = 0;
        mat = &objIter->meshDatas[submeshIDX].mMaterial;

        // 베이스 텍스쳐가 없다면 아무 텍스쳐나 입힘
        if (!mat->BaseColorTexutures.size())
        {
            srv =
                MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                    MemoryPool::getInstance().TextureNames[0]
                ];

            CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                AccumulateSlotIndex,
                1,
                &srv
            );
            CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                AccumulateSlotIndex,
                1,
                &CreateWindows::getInstance().samplerState
            );

            AccumulateSlotIndex++;
        }
        // 베이스 텍스쳐가 있다면
        else
        {
            // 베이스 텍스쳐 개수만큼 집어 넣음
            for (TextureIndex = 0; TextureIndex < mat->BaseColorTexutures.size(); TextureIndex++)
            {
                srv =
                    MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                        mat->BaseColorTexutures[TextureIndex]
                    ];

                CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                    AccumulateSlotIndex,
                    1,
                    &srv
                );
                CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                    AccumulateSlotIndex,
                    1,
                    &CreateWindows::getInstance().samplerState
                );

                AccumulateSlotIndex++;
            }
        }

        // 노말 텍스쳐가 없다면, 넣지 않음
        // 노말 텍스쳐가 있다면
        if (mat->NormalTexutures.size())
        {
            // 노말 텍스쳐 개수만큼 집어 넣음
            for (TextureIndex = 0; TextureIndex < mat->NormalTexutures.size(); TextureIndex++)
            {
                srv =
                    MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                        mat->NormalTexutures[TextureIndex]
                    ];

                CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                    AccumulateSlotIndex,
                    1,
                    &srv
                );
                CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                    AccumulateSlotIndex,
                    1,
                    &CreateWindows::getInstance().samplerState
                );

                AccumulateSlotIndex++;
            }
        }

        // 마스크 텍스쳐가 없다면, 넣지 않음
        // 마스크 텍스쳐가 있다면
        if (mat->MaskedTextures.size())
        {
            // 마스크 텍스쳐 개수만큼 집어 넣음
            for (TextureIndex = 0; TextureIndex < mat->MaskedTextures.size(); TextureIndex++)
            {
                srv =
                    MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                        mat->MaskedTextures[TextureIndex]
                    ];

                CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                    AccumulateSlotIndex,
                    1,
                    &srv
                );
                CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                    AccumulateSlotIndex,
                    1,
                    &CreateWindows::getInstance().samplerState
                );

                AccumulateSlotIndex++;
            }
        }

        // 메탈릭 텍스쳐가 없다면, 넣지 않음
        // 메탈릭 텍스쳐가 있다면
        if (mat->MetallicTextures.size())
        {
            // 마스크 텍스쳐 개수만큼 집어 넣음
            for (TextureIndex = 0; TextureIndex < mat->MetallicTextures.size(); TextureIndex++)
            {
                srv =
                    MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                        mat->MetallicTextures[TextureIndex]
                    ];

                CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                    AccumulateSlotIndex,
                    1,
                    &srv
                );
                CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                    AccumulateSlotIndex,
                    1,
                    &CreateWindows::getInstance().samplerState
                );

                AccumulateSlotIndex++;
            }
        }

        // 러프니스 텍스쳐가 없다면, 넣지 않음
        // 러프니스 텍스쳐가 있다면
        if (mat->RoughnessTextures.size())
        {
            // 마스크 텍스쳐 개수만큼 집어 넣음
            for (TextureIndex = 0; TextureIndex < mat->RoughnessTextures.size(); TextureIndex++)
            {
                srv =
                    MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                        mat->RoughnessTextures[TextureIndex]
                    ];

                CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                    AccumulateSlotIndex,
                    1,
                    &srv
                );
                CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                    AccumulateSlotIndex,
                    1,
                    &CreateWindows::getInstance().samplerState
                );

                AccumulateSlotIndex++;
            }
        }

#pragma endregion

}

void AnimationSequence::AnimationSequenceUpdate()
{
    ImGui::Begin("Animation Sequence"); 

    ImGui::Text("Animation Sequence");   
    ImGui::Checkbox("Play", &AnimPlay);
    ImGui::Checkbox("GlobalPivot", &GlobalPivot);

    ImGui::SliderInt("Frame", &frame, 0, maxFrame - 1);

    ImGui::End();
}

// 클립의 최대, 최소 프레임을 기억해둡니다
int MinStartFrame = 0;
int MaxEndFrame = 0;
// 선택된 클립의 시작, 끝 프레임을 변경하기 위한 포인트 
int* StartFrame = nullptr;
int* EndFrame = nullptr;

void AnimationSequence::AnimationMontageUpdate()
{
    ImGui::Begin("Animation Montage");

    ImGui::Text("Animation Montage");
    ImGui::Checkbox("Play", &AnimPlay);
    ImGui::Checkbox("GlobalPivot", &GlobalPivot);

    // 리스트박스가 눌려 졌을때
    if (bIsListBoxUpdate)
    {
        if (bIsListBoxUpdate->name == "SequenceListBox")
        {
            target->SetAsset(
                (AnimationAsset*)(target->Sequences[bIsListBoxUpdate->Selected]));
        }
        else if (bIsListBoxUpdate->name == "MontageListBox")
        {
            target->SetAsset(
                (AnimationAsset*)(&target->Montages[bIsListBoxUpdate->GetTextListBoxMessage()]));

            MontageSeqListBox.ResetListBoxMessage();

            Montage* mont = (Montage*)target->GetAsset();
            for (Montage::SequenceClip& clip : mont->mSequenceList)
            {
                MontageSeqListBox.AppendListBoxMessage(clip.mSequence->mName);
            }
        }

        frame = 0;
        maxFrame = target->GetAsset()->mFrameLength;

        bIsListBoxUpdate = nullptr;
    }
    
    std::string SelectedName = AnimationSequence::getInstance().MontageListBox.GetTextListBoxMessage();
    ObjectData* Target = AnimationSequence::getInstance().GetTarget();

    // 만일 StartFrame, EndFrame의 변경이 감지되면
    if (StartFrame)
    {
        ImGui::InputInt("Start", StartFrame, 1, 1);
        ImGui::InputInt("End", EndFrame, 1, 1);

        // 블랜딩이 가변적으로 변화할 수 있도록 알고리즘을 만들어 줘야함.
        *StartFrame = (MinStartFrame <= *StartFrame) ? *StartFrame : MinStartFrame;
        *EndFrame = (*EndFrame < MaxEndFrame) ? *EndFrame : MaxEndFrame;
  
        if (*StartFrame)
        {

        }
        if (*EndFrame < MaxEndFrame)
        {

        }
    }

    for (Montage::SequenceClip& clip : Target->Montages[SelectedName].mSequenceList)
    {
        if(clip.mClipStartFrame <= frame && frame < clip.mClipEndFrame )
            ImGui::SliderInt(
                clip.mSequence->mName.c_str(), 
                &frame, 
                clip.mClipStartFrame, 
                clip.mClipEndFrame
            );
        else
            ImGui::SliderInt(
                clip.mSequence->mName.c_str(), 
                (int*)(&clip.mClipStartFrame),
                clip.mClipStartFrame,
                clip.mClipEndFrame
            );
    }

    Target->Montages[SelectedName].mCurrentSequenceIndex =
        (Target->Montages[SelectedName].mCurrentSequenceIndex + 1) < Target->Montages[SelectedName].mSequenceList.size() ?
        (Target->Montages[SelectedName].mCurrentSequenceIndex + 1) : 0;

    ImGui::End();
}

// 클립의 최대, 최소 프레임을 기억해둡니다
float PreviousX = 0;
float PreviousY = 0;
// 선택된 클립의 시작, 끝 프레임을 변경하기 위한 포인트 
float* PositionX = nullptr;
float* PositionY = nullptr;

void AnimationSequence::AnimationBlendSpaceUpdate()
{
    ImGui::Begin("Animation BlendSpace");

    ImGui::Text("Animation BlendSpace");
    ImGui::Checkbox("Play", &AnimPlay);
    ImGui::Checkbox("GlobalPivot", &GlobalPivot);

    // 리스트박스가 눌려 졌을때
    if (bIsListBoxUpdate)
    {
        // 시퀀스가 눌려졌다면
        if (bIsListBoxUpdate->name == "SequenceListBox1")
        {
            // 시퀀스 실행
            target->SetAsset(
                (AnimationAsset*)(target->Sequences[bIsListBoxUpdate->Selected]));
        }
        // 블랜드 스페이스가 눌려졌다면
        else if (bIsListBoxUpdate->name == "BlendSpaceListBox")
        {
            // 블랜드 스페이스 실행
            target->SetAsset(
                (AnimationAsset*)(&target->BlendSpaces[bIsListBoxUpdate->GetTextListBoxMessage()]));

            // 이전의 블랜드 스페이스의 시퀀스 리스트를 모두 리셋하고
            BlendSpaceSeqListBox.ResetListBoxMessage();
            // 현재 선택된 블랜드 스페이스의 시퀸스 리스트를 다시 보여준다.
            BlendSpace* clips = (BlendSpace*)target->GetAsset();
            for (BlendSpace::SequenceClip& clip : clips->mClips)
            {
                BlendSpaceSeqListBox.AppendListBoxMessage(clip.mSequence->mName);
            }
        }

        frame = 0;
        maxFrame = target->GetAsset()->mFrameLength;

        bIsListBoxUpdate = nullptr;
    }

    if (PositionX)
    {
        ImGui::InputFloat("PositionX", PositionX);
        ImGui::InputFloat("PositionY", PositionY);
    }

    std::string SelectedName = AnimationSequence::getInstance().BlendSpaceListBox.GetTextListBoxMessage();
    ObjectData* Target = AnimationSequence::getInstance().GetTarget();

    Target->BlendSpaces[SelectedName].mFrame =
        (Target->BlendSpaces[SelectedName].mFrame + 1) < Target->BlendSpaces[SelectedName].mFrameLength ?
        (Target->BlendSpaces[SelectedName].mFrame + 1) : 0;

    ImGui::End();
}

// 현재 선택하고 있는 모프의 Weight Point 
float* Weight = nullptr;

void AnimationSequence::AnimationMorphUpdate()
{
    ImGui::Begin("Animation Morph");
    ImGui::Text("Animation Morph");

    // 리스트박스가 눌려 졌을때
    if (bIsListBoxUpdate)
    {
        // 모프 리스트가 눌려졌다면
        if (bIsListBoxUpdate->name == "MorphListBox")
        {
            Morph& item = target->Morphes[MorphListBox.GetTextListBoxMessage()];

            Weight = &item.mWeight;
        }

        bIsListBoxUpdate = nullptr;
    }

    if (Weight)
    {
        ImGui::InputFloat("Weight", Weight);
    }

    std::string SelectedName = AnimationSequence::getInstance().BlendSpaceListBox.GetTextListBoxMessage();
    ObjectData* Target = AnimationSequence::getInstance().GetTarget();

    Target->BlendSpaces[SelectedName].mFrame =
        (Target->BlendSpaces[SelectedName].mFrame + 1) < Target->BlendSpaces[SelectedName].mFrameLength ?
        (Target->BlendSpaces[SelectedName].mFrame + 1) : 0;

    ImGui::End();
}