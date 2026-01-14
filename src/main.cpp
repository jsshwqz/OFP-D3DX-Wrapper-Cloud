#include "config.hpp"
#include "d3d8to9.hpp"
#include "d3d8to9-wrapper.hpp"
#include <d3d9.h>

namespace D3D8
{
    HMODULE g_hOriginalD3D8 = nullptr;
    Direct3DCreate8_t Original_Direct3DCreate8 = nullptr;
    
    IDirect3D8* WINAPI Hook_Direct3DCreate8(UINT SDKVersion)
    {
        Config::Log("D3D8: Direct3DCreate8(SDKVersion=%u)\n", SDKVersion);
        
        if (!Config::g_Config.EnableWrapper || !Config::g_Config.D3d8to9)
        {
            if (Original_Direct3DCreate8)
                return Original_Direct3DCreate8(SDKVersion);
            return nullptr;
        }
        
        if (D3D8To9::g_bFallbackMode)
        {
            Config::Log("D3D8: Using fallback mode\n");
            if (Original_Direct3DCreate8)
                return Original_Direct3DCreate8(SDKVersion);
            return nullptr;
        }
        
        D3D8To9::Init();
        
        if (!D3D8To9::g_pD3D9)
        {
            Config::Log("D3D8: No D3D9 available, using fallback\n");
            if (Original_Direct3DCreate8)
                return Original_Direct3DCreate8(SDKVersion);
            return nullptr;
        }
        
        Config::Log("D3D8: Redirecting to D3D9 wrapper\n");
        
        return D3D8To9::CreateD3D8Wrapper(D3D8To9::g_pD3D9, SDKVersion);
    }
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        Config::Load();
        Config::Log("OFP D3DX Wrapper: Loaded (ProcessAttach)\n");
        
        if (Config::g_Config.EnableWrapper && Config::g_Config.D3d8to9)
        {
            D3D8::g_hOriginalD3D8 = LoadLibraryA("d3d8.dll");
            if (D3D8::g_hOriginalD3D8)
            {
                D3D8::Original_Direct3DCreate8 = 
                    (D3D8::Direct3DCreate8_t)GetProcAddress(D3D8::g_hOriginalD3D8, "Direct3DCreate8");
                
                if (D3D8::Original_Direct3DCreate8)
                {
                    BYTE* pAddr = (BYTE*)D3D8::Original_Direct3DCreate8;
                    DWORD oldProtect;
                    if (VirtualProtect(pAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
                    {
                        pAddr[0] = 0xE9;
                        *(DWORD*)(pAddr + 1) = (DWORD)D3D8::Hook_Direct3DCreate8 - (DWORD)pAddr - 5;
                        VirtualProtect(pAddr, 5, oldProtect, &oldProtect);
                        Config::Log("OFP D3DX Wrapper: Hook installed\n");
                    }
                }
            }
        }
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        Config::Log("OFP D3DX Wrapper: Unloading...\n");
        
        D3D8To9::Shutdown();
        
        if (D3D8::g_hOriginalD3D8)
        {
            FreeLibrary(D3D8::g_hOriginalD3D8);
            D3D8::g_hOriginalD3D8 = nullptr;
        }
        
        Config::Log("OFP D3DX Wrapper: Unloaded\n");
    }
    
    return TRUE;
}

extern "C" __declspec(dllexport) void DummyExport() {}
