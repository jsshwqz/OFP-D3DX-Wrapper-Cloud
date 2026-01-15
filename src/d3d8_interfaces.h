#pragma once

#include <windows.h>

typedef enum { D3DDEVTYPE_HAL = 1, D3DDEVTYPE_REF = 2, D3DDEVTYPE_SW = 3, D3DDEVTYPE_NULLREF = 4 } D3DDEVTYPE;
typedef enum { D3DFMT_UNKNOWN = 0, D3DFMT_R8G8B8 = 20, D3DFMT_A8R8G8B8 = 21, D3DFMT_X8R8G8B8 = 22, D3DFMT_R5G6B5 = 23, D3DFMT_X1R5G5B5 = 24, D3DFMT_A1R5G5B5 = 25, D3DFMT_A4R4G4B4 = 26, D3DFMT_A8 = 28, D3DFMT_D16 = 80, D3DFMT_D24S8 = 75, D3DFMT_D32 = 81, D3DFMT_INDEX16 = 101, D3DFMT_INDEX32 = 102 } D3DFORMAT;
typedef enum { D3DRESOURCETYPE_SURFACE = 1, D3DRESOURCETYPE_VOLUME = 2, D3DRESOURCETYPE_TEXTURE = 3, D3DRESOURCETYPE_VOLUMETEXTURE = 4, D3DRESOURCETYPE_CUBETEXTURE = 5, D3DRESOURCETYPE_VERTEXBUFFER = 6, D3DRESOURCETYPE_INDEXBUFFER = 7 } D3DRESOURCETYPE;
typedef enum { D3DPRIMITIVETYPE_POINTLIST = 1, D3DPRIMITIVETYPE_LINELIST = 2, D3DPRIMITIVETYPE_LINESTRIP = 3, D3DPRIMITIVETYPE_TRIANGLELIST = 4, D3DPRIMITIVETYPE_TRIANGLESTRIP = 5, D3DPRIMITIVETYPE_TRIANGLEFAN = 6 } D3DPRIMITIVETYPE;
typedef enum { D3DTRANSFORMSTATETYPE_WORLD = 0, D3DTRANSFORMSTATETYPE_VIEW = 1, D3DTRANSFORMSTATETYPE_PROJECTION = 2 } D3DTRANSFORMSTATETYPE;
typedef enum { D3DRENDERSTATETYPE_ZENABLE = 1, D3DRENDERSTATETYPE_FILLMODE = 3, D3DRENDERSTATETYPE_CULLMODE = 22 } D3DRENDERSTATETYPE;
typedef enum { D3DTEXTURESTAGESTATETYPE_COLOROP = 1, D3DTEXTURESTAGESTATETYPE_COLORARG1 = 2, D3DTEXTURESTAGESTATETYPE_ALPHAOP = 4 } D3DTEXTURESTAGESTATETYPE;
typedef enum { D3DSAMPLERSTATETYPE_MINFILTER = 0, D3DSAMPLERSTATETYPE_MAGFILTER = 1, D3DSAMPLERSTATETYPE_MIPFILTER = 2, D3DSAMPLERSTATETYPE_ADDRESSU = 3, D3DSAMPLERSTATETYPE_ADDRESSV = 4, D3DSAMPLERSTATETYPE_MAXANISOTROPY = 9 } D3DSAMPLERSTATETYPE;
typedef enum { D3DMULTISAMPLE_NONE = 0, D3DMULTISAMPLE_2_SAMPLES = 1, D3DMULTISAMPLE_4_SAMPLES = 3, D3DMULTISAMPLE_8_SAMPLES = 7 } D3DMULTISAMPLE_TYPE;
typedef enum { D3DPOOL_DEFAULT = 0, D3DPOOL_MANAGED = 1, D3DPOOL_SYSTEMMEM = 2 } D3DPOOL;
typedef enum { D3DQUERYTYPE_EVENT = 8, D3DQUERYTYPE_OCCLUSION = 9 } D3DQUERYTYPE;

