#pragma once

#include <Windows.h>
#include <string>

class CModalWindowBase
{
protected:
    HINSTANCE AppInstance = NULL;
    HWND HandleParent = NULL;
    WORD ModalResult = 0;
    HWND HandleWindow = NULL;
public:
    CModalWindowBase(HWND hParent, HINSTANCE hAppInstance);
    virtual ~CModalWindowBase();

    std::wstring CModalWindowBase::GetTextFromEditW(HWND hwnd);
    std::string CModalWindowBase::GetTextFromEditA(HWND hwnd);

    HWND AddLabel(int x, int y, int w, int h, wchar_t *caption);
    HWND AddEdit(int x, int y, int w, int h, wchar_t *caption);
    HWND AddEditA(int x, int y, int w, int h, char *caption);
    HWND AddButton(int x, int y, int w, int h, wchar_t *caption, short data, DWORD extrastyle);

    HWND AddHotKeyEdit(int x, int y, int w, int h, DWORD vkey);
    DWORD GetHotkeyFromHotkeyEdit(HWND hwnd);

    static LRESULT CALLBACK MsgHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};
