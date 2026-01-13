#include "config.hpp"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

namespace Config
{
    WrapperConfig g_Config;

    void Load()
    {
        char configPath[MAX_PATH];
        char gamePath[MAX_PATH];
        
        GetModuleFileNameA(NULL, gamePath, MAX_PATH);
        char* p = strrchr(gamePath, '\\');
        if (p) p[1] = '\0';
        
        snprintf(configPath, MAX_PATH, "%sd3dx.ini", gamePath);
        
        g_Config.EnableWrapper = GetPrivateProfileIntA("General", "EnableWrapper", 1, configPath) ? TRUE : FALSE;
        g_Config.debugOutput = GetPrivateProfileIntA("General", "debugOutput", 0, configPath) ? TRUE : FALSE;
        g_Config.WrapperMode = GetPrivateProfileIntA("Renderer", "WrapperMode", 0, configPath);
        
        g_Config.D3d8to9 = GetPrivateProfileIntA("Compatibility", "D3d8to9", 1, configPath) ? TRUE : FALSE;
        g_Config.RemoveNightFilter = GetPrivateProfileIntA("Compatibility", "RemoveNightFilter", 1, configPath) ? TRUE : FALSE;
        g_Config.FixNightScene = GetPrivateProfileIntA("Compatibility", "FixNightScene", 1, configPath) ? TRUE : FALSE;
        
        g_Config.AnisotropicFiltering = GetPrivateProfileIntA("d3d9", "AnisotropicFiltering", 16, configPath);
        g_Config.AntiAliasing = GetPrivateProfileIntA("d3d9", "AntiAliasing", 0, configPath);
        g_Config.EnableVSync = GetPrivateProfileIntA("d3d9", "EnableVSync", 0, configPath) ? TRUE : FALSE;
        g_Config.ForceVSyncMode = GetPrivateProfileIntA("d3d9", "ForceVSyncMode", 1, configPath) ? TRUE : FALSE;
        g_Config.EnableWindowMode = GetPrivateProfileIntA("d3d9", "EnableWindowMode", 0, configPath) ? TRUE : FALSE;
        
        g_Config.AutoFallback = GetPrivateProfileIntA("Performance", "AutoFallback", 1, configPath) ? TRUE : FALSE;
        g_Config.MinPerformanceMode = GetPrivateProfileIntA("Performance", "MinPerformanceMode", 0, configPath) ? TRUE : FALSE;
    }

    void Log(const char* format, ...)
    {
        if (!g_Config.debugOutput) return;
        
        char buffer[512];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        
        OutputDebugStringA(buffer);
    }
}
