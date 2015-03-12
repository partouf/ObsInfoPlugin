
#include "FormBase.h"
#include "Commctrl.h"


DWORD CModalWindowBase::GetHotkeyFromHotkeyEdit(HWND hwnd)
{
    return static_cast<DWORD>(SendMessage(hwnd, HKM_GETHOTKEY, 0, 0));
}

std::string CModalWindowBase::GetTextFromEditA(HWND hwnd)
{
    char s[1025];

    // The return value is the number of characters copied, not including the terminating null character.
    DWORD length = static_cast<DWORD>(SendMessageA(hwnd, WM_GETTEXT, 1024, (LPARAM)s));
    s[length] = 0;

    return std::string(s);
}

std::wstring CModalWindowBase::GetTextFromEditW(HWND hwnd)
{
    wchar_t s[1025];

    // The return value is the number of characters copied, not including the terminating null character.
    DWORD length = static_cast<DWORD>(SendMessageW(hwnd, WM_GETTEXT, 1024, (LPARAM)s));
    s[length] = 0;

    return std::wstring(s);
}

HWND CModalWindowBase::AddLabel(int x, int y, int w, int h, const wchar_t *caption)
{
    HWND lbl =
        CreateWindowExW(
        0,
        L"STATIC",
        caption,
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        x, y, w, h,
        HandleWindow,
        0,
        AppInstance,
        NULL
        );

    this->Handles.push_back(lbl);

    SendMessage(lbl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    return lbl;
}

HWND CModalWindowBase::AddEdit(int x, int y, int w, int h, const wchar_t *caption)
{
    HWND edt =
        CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        caption,
        WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_TABSTOP,
        x, y, w, h,
        HandleWindow,
        0,
        AppInstance,
        NULL
        );

    this->Handles.push_back(edt);

    SendMessage(edt, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    return edt;
}

HWND CModalWindowBase::AddEditA(int x, int y, int w, int h, const char *caption)
{
    HWND edt =
        CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        caption,
        WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_TABSTOP,
        x, y, w, h,
        HandleWindow,
        0,
        AppInstance,
        NULL
        );

    this->Handles.push_back(edt);

    SendMessage(edt, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    return edt;
}

HWND CModalWindowBase::AddHotKeyEdit(int x, int y, int w, int h, DWORD vkey)
{
    HWND edt =
        CreateWindowExA(
        WS_EX_CLIENTEDGE,
        HOTKEY_CLASSA,
        "",
        WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_TABSTOP,
        x, y, w, h,
        HandleWindow,
        0,
        AppInstance,
        NULL
        );

    this->Handles.push_back(edt);

    SendMessage(edt, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    SendMessage(edt,
        HKM_SETHOTKEY,
        vkey,
        0);

    return edt;
}


HWND CModalWindowBase::AddButton(int x, int y, int w, int h, const wchar_t *caption, short data, DWORD extrastyle)
{
    HWND btn =
        CreateWindowExW(
        0,
        L"BUTTON",
        caption,
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_TABSTOP | extrastyle,
        x, y, w, h,
        HandleWindow,
        (HMENU)data,
        AppInstance,
        NULL
        );

    this->Handles.push_back(btn);

    SendMessage(btn, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    return btn;
}

LRESULT CALLBACK CModalWindowBase::MsgHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_COMMAND:
        short cmd = LOWORD(wparam);
        if ((cmd == IDOK) || (cmd == IDCANCEL)) {
            CModalWindowBase *window = (CModalWindowBase *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            window->ModalResult = cmd;

            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        else
        {
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

CModalWindowBase::CModalWindowBase(HWND hParent, HINSTANCE hAppInstance)
{
    HandleParent = hParent;
    HandleWindow = NULL;
    AppInstance = hAppInstance;
    wndclass.lpszClassName = nullptr;
}

CModalWindowBase::~CModalWindowBase()
{
    for (auto h : Handles)
    {
        DestroyWindow(h);
    }
    Handles.clear();

    DestroyWindow(HandleWindow);
    HandleWindow = NULL;

    if (wndclass.lpszClassName != nullptr) {
        UnregisterClassW(wndclass.lpszClassName, AppInstance);
    }
}

void CModalWindowBase::InitDefaultWindowClass(const wchar_t *sClass)
{
    wndclass.hInstance = AppInstance;
    wndclass.lpszClassName = sClass;
    wndclass.lpfnWndProc = CModalWindowBase::MsgHandler;
    wndclass.style = CS_DBLCLKS;
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.lpszMenuName = NULL;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

    if (!RegisterClassExW(&wndclass)) {
        throw std::exception("Can't create window class?");
    }
}

void CModalWindowBase::InitDefaultWindow(const wchar_t *sTitle, int width, int height)
{
    RECT parentRect;

    GetWindowRect(HandleParent, &parentRect);

    // create window
    HandleWindow =
        CreateWindowExW(
        WS_EX_STATICEDGE | WS_EX_CONTROLPARENT,
        wndclass.lpszClassName,
        sTitle,
        WS_OVERLAPPED | WS_GROUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        (parentRect.left + parentRect.right) / 2 - (width / 2),
        (parentRect.top + parentRect.bottom) / 2 - (height / 2) - 10,
        width, height,
        HandleParent,
        NULL,
        wndclass.hInstance,
        NULL
        );

    if (HandleWindow == NULL)
    {
        throw std::exception("Window Creation Failed!");
    }

    SetWindowLongPtr(HandleWindow, GWLP_USERDATA, (LONG_PTR)this);
}