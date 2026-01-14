#ifndef D3D8TO9_HPP
#define D3D8TO9_HPP

#pragma once

#include <d3d9.h>
#include <Windows.h>

interface IDirect3D8;
interface IDirect3DDevice8;

typedef interface IDirect3D8 IDirect3D8;
typedef interface IDirect3DDevice8 IDirect3DDevice8;

extern "C" 
{
    typedef IDirect3D8* (WINAPI* Direct3DCreate8_t)(UINT SDKVersion);
    extern Direct3DCreate8_t Original_Direct3DCreate8;
}

IDirect3D8* WINAPI Hook_Direct3DCreate8(UINT SDKVersion);

namespace D3D8To9
{
    extern IDirect3D9* g_pD3D9;
    extern HMODULE g_hOriginalD3D9;
    extern BOOL g_bFallbackMode;
    
    void Init();
    void Shutdown();
    BOOL IsActive();
    void SetFallbackMode(BOOL enabled);
    IDirect3D9* GetD3D9();
}

#endif // D3D8TO9_HPP
