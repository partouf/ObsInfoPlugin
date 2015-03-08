#include "SettingsManager.h"

// main singleton
CSettingsManager _settingsmanager;
CSettingsManager *customSettingsManager = NULL;

void setCustomSettings(CSettingsManager *aSettings)
{
    customSettingsManager = aSettings;
}

CSettingsManager *Settings() {
    if (customSettingsManager != NULL)
    {
        return customSettingsManager;
    }
    else
    {
        return &_settingsmanager;
    }
}

// ----------------------------------------------------------------

CSettingsManager::CSettingsManager()
{
    this->Parent = NULL;
    this->FileName = L"ObsInfo.txt";
    this->Format = L"%d:%02d:%02d";
    this->HotVKey = VK_F12;
}

CSettingsManager::~CSettingsManager()
{
}

void CSettingsManager::Save()
{
    wchar_t str[1024];
    wsprintf(str, L"%d\0", this->HotVKey);
    WritePrivateProfileStringW(L"Settings", L"hotkey", str, this->IniFile.c_str());
    WritePrivateProfileStringW(L"Settings", L"format", this->Format.c_str(), this->IniFile.c_str());
}

void CSettingsManager::Load()
{
    this->HotVKey = GetPrivateProfileIntW(L"Settings", L"hotkey", this->HotVKey, this->IniFile.c_str());

    wchar_t str[1024];
    DWORD len = GetPrivateProfileStringW(L"Settings", L"hotkey", L"%d:%02d:%02d", &str[0], 1024, this->IniFile.c_str());
    // getprivateprofilestringw should already have \0\0'ed the output string
    this->Format = str;
}

// -----------------------------------------------------------------

bool CSettingsForm::AskAndSetSettings()
{
    // make class for window
    WCHAR sClass[] = L"_ClassAskAndSetSettings";
    WCHAR sTitle[1024] = L"";

    wsprintf(sTitle, L"ObsInfo Settings\0\0\0\0", "");

    WNDCLASSEXW wndclass;
    wndclass.hInstance = AppInstance;
    wndclass.lpszClassName = sClass;
    wndclass.lpfnWndProc = CSettingsForm::MsgHandler;
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
        MessageBoxA(HandleParent, "Can't create window class?", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }


    RECT parentRect;

    GetWindowRect(HandleParent, &parentRect);


    // create window
    HandleWindow =
        CreateWindowExW(
        WS_EX_STATICEDGE | WS_EX_CONTROLPARENT,
        wndclass.lpszClassName,
        sTitle,
        WS_OVERLAPPED | WS_GROUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        (parentRect.left + parentRect.right) / 2 - 150,
        (parentRect.top + parentRect.bottom) / 2 - 90,
        305, 200,
        HandleParent,
        NULL,
        wndclass.hInstance,
        NULL
        );

    if (HandleWindow == NULL)
    {
        UnregisterClassW(wndclass.lpszClassName, AppInstance);

        MessageBoxA(HandleParent, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    SetWindowLongPtr(HandleWindow, GWLP_USERDATA, (LONG)this);

    wchar_t caption[1024] = L"";

    // create inputs
    HWND lblFormat = AddLabel(10, 20, 100, 21, L"Format:");
    HWND lblHotkey = AddLabel(10, 45, 100, 21, L"Hotkey:");

    char captionA[1024] = "";
    sprintf(captionA, "%s", Settings()->Format.c_str());
    HWND edFormat = AddEditA(115, 20, 100, 21, captionA);

    HWND edHotkey = AddHotKeyEdit(115, 45, 100, 21, Settings()->HotVKey);

    // create OK/Cancel button
    HWND btnOk = AddButton(75, 130, 100, 22, L"Ok", IDOK, BS_DEFPUSHBUTTON);
    HWND btnCancel = AddButton(180, 130, 100, 22, L"Cancel", IDCANCEL, 0);

    // show window
    ShowWindow(HandleWindow, SW_NORMAL);

    bool bClosed = false;

    // (show modal)

    EnableWindow(HandleParent, false);

    MSG message;
    while (!bClosed)
    {
        WaitMessage();

        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_SYSCOMMAND)
            {
                if (message.wParam == SC_CLOSE) /// "close all windows" thing in Win7
                {
                    bClosed = true;
                    break;
                }
            }
            else if ((message.message == WM_QUIT) || (message.message == WM_CLOSE))
            {
                bClosed = true;

                if (ModalResult == IDOK)
                {
                    Settings()->Format = GetTextFromEditW(edFormat);

                    Settings()->HotVKey = GetHotkeyFromHotkeyEdit(edHotkey);
                }

                break;
            }

            if (!IsDialogMessage(HandleWindow, &message)) {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
    }

    EnableWindow(HandleParent, true);

    // todo: throw controls into classes so we take advantage of RAII :o , but not really worth the effort
    DestroyWindow(lblFormat);
    DestroyWindow(lblHotkey);
    DestroyWindow(edFormat);
    DestroyWindow(edHotkey);
    DestroyWindow(btnOk);
    DestroyWindow(btnCancel);
    DestroyWindow(HandleWindow);

    HandleWindow = NULL;

    UnregisterClassW(wndclass.lpszClassName, AppInstance);

    // return true if Ok, false for Cancel
    return (ModalResult == IDOK);
}
