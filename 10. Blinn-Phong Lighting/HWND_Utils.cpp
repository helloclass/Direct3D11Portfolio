#include "HWND_Utils.h"

UIPool UIPool::pool;

LRESULT CALLBACK subEditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::string s;
    float test = 0;

    switch (msg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            //Do your stuff
            for (UI_Edit* edit : UIPool::getInstance().UI_EDIT_LIST)
            {
                if (hwnd == edit->hwnd)
                    *edit->data = std::stof(edit->GetEditText());
            }
            break;  //or return 0; if you don't want to pass it further to def 
        }

        break;
    default:
        return CallWindowProc(
            oldEditProc, hwnd, msg, wParam, lParam);
    }
    return 0;
}


void UI_Button::CreateButton(
    _In_ HWND mainHWND,
    _In_ UINT index,
    _In_ std::string ID,
    _In_ LPCWSTR text,
    _In_ int x,
    _In_ int y,
    _In_ int w,
    _In_ int h,
    _In_ void(*func)()
) {
    this->name = ID;
    Main_WINDOW_HWND = &mainHWND;

    type = UIType::UI_BUTTON;
    HWND_BUTTON_ID =
        _HWND_BUTTON_OFFSET + UIPool::getInstance().UI_BUTTON_LIST.size();
    OnClicked = func;

    hwnd = CreateWindow(
        L"BUTTON",                  // Predefined class; Unicode assumed 
        text,                      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        x,                          // x position 
        y,                          // y position 
        w,                          // Button width
        h,                          // Button height
        mainHWND,                   // Parent window
        (HMENU)(HWND_BUTTON_ID), // menu.
        (HINSTANCE)GetWindowLongPtr(mainHWND, GWLP_HINSTANCE),
        NULL);                      // Pointer not needed.

    if (!hwnd) {
        MessageBoxA(0, "CreateButton failed", "Fatal Error", MB_OK);
        throw std::runtime_error("Create Button Exception!!");
    }

    if (0 < index)
        ShowWindow(hwnd, SW_HIDE);

    UIPool::getInstance().UI_BUTTON_LIST.push_back(this);
}

void UI_Edit::CreateEdit(
    _In_ HWND mainHWND,
    _In_ UINT index,
    _In_ std::string ID,
    _In_ int x,
    _In_ int y,
    _In_ int w,
    _In_ int h,
    _In_ float* data_ptr
) {
    this->name = ID;
    Main_WINDOW_HWND = &mainHWND;

    type = UIType::UI_EDIT;
    HWND_EDIT_ID =
        _HWND_EDIT_OFFSET + UIPool::getInstance().UI_EDIT_LIST.size();
    data = data_ptr;

    HINSTANCE hIns = GetModuleHandle(0);

    hwnd = CreateWindow(
        L"edit",                 // Predefined class; Unicode assumed 
        NULL,                       // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        x,                          // x position 
        y,                          // y position 
        w,                          // Button width
        h,                          // Button height
        mainHWND,                   // Parent window
        (HMENU)(HWND_EDIT_ID),      // menu.
        (HINSTANCE)GetWindowLongPtr(mainHWND, GWLP_HINSTANCE),
        NULL);                      // Pointer not needed.                   // Pointer not needed.

    if (!hwnd) {
        MessageBoxA(0, "CreateEdit failed", "Fatal Error", MB_OK);
        throw std::runtime_error("Create Edit Exception!!");
    }

    if (0 < index)
        ShowWindow(hwnd, SW_HIDE);

    oldEditProc =
        (WNDPROC)SetWindowLongPtr(
            hwnd, GWLP_WNDPROC, (LONG_PTR)subEditProc
        );

    UIPool::getInstance().UI_EDIT_LIST.push_back(this);
}

void UI_ListBox::CreateListBox(
    _In_ HWND mainHWND,
    _In_ UINT index,
    _In_ std::string ID,
    _In_ int x,
    _In_ int y,
    _In_ int w,
    _In_ int h) 
{
    this->name = ID;
    Main_WINDOW_HWND = &mainHWND;

    type = UIType::UI_LISTBOX;
    HWND_LIST_BOX_ID =
        _HWND_LIST_BOX_OFFSET + UIPool::getInstance().UI_LIST_BOX_LIST.size();

    HINSTANCE hIns = GetModuleHandle(0);
    LPCWSTR text = L"";

    hwnd = CreateWindow(
        L"listbox",                 // Predefined class; Unicode assumed 
        text,                       // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY,  // Styles 
        x,                          // x position 
        y,                          // y position 
        w,                          // Button width
        h,                          // Button height
        mainHWND,                   // Parent window
        (HMENU)(HWND_LIST_BOX_ID),  // menu.
        (HINSTANCE)GetWindowLongPtr(mainHWND, GWLP_HINSTANCE),
        NULL);                      // Pointer not needed.    

    if (!hwnd) {
        MessageBoxA(0, "CreateListBox failed", "Fatal Error", MB_OK);
        throw std::runtime_error("Create ListBox Exception!!");
    }

    if (0 < index)
        ShowWindow(hwnd, SW_HIDE);

    UIPool::getInstance().UI_LIST_BOX_LIST.push_back(this);
}

