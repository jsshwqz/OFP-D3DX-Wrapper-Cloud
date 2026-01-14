#ifndef D3D8TO9_WRAPPER_HPP
#define D3D8TO9_WRAPPER_HPP

#pragma once

#include <d3d9.h>

interface IDirect3D8;
interface IDirect3DDevice8;
interface IDirect3DTexture8;
interface IDirect3DSurface8;
interface IDirect3DVertexBuffer8;
interface IDirect3DIndexBuffer8;

#define D3D8_SDK_VERSION 220

typedef enum _D3DPRIMITIVETYPE {
    D3DPT_POINTLIST = 1,
    D3DPT_LINELIST = 2,
    D3DPT_TRIANGLELIST = 3,
    D3DPT_TRIANGLESTRIP = 4,
    D3DPT_TRIANGLEFAN = 5
} D3DPRIMITIVETYPE;

typedef enum _D3DRENDERSTATETYPE {
    D3DRS_ZENABLE = 7,
    D3DRS_FILLMODE = 8,
    D3DRS_SHADEMODE = 9,
    D3DRS_ZWRITEENABLE = 14,
    D3DRS_ALPHATESTENABLE = 15,
    D3DRS_LASTPIXEL = 16,
    D3DRS_SRCBLEND = 19,
    D3DRS_DESTBLEND = 20,
    D3DRS_CULLMODE = 22,
    D3DRS_ZFUNC = 23,
    D3DRS_ALPHAREF = 24,
    D3DRS_ALPHAFUNC = 25,
    D3DRS_DITHERENABLE = 26,
    D3DRS_FOGSTART = 27,
    D3DRS_FOGEND = 28,
    D3DRS_FOGDENSITY = 29,
    D3DRS_RANGEFOGENABLE = 30,
    D3DRS_STENCILENABLE = 31,
    D3DRS_STENCILFAIL = 32,
    D3DRS_STENCILZFAIL = 33,
    D3DRS_STENCILPASS = 34,
    D3DRS_STENCILFUNC = 35,
    D3DRS_STENCILREF = 36,
    D3DRS_STENCILMASK = 37,
    D3DRS_STENCILWRITEMASK = 38,
    D3DRS_TEXTUREFACTOR = 39,
    D3DRS_WRAP0 = 128,
    D3DRS_WRAP1 = 129,
    D3DRS_WRAP2 = 130,
    D3DRS_WRAP3 = 131,
    D3DRS_WRAP4 = 132,
    D3DRS_WRAP5 = 133,
    D3DRS_WRAP6 = 134,
    D3DRS_WRAP7 = 135,
    D3DRS_LIGHTING = 137,
    D3DRS_AMBIENT = 139,
    D3DRS_FOGENABLE = 142,
    D3DRS_SPECULARENABLE = 143,
    D3DRS_FOGCOLOR = 144,
    D3DRS_FOGTABLEMODE = 145,
    D3DRS_FOGSTART = 146,
    D3DRS_FOGEND = 147,
    D3DRS_FOGDENSITY = 148,
    D3DRS_RANGEFOGENABLE = 149,
    D3DRS_INDEXEDVERTEXBLENDENABLE = 150,
    D3DRS_COLORWRITEENABLE = 151,
    D3DRS_BLENDOP = 152,
    D3DRS_SCISSORTESTENABLE = 155,
    D3DRS_CLEARCOLOR = 156,
    D3DRS_CLEARSTENCIL = 157,
    D3DRS_CLEARDEPTH = 158,
    D3DRS_COLORWRITEENABLE1 = 159,
    D3DRS_COLORWRITEENABLE2 = 160,
    D3DRS_COLORWRITEENABLE3 = 161,
    D3DRS_BLENDFACTOR = 163,
    D3DRS_SRGBWRITEENABLE = 164,
    D3DRS_SEPARATEALPHABLENDENABLE = 165,
} D3DRENDERSTATETYPE;

typedef enum _D3DTRANSFORMSTATETYPE {
    D3DTS_VIEW = 2,
    D3DTS_PROJECTION = 3,
    D3DTS_TEXTURE0 = 16,
    D3DTS_TEXTURE1 = 17,
    D3DTS_TEXTURE2 = 18,
    D3DTS_TEXTURE3 = 19,
} D3DTRANSFORMSTATETYPE;

