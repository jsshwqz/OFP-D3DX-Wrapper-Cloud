#include "config.hpp"
#include "d3d8to9.hpp"
#include "d3d8-wrapper.hpp"
#include <d3d9.h>
#include <stdio.h>
#include <string.h>

static HMODULE g_hRealD3D8 = nullptr;
static BOOL g_bInitialized = FALSE;

typedef IDirect3D8* (WINAPI* Direct3DCreate8_t)(UINT SDKVersion);
static Direct3DCreate8_t g_pRealDirect3DCreate8 = nullptr;

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (!g_bInitialized)
        {
            g_bInitialized = TRUE;
            Config::Load();
            Config::Log("OFP D3DX Wrapper: DLL_PROCESS_ATTACH\n");

            char sysPath[MAX_PATH];
            GetSystemDirectoryA(sysPath, MAX_PATH);
            strcat(sysPath, "\\d3d8.dll");

            g_hRealD3D8 = LoadLibraryA(sysPath);
            if (g_hRealD3D8)
            {
                g_pRealDirect3DCreate8 = (Direct3DCreate8_t)GetProcAddress(g_hRealD3D8, "Direct3DCreate8");
                if (g_pRealDirect3DCreate8)
                {
                    Config::Log("OFP D3DX Wrapper: Loaded real d3d8.dll\n");
                }
            }

            if (Config::g_Config.EnableWrapper && Config::g_Config.D3d8to9)
            {
                D3D8To9::Init();
            }
        }
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        Config::Log("OFP D3DX Wrapper: DLL_PROCESS_DETACH\n");
        
        D3D8To9::Shutdown();
        
        if (g_hRealD3D8)
        {
            FreeLibrary(g_hRealD3D8);
            g_hRealD3D8 = nullptr;
        }
    }
    
    return TRUE;
}

extern "C" __declspec(dllexport) IDirect3D8* WINAPI Direct3DCreate8(UINT SDKVersion)
{
    Config::Log("OFP D3DX Wrapper: Direct3DCreate8(SDKVersion=%u)\n", SDKVersion);

    if (!Config::g_Config.EnableWrapper || !Config::g_Config.D3d8to9)
    {
        if (g_pRealDirect3DCreate8)
            return g_pRealDirect3DCreate8(SDKVersion);
        return nullptr;
    }

    if (D3D8To9::g_bFallbackMode)
    {
        Config::Log("OFP D3DX Wrapper: Fallback mode - using real d3d8\n");
        if (g_pRealDirect3DCreate8)
            return g_pRealDirect3DCreate8(SDKVersion);
        return nullptr;
    }

    if (!D3D8To9::g_pD3D9)
    {
        D3D8To9::Init();
    }

    if (!D3D8To9::g_pD3D9)
    {
        Config::Log("OFP D3DX Wrapper: No D3D9 - fallback\n");
        if (g_pRealDirect3DCreate8)
            return g_pRealDirect3DCreate8(SDKVersion);
        return nullptr;
    }

    Config::Log("OFP D3DX Wrapper: Creating D3D8 wrapper with D3D9 enhancements\n");
    Config::Log("  - Anisotropic filtering: %dx\n", Config::g_Config.AnisotropicFiltering);
    Config::Log("  - VSync: %s\n", Config::g_Config.EnableVSync ? "ON" : "OFF");

    return CreateD3D8Wrapper(D3D8To9::g_pD3D9);
}
