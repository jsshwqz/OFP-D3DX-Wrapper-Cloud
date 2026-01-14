#ifndef CONFIG_HPP
#define CONFIG_HPP

#pragma once

#include <Windows.h>

struct WrapperConfig
{
    BOOL EnableWrapper;
    BOOL debugOutput;
    DWORD WrapperMode;
    
    BOOL D3d8to9;
    BOOL RemoveNightFilter;
    BOOL FixNightScene;
    
    DWORD AnisotropicFiltering;
    DWORD AntiAliasing;
    BOOL EnableVSync;
    BOOL ForceVSyncMode;
    BOOL EnableWindowMode;
    
    BOOL AutoFallback;
    BOOL MinPerformanceMode;

    DWORD FPSUpdateInterval;
    DWORD FPSPosX;
    DWORD FPSPosY;
    
    DWORD DefaultFOV;
    BOOL AllowDynamicFOV;
    DWORD FOVToggleKey;
    FLOAT AimingFOVMultiplier;
    
    BOOL ShowFPS;
    BOOL EnableFOV;
};

namespace Config
{
    extern WrapperConfig g_Config;
    
    void Load();
    void Log(const char* format, ...);
}

#endif // CONFIG_HPP
