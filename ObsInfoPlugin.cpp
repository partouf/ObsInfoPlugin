// ObsInfoPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "ObsInfoPlugin.h"
#include "SettingsManager.h"
#include "OBSApi.h"
#include <string>

void DumpStringToFile(std::string things) {
    std::wstring fullpath = Settings()->AppDataPath + L"\\" + Settings()->FileName;

    // check if file exists and append
    FILE *fp = _wfopen(fullpath.c_str(), L"ab");
    if (fp == NULL) {
        // if it doesn't exist yet, create it through write
        fp = _wfopen(fullpath.c_str(), L"wb");
    }

    if (fp != NULL)
    {
        fprintf(fp, "%s\r\n", things.c_str());

        fclose(fp);
    }
    else
    {
        // I don't even...
    }
}

// convert milliseconds to hours:minutes:seconds, 0 leading on minutes and seconds, hours can be a lot
std::string MilliToHMSString(UINT time)
{
    char s[1024];

    uint32_t totalseconds = (uint32_t)trunc(time / 1000.0);

    uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
    uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
    uint32_t sec = totalseconds % 60;

    sprintf(s, Settings()->Format.c_str(), hr, min, sec);

    return s;
}

void __stdcall HotKeyProc(DWORD a, UPARAM b, bool bDown) {
    if (bDown)
    {
        // F12 down key hit

        // get total stream time till now and dump that into a file

        UINT time = OBSGetTotalStreamTime();

        DumpStringToFile(MilliToHMSString(time));
    }
}

OBSINFOPLUGIN_API bool LoadPlugin() {
    // intialization stuff when plugin "really" gets loaded - register hotkey in OBS and get the appdata path

    Settings()->RegisteredHotKey = OBSCreateHotkey(VK_F12, &HotKeyProc, 0);
    Settings()->AppDataPath = OBSGetAppDataPath();

/*
    DumpStringToFile(MilliToHMSString(60000));
    DumpStringToFile(MilliToHMSString(61000));
    DumpStringToFile(MilliToHMSString(3600000));
    DumpStringToFile(MilliToHMSString(119000));
    DumpStringToFile(MilliToHMSString(3600000));
    DumpStringToFile(MilliToHMSString(3660000));
    DumpStringToFile(MilliToHMSString(7200000));
    DumpStringToFile(MilliToHMSString(14400000));
    DumpStringToFile(MilliToHMSString(14461000));
*/

    return true;
}

OBSINFOPLUGIN_API void UnloadPlugin() {
    UINT k = Settings()->RegisteredHotKey;
    Settings()->RegisteredHotKey = 0;

    OBSDeleteHotkey(k);
}

OBSINFOPLUGIN_API void OnStartStream() {
    // might be useful to mark start and stops of older streams
    DumpStringToFile("StartStream");
}

OBSINFOPLUGIN_API void OnStopStream() {
    DumpStringToFile("StopStream");
}

OBSINFOPLUGIN_API CTSTR GetPluginName() {
    return L"ObsInfoPlugin";
}

OBSINFOPLUGIN_API CTSTR GetPluginDescription() {
    return L"ObsInfoPlugin does stuff when you press F12";
}
