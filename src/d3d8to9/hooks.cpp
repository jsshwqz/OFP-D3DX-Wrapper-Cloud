#pragma once

#include "config.hpp"
#include "d3d8to9.hpp"
#include <MinHook.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <time.h>

#pragma comment(lib, "d3dx9.lib")

typedef HRESULT(WINAPI* EndScene_t)(IDirect3DDevice9* pDevice);
typedef HRESULT(WINAPI* SetViewport_t)(IDirect3DDevice9* pDevice, const D3DVIEWPORT9* pViewport);
typedef HRESULT(WINAPI* SetSamplerState_t)(IDirect3DDevice9* pDevice, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
typedef HRESULT(WINAPI* CreateTexture_t)(IDirect3DDevice9* pDevice, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle);

static EndScene_t Original_EndScene = nullptr;
static SetViewport_t Original_SetViewport = nullptr;
static SetSamplerState_t Original_SetSamplerState = nullptr;
static CreateTexture_t Original_CreateTexture = nullptr;

static IDirect3DDevice9* g_pHookedDevice = nullptr;
static IDirect3D9* g_pD3D9Hook = nullptr;
static BOOL g_bHooksInstalled = FALSE;
static BOOL g_bTextureHooked = FALSE;

static DWORD g_dwFrameCount = 0;
static DWORD g_dwLastFPSTime = 0;
static DWORD g_dwCurrentFPS = 0;
static FLOAT g_fCurrentFOV = 90.0f;
static BOOL g_bFOVAdjusted = FALSE;
static BOOL g_bKeyState[256] = {0};
static ID3DXFont* g_pFPSFont = nullptr;

static CRITICAL_SECTION g_csFPS;

static VOID InitializeCriticalSection_wrap(VOID)
{
    InitializeCriticalSection(&g_csFPS);
}

static VOID DeleteCriticalSection_wrap(VOID)
{
    DeleteCriticalSection(&g_csFPS);
}

static VOID EnterCriticalSection_wrap(VOID)
{
    EnterCriticalSection(&g_csFPS);
}

static VOID LeaveCriticalSection_wrap(VOID)
{
    LeaveCriticalSection(&g_csFPS);
}

static HRESULT WINAPI Hooked_EndScene(IDirect3DDevice9* pDevice)
{
    if (pDevice != g_pHookedDevice)
    {
        return Original_EndScene(pDevice);
    }

    DWORD currentTime = GetTickCount();

    EnterCriticalSection_wrap();
    g_dwFrameCount++;
    
    if (currentTime - g_dwLastFPSTime >= (Config::g_Config.FPSUpdateInterval * 1000))
    {
        g_dwCurrentFPS = g_dwFrameCount;
        g_dwFrameCount = 0;
        g_dwLastFPSTime = currentTime;
    }

    if (Config::g_Config.ShowFPS && g_pFPSFont)
    {
        RECT rect;
        rect.left = Config::g_Config.FPSPosX;
        rect.top = Config::g_Config.FPSPosY;
        rect.right = rect.left + 200;
        rect.bottom = rect.top + 50;

        char fpsText[64];
        snprintf(fpsText, sizeof(fpsText), "FPS: %u", g_dwCurrentFPS);

        g_pFPSFont->DrawTextA(NULL, fpsText, -1, &rect, DT_LEFT | DT_TOP, D3DCOLOR_XRGB(0, 255, 0));
    }

    LeaveCriticalSection_wrap();

    return Original_EndScene(pDevice);
}

static BOOL g_bAnisotropicSet = FALSE;

static HRESULT WINAPI Hooked_SetSamplerState(IDirect3DDevice9* pDevice, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
    if (Config::g_Config.AnisotropicFiltering > 0 && 
        Sampler == 0 && 
        Type == D3DSAMP_MAGFILTER)
    {
        if (!g_bAnisotropicSet)
        {
            Config::Log("Hook: Applying anisotropic filtering %dx\n", Config::g_Config.AnisotropicFiltering);
            g_bAnisotropicSet = TRUE;
        }
        return Original_SetSamplerState(pDevice, Sampler, Type, D3DTEXF_ANISOTROPIC);
    }

    return Original_SetSamplerState(pDevice, Sampler, Type, Value);
}

static VOID CheckFOVKey()
{
    if (!Config::g_Config.EnableFOV || Config::g_Config.FOVToggleKey == 0)
        return;

    BYTE keyState = GetAsyncKeyState((int)Config::g_Config.FOVToggleKey);
    BOOL pressed = (keyState & 0x8000) != 0;

    if (pressed && !g_bKeyState[Config::g_Config.FOVToggleKey])
    {
        if (g_fCurrentFOV >= 90.0f)
        {
            g_fCurrentFOV = 60.0f;
            Config::Log("FOV: Switched to aiming FOV (60)\n");
        }
        else
        {
            g_fCurrentFOV = (FLOAT)Config::g_Config.DefaultFOV;
            Config::Log("FOV: Switched to default FOV (%u)\n", Config::g_Config.DefaultFOV);
        }
    }

    g_bKeyState[Config::g_Config.FOVToggleKey] = pressed;
}

static BOOL g_bOriginalAnisotropic = FALSE;

static void ApplyAnisotropicFiltering(IDirect3DDevice9* pDevice)
{
    if (Config::g_Config.AnisotropicFiltering > 0)
    {
        for (UINT i = 0; i < 8; i++)
        {
            pDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
            pDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
            pDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, Config::g_Config.AnisotropicFiltering);
        }
        Config::Log("Applied anisotropic filtering %dx\n", Config::g_Config.AnisotropicFiltering);
        g_bOriginalAnisotropic = TRUE;
    }
}

