#pragma once

#include <d3d9.h>

interface IDirect3D8;
interface IDirect3DDevice8;
interface IDirect3DTexture8;
interface IDirect3DSurface8;
interface IDirect3DVertexBuffer8;
interface IDirect3DIndexBuffer8;

typedef interface IDirect3D8 IDirect3D8;
typedef interface IDirect3DDevice8 IDirect3DDevice8;
typedef interface IDirect3DTexture8 IDirect3DTexture8;
typedef interface IDirect3DSurface8 IDirect3DSurface8;
typedef interface IDirect3DVertexBuffer8 IDirect3DVertexBuffer8;
typedef interface IDirect3DIndexBuffer8 IDirect3DIndexBuffer8;

#define D3D8_SDK_VERSION 220

IDirect3D8* CreateD3D8Wrapper(IDirect3D9* pD3D9, UINT SDKVersion);
