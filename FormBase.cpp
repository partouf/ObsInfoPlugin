
#include "FormBase.h"
#include "Commctrl.h"

DWORD CModalWindowBase::GetHotkeyFromHotkeyEdit(HWND hwnd)
{
    return (DWORD)SendMessage(hwnd, HKM_GETHOTKEY, 0, 0);
}

std::string CModalWindowBase::GetTextFromEditA(HWND hwnd)
{
    char s[1025];

    // The return value is the number of characters copied, not including the terminating null character.
    DWORD length = (DWORD)SendMessageA(hwnd, WM_GETTEXT, 1024, (LPARAM)s);
    s[length] = 0;

    return std::string(s);
}

std::wstring CModalWindowBase::GetTextFromEditW(HWND hwnd)
{
    wchar_t s[1025];

    // The return value is the number of characters copied, not including the terminating null character.
    DWORD length = (DWORD)SendMessageW(hwnd, WM_GETTEXT, 1024, (LPARAM)s);
    s[length] = 0;

    return std::wstring(s);
}

HWND CModalWindowBase::AddLabel(int x, int y, int w, int h, wchar_t *caption)
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
    SendMessage(lbl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    return lbl;
}

HWND CModalWindowBase::AddEdit(int x, int y, int w, int h, wchar_t *caption)
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
    SendMessage(edt, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    return edt;
}

HWND CModalWindowBase::AddEditA(int x, int y, int w, int h, char *caption)
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
    SendMessage(edt, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    SendMessage(edt,
        HKM_SETHOTKEY,
        vkey,
        0);

    return edt;
}


HWND CModalWindowBase::AddButton(int x, int y, int w, int h, wchar_t *caption, short data, DWORD extrastyle)
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
    AppInstance = hAppInstance;
}

CModalWindowBase::~CModalWindowBase()
{
}
