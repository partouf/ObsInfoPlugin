// ObsInfoPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "ObsInfoPlugin.h"
#include "SettingsManager.h"
#include <string>
#include "OBSApi.h"
#include "Encoding.h"

void DumpStringToFile(std::wstring things) {
    std::wstring fullpath = Settings()->AppDataPath + L"\\" + Settings()->FileName;

    // check if file exists and append
    FILE *fp = _wfopen(fullpath.c_str(), L"ab");
    if (fp == NULL) {
        // if it doesn't exist yet, create it through write
        fp = _wfopen(fullpath.c_str(), L"wb");
    }

    if (fp != NULL)
    {
        // convert things to default windows codepage
        std::string def = CEncoding::Wide2Default(things);
        fprintf(fp, "%s\r\n", def.c_str());

        fclose(fp);
    }
    else
    {
        // I don't even...
    }
}

// convert milliseconds to hours:minutes:seconds, 0 leading on minutes and seconds, hours can be a lot
std::wstring MilliToHMSString(UINT time)
{
    wchar_t s[1024];

    uint32_t totalseconds = (uint32_t)trunc(time / 1000.0);

    uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
    uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
    uint32_t sec = totalseconds % 60;

    wsprintf(s, Settings()->Format.c_str(), hr, min, sec);

    return s;
}

void __stdcall HotKeyProc(DWORD a, UPARAM b, bool bDown) {
    if (bDown)
    {
        // get total stream time till now and dump that into a file

        UINT time = OBSGetTotalStreamTime();

        DumpStringToFile(MilliToHMSString(time));
    }
}

bool LoadPlugin() {
    // intialization stuff when plugin "really" gets loaded - register hotkey in OBS and get the appdata path

    Settings()->AppDataPath = OBSGetAppDataPath();
    Settings()->IniFile = OBSGetPluginDataPath() + L"\\" + L"ObsInfo.ini";

    // load from ini file if available
    Settings()->Load();

    Settings()->RegisteredHotKey = OBSCreateHotkey(Settings()->HotVKey, &HotKeyProc, 0);

    return true;
}

void UnloadPlugin() {
    UINT k = Settings()->RegisteredHotKey;
    Settings()->RegisteredHotKey = 0;

    OBSDeleteHotkey(k);
}

void OnStartStream() {
    // might be useful to mark start and stops of older streams
    DumpStringToFile(L"StartStream");
}

void OnStopStream() {
    DumpStringToFile(L"StopStream");
}

CTSTR GetPluginName() {
    return L"ObsInfoPlugin";
}

CTSTR GetPluginDescription() {
    return L"ObsInfoPlugin does stuff when you press F12";
}

void ConfigPlugin(HWND hHandle) {
    CSettingsForm frmSettings(hHandle, Settings()->Parent);

    if (frmSettings.AskAndSetSettings())
    {
        Settings()->Save();

        UnloadPlugin();
        LoadPlugin();
    }
}
