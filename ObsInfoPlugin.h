#pragma once

#include "OBSApi.h"

#ifdef OBSINFOPLUGIN_EXPORTS
#define OBSINFOPLUGIN_API extern "C" __declspec(dllexport)
#else
#define OBSINFOPLUGIN_API extern "C" __declspec(dllimport)
#endif

OBSINFOPLUGIN_API bool LoadPlugin();
OBSINFOPLUGIN_API void UnloadPlugin();
OBSINFOPLUGIN_API void OnStartStream();
OBSINFOPLUGIN_API void OnStopStream();
OBSINFOPLUGIN_API CTSTR GetPluginName();
OBSINFOPLUGIN_API CTSTR GetPluginDescription();
OBSINFOPLUGIN_API void ConfigPlugin(HWND hHandle);
