#pragma once

#include <d3d9.h>

interface IDirect3D8;
interface IDirect3DDevice8;

typedef interface IDirect3D8 IDirect3D8;
typedef interface IDirect3DDevice8 IDirect3DDevice8;

IDirect3D8* CreateD3D8Wrapper(IDirect3D9* pD3D9);
