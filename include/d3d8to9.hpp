#ifndef D3D8TO9_HPP
#define D3D8TO9_HPP

#pragma once

#include <d3d9.h>

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

void D3D8To9_Init();
void D3D8To9_Shutdown();

BOOL D3D8To9_IsActive();
void D3D8To9_SetFallbackMode(BOOL enabled);

#endif // D3D8TO9_HPP
