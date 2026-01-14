#include "config.hpp"
#include "d3d8to9.hpp"
#include <d3d9.h>

namespace D3D8To9
{
    IDirect3D9* g_pD3D9 = nullptr;
    HMODULE g_hOriginalD3D9 = nullptr;
    BOOL g_bFallbackMode = FALSE;
    
    typedef IDirect3D9* (WINAPI* Direct3DCreate9_t)(UINT SDKVersion);
    Direct3DCreate9_t Original_Direct3DCreate9 = nullptr;
    
    void Init()
    {
        Config::Load();
        Config::Log("D3D8To9: Initializing...\n");
        
        if (!Config::g_Config.D3d8to9)
        {
            Config::Log("D3D8To9: Disabled by config\n");
            return;
        }
        
        g_hOriginalD3D9 = LoadLibraryA("d3d9.dll");
        if (!g_hOriginalD3D9)
        {
            Config::Log("D3D8To9: Failed to load d3d9.dll\n");
            g_bFallbackMode = TRUE;
            return;
        }
        
        Original_Direct3DCreate9 = (Direct3DCreate9_t)GetProcAddress(g_hOriginalD3D9, "Direct3DCreate9");
        if (!Original_Direct3DCreate9)
        {
            Config::Log("D3D8To9: Failed to get Direct3DCreate9\n");
            FreeLibrary(g_hOriginalD3D9);
            g_hOriginalD3D9 = nullptr;
            g_bFallbackMode = TRUE;
            return;
        }
        
        g_pD3D9 = Original_Direct3DCreate9(220);
        if (!g_pD3D9)
        {
            Config::Log("D3D8To9: Failed to create D3D9 interface\n");
            g_bFallbackMode = TRUE;
            return;
        }
        
        Config::Log("D3D8To9: Initialized successfully (D3D9=%p)\n", g_pD3D9);
    }
    
    void Shutdown()
    {
        Config::Log("D3D8To9: Shutting down...\n");
        
        if (g_pD3D9)
        {
            g_pD3D9->Release();
            g_pD3D9 = nullptr;
        }
        
        if (g_hOriginalD3D9)
        {
            FreeLibrary(g_hOriginalD3D9);
            g_hOriginalD3D9 = nullptr;
        }
        
        Config::Log("D3D8To9: Shutdown complete\n");
    }
    
    BOOL IsActive()
    {
        return Config::g_Config.EnableWrapper && 
               Config::g_Config.D3d8to9 && 
               !g_bFallbackMode &&
               g_pD3D9 != nullptr;
    }
    
    void SetFallbackMode(BOOL enabled)
    {
        g_bFallbackMode = enabled;
        Config::Log("D3D8To9: Fallback mode %s\n", enabled ? "enabled" : "disabled");
    }
    
    IDirect3D9* GetD3D9()
    {
        return g_pD3D9;
    }
}