static void InitializeFont(IDirect3DDevice9* pDevice)
{
    if (g_pFPSFont)
        return;

    D3DXFONT_DESCA fontDesc;
    memset(&fontDesc, 0, sizeof(fontDesc));
    fontDesc.Height = 24;
    fontDesc.Width = 12;
    fontDesc.Weight = FW_NORMAL;
    fontDesc.MipLevels = 1;
    fontDesc.Italic = FALSE;
    fontDesc.CharSet = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    strcpy_s(fontDesc.FaceName, "Arial");

    if (SUCCEEDED(D3DXCreateFontIndirectA(pDevice, &fontDesc, &g_pFPSFont)))
    {
        Config::Log("FPS font created successfully\n");
    }
    else
    {
        Config::Log("Failed to create FPS font\n");
    }
}

static void ShutdownFont()
{
    if (g_pFPSFont)
    {
        g_pFPSFont->Release();
        g_pFPSFont = nullptr;
    }
}

namespace Hooks
{
    BOOL Install(IDirect3DDevice9* pDevice)
    {
        if (g_bHooksInstalled || !pDevice)
            return FALSE;

        Config::Log("Hooks: Installing hooks for device %p\n", pDevice);

        InitializeCriticalSection_wrap();

        if (MH_Initialize() != MH_OK)
        {
            Config::Log("Hooks: MinHook initialization failed\n");
            return FALSE;
        }

        HMODULE hD3D9 = GetModuleHandleA("d3d9.dll");
        if (!hD3D9)
        {
            Config::Log("Hooks: Failed to get d3d9.dll handle\n");
            return FALSE;
        }

        void* pEndScene = (void*)GetProcAddress(hD3D9, "EndScene");
        void* pSetViewport = (void*)GetProcAddress(hD3D9, "SetViewport");
        void* pSetSamplerState = (void*)GetProcAddress(hD3D9, "SetSamplerState");

        if (!pEndScene || !pSetViewport || !pSetSamplerState)
        {
            Config::Log("Hooks: Failed to get D3D9 function addresses\n");
            return FALSE;
        }

        if (MH_CreateHook(pEndScene, Hooked_EndScene, (void**)&Original_EndScene) != MH_OK)
        {
            Config::Log("Hooks: Failed to create EndScene hook\n");
            return FALSE;
        }

        if (MH_CreateHook(pSetSamplerState, Hooked_SetSamplerState, (void**)&Original_SetSamplerState) != MH_OK)
        {
            Config::Log("Hooks: Failed to create SetSamplerState hook\n");
            return FALSE;
        }

        if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
        {
            Config::Log("Hooks: Failed to enable hooks\n");
            return FALSE;
        }

        g_pHookedDevice = pDevice;
        pDevice->AddRef();

        ApplyAnisotropicFiltering(pDevice);
        InitializeFont(pDevice);

        g_bHooksInstalled = TRUE;
        g_dwLastFPSTime = GetTickCount();

        Config::Log("Hooks: Successfully installed\n");
        return TRUE;
    }

    BOOL Uninstall()
    {
        if (!g_bHooksInstalled)
            return TRUE;

        Config::Log("Hooks: Uninstalling hooks\n");

        if (g_pHookedDevice)
        {
            ShutdownFont();
            g_pHookedDevice->Release();
            g_pHookedDevice = nullptr;
        }

        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();

        DeleteCriticalSection_wrap();

        g_bHooksInstalled = FALSE;
        Config::Log("Hooks: Successfully uninstalled\n");
        return TRUE;
    }

    BOOL IsActive()
    {
        return g_bHooksInstalled;
    }

    IDirect3DDevice9* GetHookedDevice()
    {
        return g_pHookedDevice;
    }

    void Update()
    {
        CheckFOVKey();
    }

    DWORD GetCurrentFPS()
    {
        return g_dwCurrentFPS;
    }

    FLOAT GetCurrentFOV()
    {
        return g_fCurrentFOV;
    }

    void SetDefaultFOV(FLOAT fov)
    {
        g_fCurrentFOV = fov;
    }
}