typedef struct { UINT X, Y, Width, Height, MinZ, MaxZ; } D3DVIEWPORT8;
typedef struct { UINT Width, Height, RefreshRate; D3DFORMAT Format; } D3DDISPLAYMODE;
typedef struct { WORD red[256], green[256], blue[256]; } D3DGAMMARAMP;
typedef DWORD D3DCOLOR;
typedef enum { D3DLIGHTTYPE_POINT = 1, D3DLIGHTTYPE_SPOT = 2, D3DLIGHTTYPE_DIRECTIONAL = 3 } D3DLIGHTTYPE;
typedef struct { D3DLIGHTTYPE Type; D3DCOLOR Color; FLOAT PositionX, PositionY, PositionZ, DirectionX, DirectionY, DirectionZ, Range, Falloff, Attenuation0, Attenuation1, Attenuation2, Theta, Phi; } D3DLIGHT8;
typedef struct { D3DCOLOR Diffuse, Ambient, Specular, Emissive; FLOAT Power; } D3DMATERIAL8;
typedef struct { UINT AdapterOrdinal; D3DDEVTYPE DeviceType; HWND hFocusWindow; DWORD BehaviorFlags; } D3DDEVICE_CREATION_PARAMETERS;
typedef struct { BOOL InVBlank; UINT ScanLine; } D3DRASTER_STATUS;
typedef struct { BYTE Stream, Offset; BYTE Type, Method, Usage, UsageIndex; } D3DVERTEXELEMENT9;

typedef struct _D3DADAPTER_IDENTIFIER8 { char Driver[512], Description[512]; DWORD DriverVersionLowPart, DriverVersionHighPart, VendorId, DeviceId, SubSysId, Revision; GUID DeviceIdentifier; DWORD WHQLLevel; } D3DADAPTER_IDENTIFIER8;

typedef struct _D3DCAPS8 {
    DWORD DeviceType, Caps, Caps2, Caps3, Caps4, PresentationIntervals, CursorCaps, DevCaps, MiscCaps, RasterCaps, ZCmpCaps, SrcBlendCaps, DestBlendCaps, AlphaCmpCaps, ShadeCaps, TextureCaps, TextureFilterCaps, CubeTextureFilterCaps, VolumeTextureFilterCaps, TextureAddressCaps, VolumeTextureAddressCaps, LineCaps, StencilCaps, FVFCaps, TextureOpCaps, MaxTextureBlendStages, MaxSimultaneousTextures, MaxUserClipPlanes, MaxActiveLightCount, OcclusionQuerySupportMask, VertexTextureFilterCaps, MaxVShaderInstructionsExecuted, MaxPShaderInstructionsExecuted, MaxVertexShader30InstructionSlots, MaxPixelShader30InstructionSlots, NumSimultaneousRTs, VS20Caps, PS20Caps;
    UINT AdapterOrdinal, MaxTextureWidth, MaxTextureHeight, MaxVolumeExtent, MaxTextureRepeat, MaxTextureAspectRatio;
    DWORD MaxAnisotropy;
    float MaxVertexW, GuardBandLeft, GuardBandRight, GuardBandTop, GuardBandBottom, ExtentsAdjust;
} D3DCAPS8;

typedef struct { UINT BackBufferWidth, BackBufferHeight, BackBufferCount; D3DFORMAT BackBufferFormat; D3DMULTISAMPLE_TYPE MultiSampleType; DWORD SwapEffect; HWND hDeviceWindow; BOOL Windowed, EnableAutoDepthStencil; D3DFORMAT AutoDepthStencilFormat; DWORD Flags; UINT FullScreen_RefreshRateInHz, FullScreen_PresentationInterval; } D3DPRESENT_PARAMETERS8;

interface IDirect3D8;
interface IDirect3DDevice8;
interface IDirect3DSurface8;
interface IDirect3DBaseTexture8;
interface IDirect3DTexture8;
interface IDirect3DVolumeTexture8;
interface IDirect3DCubeTexture8;
interface IDirect3DVertexBuffer8;
interface IDirect3DIndexBuffer8;
interface IDirect3DSwapChain8;
interface IDirect3DStateBlock8;
interface IDirect3DVertexShader8;
interface IDirect3DPixelShader8;
interface IDirect3DQuery8;
interface IDirect3DVertexDeclaration9;