typedef enum _D3DVERTEXBLEND {
    D3DVBLEND_DISABLE = 0,
    D3DVBLEND_CONSTANTALPHA = 1,
    D3DVBLEND_CURRENTBEST = 2,
    D3DVBLEND_CONSTANT = 3,
    D3DVBLEND_SRCCOLOR = 4,
    D3DVBLEND_INVSRCCOLOR = 5,
    D3DVBLEND_SRCALPHA = 6,
    D3DVBLEND_INVSRCALPHA = 7,
    D3DVBLEND_DESTALPHA = 8,
    D3DVBLEND_INVDESTALPHA = 9,
    D3DVBLEND_DESTCOLOR = 10,
    D3DVBLEND_INVDESTCOLOR = 11,
    D3DVBLEND_SRCALPHASAT = 12,
    D3DVBLEND_BOTHSRCALPHA = 13,
    D3DVBLEND_BOTHINVSRCALPHA = 14,
} D3DVERTEXBLEND;

typedef enum _D3DTEXTURESTAGESTATETYPE {
    D3DTSS_COLOROP = 1,
    D3DTSS_COLORARG1 = 2,
    D3DTSS_COLORARG2 = 3,
    D3DTSS_ALPHAOP = 4,
    D3DTSS_ALPHAARG1 = 5,
    D3DTSS_ALPHAARG2 = 6,
    D3DTSS_BUMPENVMAT00 = 7,
    D3DTSS_BUMPENVMAT01 = 8,
    D3DTSS_BUMPENVMAT10 = 9,
    D3DTSS_BUMPENVMAT11 = 10,
    D3DTSS_TEXCOORDINDEX = 11,
    D3DTSS_BUMPENVLSCALE = 22,
    D3DTSS_BUMPENVLOFFSET = 23,
    D3DTSS_TEXTURETRANSFORMFLAGS = 24,
    D3DTSS_COLORARG0 = 26,
    D3DTSS_ALPHAARG0 = 27,
    D3DTSS_RESULTARG = 28,
} D3DTEXTURESTAGESTATETYPE;

typedef enum _D3DTEXTUREADDRESS {
    D3DTADDRESS_WRAP = 1,
    D3DTADDRESS_MIRROR = 2,
    D3DTADDRESS_CLAMP = 3,
    D3DTADDRESS_BORDER = 4,
    D3DTADDRESS_MIRRORONCE = 5,
} D3DTEXTUREADDRESS;

typedef enum _D3DTEXTUREFILTERTYPE {
    D3DTEXF_NONE = 0,
    D3DTEXF_POINT = 1,
    D3DTEXF_LINEAR = 2,
    D3DTEXF_ANISOTROPIC = 3,
    D3DTEXF_PYRAMIDALQUAD = 4,
    D3DTEXF_GAUSSIANQUAD = 5,
} D3DTEXTUREFILTERTYPE;

typedef enum _D3DCULL {
    D3DCULL_NONE = 1,
    D3DCULL_CW = 2,
    D3DCULL_CCW = 3,
} D3DCULL;

typedef enum _D3DFORMAT {
    D3DFMT_UNKNOWN = 0,
    D3DFMT_R8G8B8 = 20,
    D3DFMT_A8R8G8B8 = 21,
    D3DFMT_X8R8G8B8 = 22,
    D3DFMT_R5G6B5 = 23,
    D3DFMT_X1R5G5B5 = 24,
    D3DFMT_A1R5G5B5 = 25,
    D3DFMT_A4R4G4B4 = 26,
    D3DFMT_R3G3B2 = 27,
    D3DFMT_A8 = 28,
    D3DFMT_A8R3G3B2 = 29,
    D3DFMT_X4R4G4B4 = 30,
    D3DFMT_A2B10G10R10 = 31,
    D3DFMT_A8B8G8R8 = 32,
    D3DFMT_X8B8G8R8 = 33,
    D3DFMT_G16R16 = 34,
    D3DFMT_A2R10G10B10 = 35,
    D3DFMT_A16B16G16R16 = 36,
    D3DFMT_L8 = 50,
    D3DFMT_A8L8 = 51,
    D3DFMT_A4L4 = 52,
    D3DFMT_V8U8 = 60,
    D3DFMT_L6V5U5 = 61,
    D3DFMT_X8L8V8U8 = 62,
    D3DFMT_Q8W8V8U8 = 63,
    D3DFMT_CxV8U8 = 64,
    D3DFMT_L16 = 81,
    D3DFMT_INDEX16 = 101,
    D3DFMT_D16_LOCKABLE = 110,
    D3DFMT_D32 = 113,
    D3DFMT_D15S1 = 115,
    D3DFMT_D24S8 = 117,
    D3DFMT_D24X8 = 118,
    D3DFMT_D24X4S4 = 119,
    D3DFMT_D16 = 121,
    D3DFMT_D32F_LOCKABLE = 113,
    D3DFMT_D24FS8 = 117,
} D3DFORMAT;