void UI_Trackbar::CreateTrackbar(
    _In_ HWND mainHWND,
    _In_ UINT index,
    _In_ std::string ID,
    _In_ int x,
    _In_ int y,
    _In_ int w,
    _In_ int h,
    _In_ UINT iMin,     // minimum value in trackbar range 
    _In_ UINT iMax,   // maximum value in trackbar range 
    _In_ UINT iSelMin,  // minimum value in trackbar selection 
    _In_ UINT iSelMax, // maximum value in trackbar selection 
    _In_ UINT& ParamPTR
) {
    this->name = ID;
    Main_WINDOW_HWND = &mainHWND;

    type = UIType::UI_TRACKBAR;
    HWND_TRACKBAR_ID =
        _HWND_TRACKBAR_OFFSET + UIPool::getInstance().UI_TRACKBAR_LIST.size();

    hwnd = CreateWindowEx(
        0,                               // no extended styles 
        TRACKBAR_CLASS,                  // class name 
        L"Trackbar Control",              // title (caption) 
        WS_CHILD |
        WS_VISIBLE |
        TBS_AUTOTICKS |
        TBS_ENABLESELRANGE,              // style 
        x, y,                            // position 
        w, h,                            // size 
        mainHWND,                        // parent window 
        (HMENU)(HWND_TRACKBAR_ID),       // control identifier 
        (HINSTANCE)GetWindowLongPtr(mainHWND, GWLP_HINSTANCE),
        NULL                             // no WM_CREATE parameter 
    );

    if (!hwnd) {
        MessageBoxA(0, "Create TrackBar Control failed", "Fatal Error", MB_OK);
        throw std::runtime_error("Create TrackBar Control Exception!!");
    }

    SendMessage(
        hwnd,
        TBM_SETRANGE,
        (WPARAM)TRUE,                   // redraw flag 
        (LPARAM)MAKELONG(iMin, iMax));  // min. & max. positions

    SendMessage(
        hwnd,
        TBM_SETPAGESIZE,
        0,
        (LPARAM)4                        // new page size 
    );

    SendMessage(
        hwnd,
        TBM_SETSEL,
        (WPARAM)FALSE,                  // redraw flag 
        (LPARAM)MAKELONG(iSelMin, iSelMax)
    );

    SendMessage(
        hwnd,
        TBM_SETPOS,
        (WPARAM)TRUE,                   // redraw flag 
        (LPARAM)iSelMin
    );

    SendMessage(
        hwnd,
        TBM_GETPOS,
        (WPARAM)TRUE,                   // redraw flag 
        (LPARAM)iSelMin);

    if (0 < index)
        ShowWindow(hwnd, SW_HIDE);

    UIPool::getInstance().UI_TRACKBAR_LIST.push_back(this);
}


void UI_Edit::SetEditText(std::string Message)
{
    std::wstring wMessage;

    wMessage.assign(Message.begin(), Message.end());
    SendMessage(hwnd, WM_SETTEXT, (WPARAM)0, (LPARAM)wMessage.c_str());

}

std::string UI_Edit::GetEditText()
{
    std::wstring message;
    message.resize(1000);
    SendMessage(hwnd, WM_GETTEXT, (WPARAM)1000, (LPARAM)message.c_str());

    std::string res;
    res.assign(message.begin(), message.end());

    return res;
}

void UI_ListBox::AppendListBoxMessage(std::string Message)
{
    char result[1024];
    strcpy_s(result, Message.c_str());

    SendMessageA(hwnd, LB_ADDSTRING, 0, (LPARAM)result);
}

void UI_ListBox::SetTextListBoxMessage(std::string Item)
{
    std::wstring wstr;
    wstr.assign(Item.begin(), Item.end());

    UINT ItemIndex = SendMessage(
        hwnd,
        LB_FINDSTRING,
        -1,
        (LPARAM)(wstr.c_str())
    );

    Selected = ItemIndex;
    SendMessageA(hwnd, LB_SETCURSEL, (WPARAM)ItemIndex, 0);
}

std::string UI_ListBox::GetTextListBoxMessage()
{
    std::string  res = "";
    std::wstring check;

    if (Selected == -1)
        return res;

    check.resize(1024);
    SendMessage(hwnd, LB_GETTEXT, Selected, (LPARAM)check.c_str());

    std::wstring::iterator checkEnd = check.begin();

    int iter = 0;
    while (check[iter++])
    {
        checkEnd++;
    }

    res.assign(check.begin(), checkEnd);

    return res;
}

void UI_ListBox::DeleteListBoxMessage(UINT ItemIndex)
{
    SendMessageA(hwnd, LB_DELETESTRING, (WPARAM)ItemIndex, 0);
}

void UI_ListBox::ResetListBoxMessage()
{
    SendMessageA(hwnd, LB_RESETCONTENT, 0, 0);
}