interface IDirect3D8 : public IUnknown {
    STDMETHOD_(UINT, GetAdapterCount)() PURE;
    STDMETHOD(GetAdapterIdentifier)(UINT, DWORD, D3DADAPTER_IDENTIFIER8*) PURE;
    STDMETHOD_(UINT, GetAdapterModeCount)(UINT) PURE;
    STDMETHOD(EnumAdapterModes)(UINT, UINT, D3DDISPLAYMODE*) PURE;
    STDMETHOD(GetAdapterDisplayMode)(UINT, D3DDISPLAYMODE*) PURE;
    STDMETHOD(CheckDeviceType)(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, BOOL) PURE;
    STDMETHOD(CheckDeviceFormat)(UINT, D3DDEVTYPE, D3DFORMAT, DWORD, D3DRESOURCETYPE, D3DFORMAT) PURE;
    STDMETHOD(CheckDeviceMultiSampleType)(UINT, D3DDEVTYPE, D3DFORMAT, BOOL, D3DMULTISAMPLE_TYPE) PURE;
    STDMETHOD(CheckDepthStencilMatch)(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, D3DFORMAT) PURE;
    STDMETHOD(GetDeviceCaps)(UINT, D3DDEVTYPE, D3DCAPS8*) PURE;
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT) PURE;
    STDMETHOD(CreateDevice)(UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS8*, IDirect3DDevice8**) PURE;
};