typedef enum _D3DMULTISAMPLE_TYPE {
    D3DMULTISAMPLE_NONE = 0,
    D3DMULTISAMPLE_2_SAMPLES = 1,
    D3DMULTISAMPLE_3_SAMPLES = 2,
    D3DMULTISAMPLE_4_SAMPLES = 3,
    D3DMULTISAMPLE_5_SAMPLES = 4,
    D3DMULTISAMPLE_6_SAMPLES = 5,
    D3DMULTISAMPLE_7_SAMPLES = 6,
    D3DMULTISAMPLE_8_SAMPLES = 7,
    D3DMULTISAMPLE_9_SAMPLES = 8,
    D3DMULTISAMPLE_10_SAMPLES = 9,
    D3DMULTISAMPLE_11_SAMPLES = 10,
    D3DMULTISAMPLE_12_SAMPLES = 11,
    D3DMULTISAMPLE_13_SAMPLES = 12,
    D3DMULTISAMPLE_14_SAMPLES = 13,
    D3DMULTISAMPLE_15_SAMPLES = 14,
    D3DMULTISAMPLE_16_SAMPLES = 15,
} D3DMULTISAMPLE_TYPE;

typedef enum _D3DSWAPEFFECT {
    D3DSWAPEFFECT_DISCARD = 1,
    D3DSWAPEFFECT_FLIP = 2,
    D3DSWAPEFFECT_COPY = 3,
} D3DSWAPEFFECT;

typedef enum _D3DDEVTYPE {
    D3DDEVTYPE_HAL = 1,
    D3DDEVTYPE_REF = 2,
    D3DDEVTYPE_SW = 3,
} D3DDEVTYPE;

typedef enum _D3DADAPTER_IDENTIFIER8 {
} D3DADAPTER_IDENTIFIER8;

typedef struct _D3DADAPTER_IDENTIFIER9 D3DADAPTER_IDENTIFIER9;
struct _D3DADAPTER_IDENTIFIER9 {
    char Driver[512];
    char Description[512];
    unsigned long Version;
    unsigned long SubVersion;
    unsigned long Build;
    unsigned long VendorId;
    unsigned long DeviceId;
    unsigned long SubSysId;
    unsigned long Revision;
    GUID DeviceIdentifier;
    unsigned long WHQLLevel;
};

typedef struct _D3DDISPLAYMODE {
    UINT Width;
    UINT Height;
    UINT RefreshRate;
    D3DFORMAT Format;
} D3DDISPLAYMODE;

typedef struct _D3DDEVICE_CREATION_PARAMETERS {
    UINT AdapterOrdinal;
    D3DDEVTYPE DeviceType;
    HANDLE hFocusWindow;
    DWORD BehaviorFlags;
} D3DDEVICE_CREATION_PARAMETERS;

typedef struct _D3DPRESENT_PARAMETERS_ {
    UINT BackBufferWidth;
    UINT BackBufferHeight;
    D3DFORMAT BackBufferFormat;
    UINT BackBufferCount;
    D3DMULTISAMPLE_TYPE MultiSampleType;
    D3DSWAPEFFECT SwapEffect;
    HWND hDeviceWindow;
    BOOL Windowed;
    BOOL EnableAutoDepthStencil;
    D3DFORMAT AutoDepthStencilFormat;
    DWORD Flags;
} D3DPRESENT_PARAMETERS;

typedef struct _D3DVERTEXBUFFER_DESC {
    D3DFORMAT Format;
    D3DVERTEXBUFFER_DESC_TYPE Type;
    UINT Size;
    DWORD Usage;
    D3DPOOL Pool;
    UINT FVF;
} D3DVERTEXBUFFER_DESC;

