#pragma once

#include <Windows.h>
#include <string>

class CSettingsManager
{
public:
    HINSTANCE Parent;
    UINT RegisteredHotKey;
    std::wstring AppDataPath;
    std::wstring FileName;
    std::string Format;

    CSettingsManager();
    virtual ~CSettingsManager();
};

// get main settings object
CSettingsManager *Settings();

// set a custom settings object (injection for testing purposes really)
void setCustomSettings(CSettingsManager *aSettings);
