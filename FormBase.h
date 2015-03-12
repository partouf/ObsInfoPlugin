#pragma once

#include <Windows.h>
#include <string>
#include <vector>

class CModalWindowBase
{
protected:
    std::vector<HWND> Handles;
    WNDCLASSEXW wndclass;
    HINSTANCE AppInstance = NULL;
    HWND HandleParent = NULL;
    WORD ModalResult = 0;
    HWND HandleWindow = NULL;
public:
    CModalWindowBase(HWND hParent, HINSTANCE hAppInstance);
    virtual ~CModalWindowBase();

    void InitDefaultWindowClass(const wchar_t *sClass);
    void InitDefaultWindow(const wchar_t *sTitle, int width, int height);

    std::wstring GetTextFromEditW(HWND hwnd);
    std::string GetTextFromEditA(HWND hwnd);

    HWND AddLabel(int x, int y, int w, int h, const wchar_t *caption);
    HWND AddEdit(int x, int y, int w, int h, const wchar_t *caption);
    HWND AddEditA(int x, int y, int w, int h, const char *caption);
    HWND AddButton(int x, int y, int w, int h, const wchar_t *caption, short data, DWORD extrastyle);

    HWND AddHotKeyEdit(int x, int y, int w, int h, DWORD vkey);
    DWORD GetHotkeyFromHotkeyEdit(HWND hwnd);

    static LRESULT CALLBACK MsgHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};