typedef struct _D3DINDEXBUFFER_DESC {
    D3DFORMAT Format;
    D3DINDEXBUFFER_DESC_TYPE Type;
    UINT Size;
    DWORD Usage;
    D3DPOOL Pool;
} D3DINDEXBUFFER_DESC;

typedef struct _D3DSURFACE_DESC {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Type;
    UINT Height;
    UINT Width;
    D3DMULTISAMPLE_TYPE MultiSampleType;
} D3DSURFACE_DESC;

typedef struct _D3DLOCKED_RECT {
    INT Pitch;
    void* pBits;
} D3DLOCKED_RECT;

typedef struct _D3DLOCKED_BOX {
    INT RowPitch;
    INT SlicePitch;
    void* pBits;
} D3DLOCKED_BOX;

typedef struct _D3DBOX {
    UINT Left;
    UINT Top;
    UINT Right;
    UINT Bottom;
    UINT Front;
    UINT Back;
} D3DBOX;

typedef struct _D3DMATRIX {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DMATRIX;

typedef struct _D3DVIEWPORT8 {
    DWORD X;
    DWORD Y;
    DWORD Width;
    DWORD Height;
    float MinZ;
    float MaxZ;
} D3DVIEWPORT8;

typedef struct _D3DMATERIAL8 {
    D3DCOLORVALUE diffuse;
    D3DCOLORVALUE ambient;
    D3DCOLORVALUE specular;
    D3DCOLORVALUE emissive;
    float Power;
} D3DMATERIAL8;

typedef struct _D3DLIGHT8 {
    D3DLIGHTTYPE Type;
    D3DCOLORVALUE diffuse;
    D3DCOLORVALUE specular;
    D3DCOLORVALUE ambient;
    D3DVECTOR Position;
    D3DVECTOR Direction;
    float Range;
    float Falloff;
    float Attenuation0;
    float Attenuation1;
    float Attenuation2;
    float Theta;
    float Phi;
} D3DLIGHT8;

typedef void* D3DCOLOR;

#undef INTERFACE
#define INTERFACE IDirect3DTexture8

DECLARE_INTERFACE_(IDirect3DTexture8, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice8** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD Priority) PURE;
    STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
    STDMETHOD_(void, PreLoad)(THIS) PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
    STDMETHOD_(DWORD, GetLevelCount)(THIS) PURE;
    STDMETHOD_(DWORD, GetLevelDesc)(THIS_ UINT Level, D3DSURFACE_DESC* pDesc) PURE;
    STDMETHOD(GetSurfaceLevel)(THIS_ UINT Level, IDirect3DSurface8** ppSurfaceLevel) PURE;
    STDMETHOD(LockRect)(THIS_ UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags) PURE;
    STDMETHOD(UnlockRect)(THIS_ UINT Level) PURE;
    STDMETHOD(D3D8LoadTexture)(THIS_ IDirect3DTexture8* pSourceTexture, DWORD Filter) PURE;
};

typedef IDirect3DTexture8* LPDIRECT3DTEXTURE8;

#undef INTERFACE
#define INTERFACE IDirect3DSurface8

DECLARE_INTERFACE_(IDirect3DSurface8, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice8** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD Priority) PURE;
    STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
    STDMETHOD_(void, PreLoad)(THIS) PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
    STDMETHOD(GetContainer)(THIS_ REFIID riid, void** ppContainer) PURE;
    STDMETHOD(GetDesc)(THIS_ D3DSURFACE_DESC* pDesc) PURE;
    STDMETHOD(LockRect)(THIS_ D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags) PURE;
    STDMETHOD(UnlockRect)(THIS) PURE;
};

typedef IDirect3DSurface8* LPDIRECT3DSURFACE8;

#undef INTERFACE
#define INTERFACE IDirect3DVertexBuffer8

