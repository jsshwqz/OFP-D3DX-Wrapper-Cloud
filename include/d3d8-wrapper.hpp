#pragma once

#include <d3d9.h>

#ifdef __cplusplus
extern "C" {
#endif

IDirect3D8* __cdecl CreateD3D8Wrapper(IDirect3D9* pD3D9);

#ifdef __cplusplus
}
#endif
