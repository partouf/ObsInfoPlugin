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

// ---

CSettingsManager::CSettingsManager()
{
    this->Parent = NULL;
    this->FileName = L"ObsInfo.txt";
    this->Format = "%d:%02d:%02d\0";
}

CSettingsManager::~CSettingsManager()
{
}