DECLARE_INTERFACE_(IDirect3DVertexBuffer8, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice8** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD Priority) PURE;
    STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
    STDMETHOD_(void, PreLoad)(THIS) PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
    STDMETHOD_(DWORD, GetLevelCount)(THIS) PURE;
    STDMETHOD(Lock)(THIS_ DWORD OffsetToLock, DWORD SizeToLock, BYTE** ppbData, DWORD Flags) PURE;
    STDMETHOD(Unlock)(THIS) PURE;
    STDMETHOD(GetDesc)(THIS_ D3DVERTEXBUFFER_DESC* pDesc) PURE;
};

typedef IDirect3DVertexBuffer8* LPDIRECT3DVERTEXBUFFER8;

#undef INTERFACE
#define INTERFACE IDirect3DIndexBuffer8

DECLARE_INTERFACE_(IDirect3DIndexBuffer8, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice8** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD Priority) PURE;
    STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
    STDMETHOD_(void, PreLoad)(THIS) PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
    STDMETHOD(Lock)(THIS_ DWORD OffsetToLock, DWORD SizeToLock, BYTE** ppbData, DWORD Flags) PURE;
    STDMETHOD(Unlock)(THIS) PURE;
    STDMETHOD(GetDesc)(THIS_ D3DINDEXBUFFER_DESC* pDesc) PURE;
};

typedef IDirect3DIndexBuffer8* LPDIRECT3DINDEXBUFFER8;

#undef INTERFACE
#define INTERFACE IDirect3DDevice8