interface IDirect3DDevice8 : public IUnknown {
    STDMETHOD(TestCooperativeLevel)() PURE;
    STDMETHOD_(UINT, GetAvailableTextureMem)() PURE;
    STDMETHOD(GetDeviceCaps)(D3DCAPS8*) PURE;
    STDMETHOD(GetDisplayMode)(D3DDISPLAYMODE*) PURE;
    STDMETHOD(GetCreationParameters)(D3DDEVICE_CREATION_PARAMETERS*) PURE;
    STDMETHOD(SetCursorProperties)(UINT, UINT, IDirect3DSurface8*) PURE;
    STDMETHOD_(void, SetCursorPosition)(int, int, DWORD) PURE;
    STDMETHOD_(BOOL, ShowCursor)(BOOL) PURE;
    STDMETHOD(Reset)(D3DPRESENT_PARAMETERS8*) PURE;
    STDMETHOD(Present)(CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*) PURE;
    STDMETHOD(GetBackBuffer)(UINT, UINT, IDirect3DSurface8**) PURE;
    STDMETHOD(GetRasterStatus)(D3DRASTER_STATUS*) PURE;
    STDMETHOD_(void, SetGammaRamp)(DWORD, CONST D3DGAMMARAMP*) PURE;
    STDMETHOD_(void, GetGammaRamp)(D3DGAMMARAMP*) PURE;
    STDMETHOD(CreateTexture)(UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DTexture8**, HANDLE*) PURE;
    STDMETHOD(CreateVolumeTexture)(UINT, UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DVolumeTexture8**, HANDLE*) PURE;
    STDMETHOD(CreateCubeTexture)(UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DCubeTexture8**, HANDLE*) PURE;
    STDMETHOD(CreateVertexBuffer)(UINT, DWORD, DWORD, D3DPOOL, IDirect3DVertexBuffer8**, HANDLE*) PURE;
    STDMETHOD(CreateIndexBuffer)(UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DIndexBuffer8**, HANDLE*) PURE;
    STDMETHOD(CreateRenderTarget)(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, BOOL, IDirect3DSurface8**, HANDLE*) PURE;
    STDMETHOD(CreateDepthStencilSurface)(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, IDirect3DSurface8**, HANDLE*) PURE;
    STDMETHOD(SetRenderTarget)(IDirect3DSurface8*, IDirect3DSurface8*) PURE;
    STDMETHOD(GetRenderTarget)(IDirect3DSurface8**) PURE;
    STDMETHOD(GetDepthStencilSurface)(IDirect3DSurface8**) PURE;
    STDMETHOD(BeginScene)() PURE;
    STDMETHOD(EndScene)() PURE;
    STDMETHOD(Clear)(DWORD, CONST D3DRECT*, DWORD, D3DCOLOR, float, DWORD) PURE;
    STDMETHOD(SetTransform)(D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*) PURE;
    STDMETHOD(GetTransform)(D3DTRANSFORMSTATETYPE, D3DMATRIX*) PURE;
    STDMETHOD(MultiplyTransform)(D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*) PURE;
    STDMETHOD(SetViewport)(CONST D3DVIEWPORT8*) PURE;
    STDMETHOD(GetViewport)(D3DVIEWPORT8*) PURE;
    STDMETHOD(SetMaterial)(CONST D3DMATERIAL8*) PURE;
    STDMETHOD(SetLight)(DWORD, CONST D3DLIGHT8*) PURE;
    STDMETHOD(LightEnable)(DWORD, BOOL) PURE;
    STDMETHOD(SetClipPlane)(DWORD, CONST float*) PURE;
    STDMETHOD(SetRenderState)(D3DRENDERSTATETYPE, DWORD) PURE;
    STDMETHOD(GetRenderState)(D3DRENDERSTATETYPE, DWORD*) PURE;
    STDMETHOD(GetTexture)(DWORD, IDirect3DBaseTexture8**) PURE;
    STDMETHOD(SetTexture)(DWORD, IDirect3DBaseTexture8*) PURE;
    STDMETHOD(SetTextureStageState)(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD) PURE;
    STDMETHOD(SetSamplerState)(DWORD, D3DSAMPLERSTATETYPE, DWORD) PURE;
    STDMETHOD(SetScissorRect)(CONST RECT*) PURE;
    STDMETHOD(SetSoftwareVertexProcessing)(BOOL) PURE;
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)() PURE;
    STDMETHOD(SetNPatchMode)(float) PURE;
    STDMETHOD_(float, GetNPatchMode)() PURE;
    STDMETHOD(DrawPrimitive)(D3DPRIMITIVETYPE, UINT, UINT) PURE;
    STDMETHOD(DrawIndexedPrimitive)(D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT) PURE;
    STDMETHOD(DrawPrimitiveUP)(D3DPRIMITIVETYPE, UINT, CONST void*, UINT) PURE;
    STDMETHOD(DrawIndexedPrimitiveUP)(D3DPRIMITIVETYPE, UINT, UINT, UINT, CONST void*, D3DFORMAT, CONST void*, UINT) PURE;
    STDMETHOD(SetStreamSource)(UINT, IDirect3DVertexBuffer8*, UINT) PURE;
    STDMETHOD(SetIndices)(IDirect3DIndexBuffer8*, UINT) PURE;
    STDMETHOD(SetFVF)(DWORD) PURE;
    STDMETHOD(SetVertexShader)(DWORD) PURE;
    STDMETHOD(SetPixelShader)(DWORD) PURE;
    STDMETHOD(DeletePatch)(UINT) PURE;
};

interface IDirect3DSurface8 : public IUnknown {};
interface IDirect3DBaseTexture8 : public IUnknown {};
interface IDirect3DTexture8 : public IDirect3DBaseTexture8 {};
interface IDirect3DVolumeTexture8 : public IDirect3DBaseTexture8 {};
interface IDirect3DCubeTexture8 : public IDirect3DBaseTexture8 {};
interface IDirect3DVertexBuffer8 : public IUnknown {};
interface IDirect3DIndexBuffer8 : public IUnknown {};
interface IDirect3DSwapChain8 : public IUnknown {};
interface IDirect3DStateBlock8 : public IUnknown {};
interface IDirect3DVertexShader8 : public IUnknown {};
interface IDirect3DPixelShader8 : public IUnknown {};
interface IDirect3DQuery8 : public IUnknown {};
interface IDirect3DVertexDeclaration9 : public IUnknown {};

#ifdef INITGUID
DEFINE_GUID(IID_IDirect3D8, 0x1DD9E8DA, 0x6C27, 0x41C8, 0xA7, 0xE7, 0x76, 0x15, 0xEE, 0xF7, 0x5C, 0x55);
#else
extern const IID IID_IDirect3D8;
#endif
