#ifndef HOOKS_HPP
#define HOOKS_HPP

#pragma once

#include <d3d9.h>

namespace Hooks
{
    BOOL Install(IDirect3DDevice9* pDevice);
    BOOL Uninstall();
    BOOL IsActive();
    IDirect3DDevice9* GetHookedDevice();
    void Update();
    DWORD GetCurrentFPS();
    FLOAT GetCurrentFOV();
    void SetDefaultFOV(FLOAT fov);
}

#endif // HOOKS_HPP