DECLARE_INTERFACE_(IDirect3DDevice8, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    STDMETHOD(TestCooperativeLevel)(THIS) PURE;
    STDMETHOD_(UINT, GetAvailableTextureMem)(THIS) PURE;
    STDMETHOD(ResourceManagerDiscardBytes)(THIS_ DWORD Bytes) PURE;
    STDMETHOD(GetDirect3D)(THIS_ IDirect3D8** ppD3D9) PURE;
    STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS8* pCaps) PURE;
    STDMETHOD(GetDisplayMode)(THIS_ D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS* pParameters) PURE;
    STDMETHOD(CreateTexture)(THIS_ UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture) PURE;
    STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppVolumeTexture) PURE;
    STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppCubeTexture) PURE;
    STDMETHOD(CreateSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface) PURE;
    STDMETHOD(CreateRenderTarget)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppRenderTarget) PURE;
    STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppDepthStencil) PURE;
    STDMETHOD(CreateImageSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface) PURE;
    STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer) PURE;
    STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer) PURE;
    STDMETHOD(SetRenderTarget)(THIS_ IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil) PURE;
    STDMETHOD(GetRenderTarget)(THIS_ IDirect3DSurface8** ppRenderTarget) PURE;
    STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface8** ppZStencilSurface) PURE;
    STDMETHOD(BeginScene)(THIS) PURE;
    STDMETHOD(EndScene)(THIS) PURE;
    STDMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) PURE;
    STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT8* pViewport) PURE;
    STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT8* pViewport) PURE;
    STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL8* pMaterial) PURE;
    STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL8* pMaterial) PURE;
    STDMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT8* pLight) PURE;
    STDMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable) PURE;
    STDMETHOD(GetLightEnable)(THIS_ DWORD Index, BOOL* pEnable) PURE;
    STDMETHOD(SetClipPlane)(THIS_ DWORD Index, CONST float* pPlane) PURE;
    STDMETHOD(GetClipPlane)(THIS_ DWORD Index, float* pPlane) PURE;
    STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD Value) PURE;
    STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD* pValue) PURE;
    STDMETHOD(SetTexture)(THIS_ DWORD Stage, IDirect3DTexture8* pTexture) PURE;
    STDMETHOD(GetTexture)(THIS_ DWORD Stage, IDirect3DTexture8** ppTexture) PURE;
    STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) PURE;
    STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGETYPE Type, DWORD Value) PURE;
    STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect) PURE;
    STDMETHOD(GetScissorRect)(THIS_ RECT* pRect) PURE;
    STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL Software) PURE;
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS) PURE;
    STDMETHOD(SetNPatchMode)(THIS_ float Segments) PURE;
    STDMETHOD_(float, GetNPatchMode)(THIS) PURE;
    STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) PURE;
    STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount) PURE;
    STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) PURE;
    STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) PURE;
    STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags) PURE;
    STDMETHOD(SetVertexShader)(THIS_ DWORD Handle) PURE;
    STDMETHOD_(DWORD, GetVertexShader)(THIS) PURE;
    STDMETHOD(SetVertexShaderConstant)(THIS_ DWORD Register, CONST void* pConstantData, DWORD ConstantCount) PURE;
    STDMETHOD(GetVertexShaderConstant)(THIS_ DWORD Register, void* pConstantData, DWORD ConstantCount) PURE;
    STDMETHOD(DeleteVertexShader)(THIS_ DWORD Handle) PURE;
    STDMETHOD(SetPixelShader)(THIS_ DWORD Handle) PURE;
    STDMETHOD_(DWORD, GetPixelShader)(THIS) PURE;
    STDMETHOD(SetPixelShaderConstant)(THIS_ DWORD Register, CONST void* pConstantData, DWORD ConstantCount) PURE;
    STDMETHOD(GetPixelShaderConstant)(THIS_ DWORD Register, void* pConstantData, DWORD ConstantCount) PURE;
    STDMETHOD(DeletePixelShader)(THIS_ DWORD Handle) PURE;
    STDMETHOD(SetRenderTarget)(THIS_ IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil) PURE;
    STDMETHOD(GetGammaRamp)(THIS_ D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD(SetGammaRamp)(THIS_ DWORD Flags, CONST D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD(UpdateTexture)(THIS_ IDirect3DTexture8* pSourceTexture, IDirect3DTexture8* pDestinationTexture) PURE;
    STDMETHOD(GetFrontBuffer)(THIS_ IDirect3DSurface8* pDestSurface) PURE;
    STDMETHOD(GetBackBuffer)(THIS_ UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer) PURE;
};

typedef IDirect3DDevice8* LPDIRECT3DDEVICE8;

typedef struct _D3DCAPS8 {
    DWORD Caps;
    DWORD Caps2;
    DWORD Caps3;
    DWORD Caps4;
    DWORD PresentationIntervals;
    DWORD CursorCaps;
    DWORD Reserved2;
    DWORD TextureCaps;
    DWORD TextureFilterCaps;
    DWORD VolumeTextureCaps;
    DWORD CubeTextureCaps;
    DWORD TextureAddressCaps;
    DWORD VolumeTextureAddressCaps;
    DWORD LineCaps;
    DWORD MaxTextureWidth;
    DWORD MaxTextureHeight;
    DWORD MaxVolumeExtent;
    DWORD MaxTextureRepeat;
    DWORD MaxTextureAspectRatio;
    DWORD MaxAnisotropy;
    float MaxVertexW;
    float GuardBandLeft;
    float GuardBandRight;
    float GuardBandTop;
    float GuardBandBottom;
    float ExtentsAdjust;
    DWORD StencilCaps;
    DWORD FVFCaps;
    DWORD TextureOpCaps;
    DWORD MaxTextureBlendStages;
    DWORD MaxSimultaneousTextures;
    DWORD VertexProcessingCaps;
    DWORD MaxActiveLights;
    DWORD MaxUserClipPlanes;
    DWORD MaxVertexBlendMatrices;
    DWORD MaxVertexBlendMatrixIndex;
    float MaxPointSize;
    DWORD MaxPrimitiveCount;
    DWORD MaxVertexIndex;
    DWORD MaxStreams;
    DWORD MaxStreamStride;
    DWORD VertexShaderVersion;
    DWORD MaxVertexShaderConst;
    DWORD PixelShaderVersion;
    float MaxPixelShaderValue;
} D3DCAPS8;

typedef D3DCAPS8* LPD3DCAPS8;

#undef INTERFACE
#define INTERFACE IDirect3D8

DECLARE_INTERFACE_(IDirect3D8, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction) PURE;
    STDMETHOD_(UINT, GetAdapterCount)(THIS) PURE;
    STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) PURE;
    STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter) PURE;
    STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter, D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) PURE;
    STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) PURE;
    STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType) PURE;
    STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) PURE;
    STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8* pCaps) PURE;
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter) PURE;
    STDMETHOD(CreateDevice)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface) PURE;
};

typedef IDirect3D8* LPDIRECT3D8;

IDirect3D8* CreateD3D8Wrapper(IDirect3D9* pD3D9, UINT SDKVersion);

#endif // D3D8TO9_WRAPPER_HPP
