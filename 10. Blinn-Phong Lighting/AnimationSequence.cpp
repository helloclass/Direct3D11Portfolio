#include "AnimationSequence.h"

// Make to Singleton AnimationSequence
AnimationSequence AnimationSequence::instance;
AnimSequenceKeyVector AnimSequenceKeyVector::instance;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace _ANIMATION_BUTTON_FUNC {

    //////////////////////////////////////////////////////
    // ��Ÿ��, ���� �����̽� ������ ������ ���
    // �̸��� ���� �ε����� �����ϱ� ���� ��������
    //////////////////////////////////////////////////////
    static UINT _MONTAGE_INDEX_ACCUMULATION = 0;
    static UINT _BLEND_SPACE_INDEX_ACCUMULATION = 0;
    static UINT _COMPOSITE_INDEX_ACCUMULATION = 0;

#pragma region Sequence_Function

    // �������� ���뿡 ��Ÿ Ű �߰� / ����
    static void AddKeyButtonFunc()
    {

    }
    // �������� ��Ÿ ����� �ִϸ��̼� ����
    static void SaveKeyButtonFunc()
    {

    }

#pragma endregion

#pragma region Montage_Function

    // ��Ÿ�� ���� ��ư
    static void SaveMontageButtonFunc(ObjectData* obj)
    {
        std::string DirPath =
            "MetaData\\" + obj->objectName + "\\Montage\\";
        std::string Path;

        // ���� ���� �Ǵ� ����
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
                // ������ �̸� ����
                MontageFile.write(clip.mSequence->mName.c_str(), sizeof(char*) * clip.mSequence->mName.size());
                // ������ ���� ������ ����
                UintToString = std::to_string(clip.mClipStartFrame);
                MontageFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
                // ������ �� ������ ����
                UintToString = std::to_string(clip.mClipEndFrame);
                MontageFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
            }

            MontageFile.close();
        }
    }

    // ��Ÿ�� ������ư
    static void CreateMontageButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
        if (0 <= AnimationSequence::getInstance().SequenceListBox.Selected)
        {
            Montage newMontage;
            UINT SequenceIndex = AnimationSequence::getInstance().SequenceListBox.Selected;
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // UI ListBox���� ������ �μ��� ���� �������� ������ �ٸ� ���
            if (Target->Sequences.size() <= SequenceIndex)
                throw std::runtime_error("���� �̽�! ������ ����Ʈ ������ ���� �������� ������ �ٸ�");

            newMontage.mName = "Montage_" + std::to_string(_MONTAGE_INDEX_ACCUMULATION++);
            newMontage.PushSequence(Target->Sequences[SequenceIndex]);

            // ��Ÿ�� �߰�
            Target->Montages[newMontage.mName] = newMontage;

            // ��Ÿ�� ����Ʈ���� �ؽ�Ʈ �߰�
            AnimationSequence::getInstance().MontageListBox.AppendListBoxMessage(newMontage.mName);

            // ��Ÿ�� �ڵ�����
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
    // ��Ÿ�� ���Ź�ư
    static void DestroyMontageButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
        if (0 <= AnimationSequence::getInstance().MontageListBox.Selected)
        {
            UINT SequenceIndex = AnimationSequence::getInstance().MontageListBox.Selected;
            std::string SelectedName = AnimationSequence::getInstance().MontageListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // Ÿ�ٿ� ����Ǿ��ִ� ��Ÿ�� ������
            Target->Montages.erase(SelectedName);
            // UI List�� ��Ÿ�� �����ϰ�
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
    // ��Ÿ�ֿ��� ������ �ֱ�
    static void PushMontageButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
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
    // ��Ÿ�ֿ��� ������ ����
    static void PopMontageButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
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

    // ���� �����̽� ���� ��ư
    static void SaveBlendSpaceButtonFunc(ObjectData* obj)
    {
        std::string DirPath =
            "MetaData\\" + obj->objectName + "\\BlendSpace\\";
        std::string Path;

        // ���� ���� �Ǵ� ����
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
                // ������ �̸� ����
                BlendSpaceFile.write(clip.mSequence->mName.c_str(), sizeof(char*) * clip.mSequence->mName.size());
                // ������ ���� ������ ����
                UintToString = std::to_string(clip.mPositionX);
                BlendSpaceFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
                // ������ �� ������ ����
                UintToString = std::to_string(clip.mPositionY);
                BlendSpaceFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
            }

            BlendSpaceFile.close();
        }
    }

    // ���� �����̽� ������ư
    static void CreateBlendSpaceButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
        if (0 <= AnimationSequence::getInstance().SequenceListBox.Selected)
        {
            BlendSpace newBlendSpace;
            UINT SequenceIndex = AnimationSequence::getInstance().SequenceListBox.Selected;
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // UI ListBox���� ������ �μ��� ���� �������� ������ �ٸ� ���
            if (Target->Sequences.size() <= SequenceIndex)
                throw std::runtime_error("���� �̽�! ������ ����Ʈ ������ ���� �������� ������ �ٸ�");

            newBlendSpace.mName = "BlendSpace_" + std::to_string(_BLEND_SPACE_INDEX_ACCUMULATION++);
            newBlendSpace.PushSequence(
                Target->Sequences[SequenceIndex],
                AnimationSequence::getInstance().pBlendSpacePositionY,
                AnimationSequence::getInstance().pBlendSpacePositionX
            );

            // ���� �����̽� �߰�
            Target->BlendSpaces[newBlendSpace.mName] = newBlendSpace;

            // ���� �����̽� ����Ʈ���� �ؽ�Ʈ �߰�
            AnimationSequence::getInstance().BlendSpaceListBox.AppendListBoxMessage(newBlendSpace.mName);

            // ���� �����̽� �ڵ�����
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

    // ���� �����̽� ���Ź�ư
    static void DestroyBlendSpaceButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
        if (0 <= AnimationSequence::getInstance().BlendSpaceListBox.Selected)
        {
            UINT SequenceIndex = AnimationSequence::getInstance().BlendSpaceListBox.Selected;
            std::string SelectedName = AnimationSequence::getInstance().BlendSpaceListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // Ÿ�ٿ� ����Ǿ��ִ� ���� �����̽� ������
            Target->BlendSpaces.erase(SelectedName);
            // UI List�� ���� �����̽� �����ϰ�
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

    // ���� �����̽��� ������ �ֱ�
    static void PushBlendSpaceButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
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

    // ���� �����̽����� ������ ����
    static void PopBlendSpaceButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
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

    // ������Ʈ ���� ��ư
    static void SaveCompositeButtonFunc(ObjectData* obj)
    {
        std::string DirPath =
            "MetaData\\" + obj->objectName + "\\Composite\\";
        std::string Path;

        // ���� ���� �Ǵ� ����
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
                // ������ �̸� ����
                CompositeFile.write(clip.mSequence->mName.c_str(), sizeof(char*) * clip.mSequence->mName.size());
                // ������ ���� ������ ����
                UintToString = std::to_string(clip.mPositionX);
                CompositeFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
                // ������ �� ������ ����
                UintToString = std::to_string(clip.mPositionY);
                CompositeFile.write(UintToString.c_str(), sizeof(char*) * UintToString.size());
            }

            CompositeFile.close();
        }
    }

    // ���� �����̽� ������ư
    static void CreateCompositeButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
        if (0 <= AnimationSequence::getInstance().SequenceListBox.Selected)
        {
            Composite newComposite;
            UINT SequenceIndex = AnimationSequence::getInstance().SequenceListBox.Selected;
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // UI ListBox���� ������ �μ��� ���� �������� ������ �ٸ� ���
            if (Target->Sequences.size() <= SequenceIndex)
                throw std::runtime_error("���� �̽�! ������ ����Ʈ ������ ���� �������� ������ �ٸ�");

            newComposite.mName = "Composite_" + std::to_string(_COMPOSITE_INDEX_ACCUMULATION++);
            newComposite.PushSequence(
                Target->Sequences[SequenceIndex],
                AnimationSequence::getInstance().pCompositePositionY,
                AnimationSequence::getInstance().pCompositePositionX
            );

            // ������Ʈ �߰�
            Target->Composites[newComposite.mName] = newComposite;

            // ������Ʈ ����Ʈ���� �ؽ�Ʈ �߰�
            AnimationSequence::getInstance().CompositeListBox.AppendListBoxMessage(newComposite.mName);

            // ������Ʈ �ڵ�����
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

    // ���� �����̽� ���Ź�ư
    static void DestroyCompositeButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
        if (0 <= AnimationSequence::getInstance().CompositeListBox.Selected)
        {
            UINT SequenceIndex = AnimationSequence::getInstance().CompositeListBox.Selected;
            std::string SelectedName = AnimationSequence::getInstance().CompositeListBox.GetTextListBoxMessage();
            ObjectData* Target = AnimationSequence::getInstance().GetTarget();

            // Ÿ�ٿ� ����Ǿ��ִ� ������Ʈ ������
            Target->Composites.erase(SelectedName);
            // UI List�� ������Ʈ �����ϰ�
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

    // ���� �����̽��� ������ �ֱ�
    static void PushCompositeButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
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

    // ���� �����̽����� ������ ����
    static void PopCompositeButtonFunc()
    {
        // �������� ���� �Ǿ��ִ� ���� �� �� 
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
            // ���� iPage�� ��ư�� classID�� ���ٸ�
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
            // �ٸ��ٸ�
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
            // ��׶��� ���¸� �Է� ����
            bIsPaused = true;
            return result;
        }
        else
        {
            bIsPaused = false;

            // ȭ���� Ȱ��ȭ �Ǹ鼭 ��Ʈ�ѷ��� ������ ��´�.
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
            // ��׶��� ���¸� �Է� ����
            bIsPaused = true;
            return result;
        }
        else
        {
            bIsPaused = false;

            // ��Ʈ�ѷ��� ������ �����´�.
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

                // ����Ʈ �ڽ��� ������ ������ �����ͼ�
                int count = SendMessage(listbox->hwnd, LB_GETCOUNT, 0, 0);
                // Ŭ���� �������� ã�´�
                for (int i = 0; i < count; i++)
                {
                    // �ش� �������� ������ �Ȱ��� �ƴ��� Ȯ���Ѵ�
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

    // �� �������� ���� IMGUI UI ����
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

    // Ÿ�� �����͸� ���� �� �ֵ��� ���͸� ����
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

    // ���콺 Ŭ�� �� ��ġ�� ������Ʈ ���
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

        // Ŭ���ؼ� ������Ʈ�� ����� ���
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
                throw std::runtime_error("�޽��� �ݵ�� Sperated By Material�̿��� ��.");

            // �ִϸ��̼� ������Ʈ
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
    * ���õ� ���뿡 ���� ���׼�
    * 
    * ���� ���� �� �ϳ��� Ŭ�� �Ǿ��ٸ�
    * Pivot�� ��ġ�� �ű��, 
    * 
    * ���� Global Ʈ���Ű� false���
    * Pivot�� ������ ȸ���� ��ŭ ȸ���Ͽ� ��Ÿ����.
    * 
    */
    if (0 <= BonesListBox.Selected)
    {
        // Pivot ��ġ �ű��
        //ObjectData* Pivot = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_["Axis"];
        std::string BoneName = BonesListBox.GetTextListBoxMessage();

        if (!Bone->transform->InstanceBuffer.count(BoneName))
            throw std::runtime_error("���õ� ���� ã�� �� ����");

        Transform::transform* pivotInst = Pivot->transform->InstanceBuffer.begin()->second;
        float3 Position;

        Bone->transform->InstanceBuffer[BoneName]->GetLocalPosition(Position);
        pivotInst->SetPosition(Position);

        // ���� ȸ������ ���� �ǹ��� ȸ��
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

    // �ִϸ��̼��� ������Ʈ �Ѵ�.
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

#pragma region ��Ÿ ����

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

#pragma region ��Ÿ ����

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

#pragma region ��Ÿ ����

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
        // ���� Ÿ���� �ִԿ����� Composite �����̶��
        else if (target->CurrentAsset->mType == AnimationAsset::AnimationAssetType::COMPOSITE)
        {
            // �ִ� ������ Composite ������ �����ͼ�
            assetComposite = (Composite*)target->GetAsset();

            // ���� �����Ӱ� �ִϸ��̼� ���۸� �ֽ�ȭ �ϰ�
            assetComposite->mFrame = frame;
            assetComposite->mAnimBuffer.resize(target->SubmeshCount);

            for (submeshIDX = 0; submeshIDX < target->SubmeshCount; submeshIDX++)
            {
                VertSize = target->meshDatas[submeshIDX].Vertices.size();

                assetComposite->mAnimBuffer[submeshIDX].clear();
                lClusterDeformation.clear();
                assetComposite->mAnimBuffer[submeshIDX].resize(VertSize);
                lClusterDeformation.resize(VertSize);

                // Default Tranlation�� ���ؽ� ����Ʈ�� �����´�. (Ŭ������ ������)
                vertices = target->meshFBXDatas[submeshIDX]->GetControlPoints();
                lSkinDeformer =
                    (FbxSkin*)target->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

                lClusterCount =
                    lSkinDeformer->GetClusterCount();

                // Ŭ������(����)���� ���� �� ���� ���(ex> ����) ��ŭ �ݺ�
                for (lClusterIDX = 0; lClusterIDX < lClusterCount; lClusterIDX++)
                {
                    lCluster = lSkinDeformer->GetCluster(lClusterIDX);
                    lClusterName = std::string(lCluster->GetNameOnly());

                    // �� ���뿡 ���� Ʈ���� ã�´�.
                    // ������Ʈ�� �����ϱ� ���� �߽��� �Ǵ� ���� �� ������ ���⼭ �����Ѵ�.
                    targetSkeleton = target->BoneHierarchy.Find(lClusterName);

                    // ������ �� ������ �� ������ �� �� ���밡 ���� ������ �������� ���������� �����ϰ� �ֽ�ȭ �Ѵ�.
                    assetComposite->mIsBlending = targetSkeleton->IsParent(assetComposite->mPivotBoneName);

                    // Composite�� ����� ���� ����(������) ��Ʈ������ �о� �´�. 
                    assetComposite->GetAnimationFBXMatrix(submeshIDX, lClusterIDX, lVertexTransformMatrix);

#pragma region ��Ÿ ����

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

                    // ����(������) ������ ���� �ε����� ���´�.
                    mIndices = lCluster->GetControlPointIndices();
                    lVertexIndexCount = lCluster->GetControlPointIndicesCount();

                    if (!mIndices)
                        continue;

                    // ���� ����ġ ����
                    mWeights = lCluster->GetControlPointWeights();

                    // �� ���ؽ��� �����Ӹ� �� �� �̻� ������ ���� ��� ������ ���� ���� �Ѵ�.
                    for (k = 0; k < lVertexIndexCount; ++k)
                    {
                        for (i = 0; i < 4; i++)
                            for (j = 0; j < 4; j++)
                                lClusterDeformation[mIndices[k]][i][j] +=
                                lVertexTransformMatrix[i][j] * mWeights[k];
                    }

                    // Ÿ���� ���� ��ġ�� ǥ���� �ִ� ���� ��
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

        // Ÿ���� �� ���� ��ġ�� Bone�� �̵���Ų��
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

    // �ִϸ��̼� �÷��� ��ư�� ���� �ִٸ� �ڵ� �÷���
    if (target && AnimPlay)
    {
        frame = (frame + 1) < maxFrame ? (frame + 1) : 0;
    }
}

void AnimationSequence::MaterialUpdate(ObjectData* objIter, UINT submeshIDX)
{
#pragma region Update Material

        ID3D11ShaderResourceView* srv = nullptr;

        // �ؽ��� ������ ä�� �� ���� ���� �ϳ��� �����ȴ�.
        UINT AccumulateSlotIndex = 0;
        // ���׸��󿡼� �ؽ��� ���ͷ����Ϳ� ����
        UINT TextureIndex = 0;
        Material* mat = nullptr;

        // ����޽� �������� ������ ���׸����� �����Ѵ�.
        AccumulateSlotIndex = 0;
        TextureIndex = 0;
        mat = &objIter->meshDatas[submeshIDX].mMaterial;

        // ���̽� �ؽ��İ� ���ٸ� �ƹ� �ؽ��ĳ� ����
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
        // ���̽� �ؽ��İ� �ִٸ�
        else
        {
            // ���̽� �ؽ��� ������ŭ ���� ����
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

        // �븻 �ؽ��İ� ���ٸ�, ���� ����
        // �븻 �ؽ��İ� �ִٸ�
        if (mat->NormalTexutures.size())
        {
            // �븻 �ؽ��� ������ŭ ���� ����
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

        // ����ũ �ؽ��İ� ���ٸ�, ���� ����
        // ����ũ �ؽ��İ� �ִٸ�
        if (mat->MaskedTextures.size())
        {
            // ����ũ �ؽ��� ������ŭ ���� ����
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

        // ��Ż�� �ؽ��İ� ���ٸ�, ���� ����
        // ��Ż�� �ؽ��İ� �ִٸ�
        if (mat->MetallicTextures.size())
        {
            // ����ũ �ؽ��� ������ŭ ���� ����
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

        // �����Ͻ� �ؽ��İ� ���ٸ�, ���� ����
        // �����Ͻ� �ؽ��İ� �ִٸ�
        if (mat->RoughnessTextures.size())
        {
            // ����ũ �ؽ��� ������ŭ ���� ����
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

// Ŭ���� �ִ�, �ּ� �������� ����صӴϴ�
int MinStartFrame = 0;
int MaxEndFrame = 0;
// ���õ� Ŭ���� ����, �� �������� �����ϱ� ���� ����Ʈ 
int* StartFrame = nullptr;
int* EndFrame = nullptr;

void AnimationSequence::AnimationMontageUpdate()
{
    ImGui::Begin("Animation Montage");

    ImGui::Text("Animation Montage");
    ImGui::Checkbox("Play", &AnimPlay);
    ImGui::Checkbox("GlobalPivot", &GlobalPivot);

    // ����Ʈ�ڽ��� ���� ������
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

    // ���� StartFrame, EndFrame�� ������ �����Ǹ�
    if (StartFrame)
    {
        ImGui::InputInt("Start", StartFrame, 1, 1);
        ImGui::InputInt("End", EndFrame, 1, 1);

        // ������ ���������� ��ȭ�� �� �ֵ��� �˰����� ����� �����.
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

// Ŭ���� �ִ�, �ּ� �������� ����صӴϴ�
float PreviousX = 0;
float PreviousY = 0;
// ���õ� Ŭ���� ����, �� �������� �����ϱ� ���� ����Ʈ 
float* PositionX = nullptr;
float* PositionY = nullptr;

void AnimationSequence::AnimationBlendSpaceUpdate()
{
    ImGui::Begin("Animation BlendSpace");

    ImGui::Text("Animation BlendSpace");
    ImGui::Checkbox("Play", &AnimPlay);
    ImGui::Checkbox("GlobalPivot", &GlobalPivot);

    // ����Ʈ�ڽ��� ���� ������
    if (bIsListBoxUpdate)
    {
        // �������� �������ٸ�
        if (bIsListBoxUpdate->name == "SequenceListBox1")
        {
            // ������ ����
            target->SetAsset(
                (AnimationAsset*)(target->Sequences[bIsListBoxUpdate->Selected]));
        }
        // ���� �����̽��� �������ٸ�
        else if (bIsListBoxUpdate->name == "BlendSpaceListBox")
        {
            // ���� �����̽� ����
            target->SetAsset(
                (AnimationAsset*)(&target->BlendSpaces[bIsListBoxUpdate->GetTextListBoxMessage()]));

            // ������ ���� �����̽��� ������ ����Ʈ�� ��� �����ϰ�
            BlendSpaceSeqListBox.ResetListBoxMessage();
            // ���� ���õ� ���� �����̽��� ������ ����Ʈ�� �ٽ� �����ش�.
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

// ���� �����ϰ� �ִ� ������ Weight Point 
float* Weight = nullptr;

void AnimationSequence::AnimationMorphUpdate()
{
    ImGui::Begin("Animation Morph");
    ImGui::Text("Animation Morph");

    // ����Ʈ�ڽ��� ���� ������
    if (bIsListBoxUpdate)
    {
        // ���� ����Ʈ�� �������ٸ�
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