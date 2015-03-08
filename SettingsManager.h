#pragma once

#include <Windows.h>
#include <string>

#include "FormBase.h"

class CSettingsManager
{
public:
    HINSTANCE Parent;
    UINT RegisteredHotKey;
    UINT HotVKey;

    std::wstring AppDataPath;
    std::wstring IniFile;
    std::wstring FileName;
    std::wstring Format;

    CSettingsManager();
    virtual ~CSettingsManager();

    void Save();
    void Load();
};

class CSettingsForm : public CModalWindowBase
{
public:
    CSettingsForm(HWND hParent, HINSTANCE hAppInstance) : CModalWindowBase(hParent, hAppInstance) {};
    
    bool AskAndSetSettings();
};


// get main settings object
CSettingsManager *Settings();

// set a custom settings object (injection for testing purposes really)
void setCustomSettings(CSettingsManager *aSettings);
