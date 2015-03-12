#include "SettingsManager.h"
#include "Encoding.h"

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
    WritePrivateProfileStringW(L"Settings", L"appdata", this->AppDataPath.c_str(), this->IniFile.c_str());
}

void CSettingsManager::Load()
{
    this->HotVKey = GetPrivateProfileIntW(L"Settings", L"hotkey", VK_F12, this->IniFile.c_str());

    wchar_t str[1024];

    auto len = GetPrivateProfileStringW(L"Settings", L"appdata", this->AppDataPath.c_str(), str, 1024, this->IniFile.c_str());
    this->AppDataPath = str;

    len = GetPrivateProfileStringW(L"Settings", L"format", this->Format.c_str(), str, 1024, this->IniFile.c_str());
    this->Format = str;
}

// -----------------------------------------------------------------

bool CSettingsForm::AskAndSetSettings()
{
    try
    {
        // make class for window
        InitDefaultWindowClass(L"_ClassAskAndSetSettings");

        InitDefaultWindow(L"ObsInfo Settings", 305, 200);

        // create inputs
        AddLabel(10, 20, 100, 21, L"Format:");
        AddLabel(10, 45, 100, 21, L"Hotkey:");
        AddLabel(10, 70, 100, 21, L"Output folder:");

        auto edFormat = AddEdit(115, 20, 100, 21, Settings()->Format.c_str());
        auto edHotkey = AddHotKeyEdit(115, 45, 100, 21, Settings()->HotVKey);
        auto edDataPath = AddEdit(115, 70, 150, 21, Settings()->AppDataPath.c_str());

        // create OK/Cancel button
        AddButton(75, 130, 100, 22, L"Ok", IDOK, BS_DEFPUSHBUTTON);
        AddButton(180, 130, 100, 22, L"Cancel", IDCANCEL, 0);

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
                        Settings()->AppDataPath = GetTextFromEditW(edDataPath);
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
    }
    catch (std::exception e)
    {
        MessageBoxA(HandleParent, e.what(), "Error!", MB_ICONEXCLAMATION | MB_OK);
    }

    return (ModalResult == IDOK);;
}
