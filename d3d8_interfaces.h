#pragma once

#include <windows.h>
#include <math.h>

// Basic COM types
typedef interface IUnknown IUnknown;
typedef GUID IID;
typedef REFIID REFIID;
typedef DWORD DWORD;
typedef UINT UINT;
typedef BOOL BOOL;
typedef float FLOAT;
typedef double DOUBLE;
typedef LONG LONG;
typedef ULONG ULONG;
typedef HWND HWND;
typedef HMONITOR HMONITOR;
typedef HANDLE HANDLE;
typedef struct _RECT RECT;
typedef struct tagPOINT POINT;
typedef void* LPVOID;
typedef BYTE BYTE;
typedef WORD WORD;

// D3D8 types (manually defined to avoid SDK dependency)
typedef enum {
    D3DDEVTYPE_HAL = 1,
    D3DDEVTYPE_REF = 2,
    D3DDEVTYPE_SW = 3,
    D3DDEVTYPE_NULLREF = 4
} D3DDEVTYPE;

typedef enum {
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
    D3DFMT_A2R10G10B10 = 35,
    D3DFMT_A16B16G16R16 = 36,
    D3DFMT_L8 = 50,
    D3DFMT_A8L8 = 51,
    D3DFMT_A4L4 = 52,
    D3DFMT_S8_LOCKABLE = 70,
    D3DFMT_D16_LOCKABLE = 71,
    D3DFMT_D24S8 = 75,
    D3DFMT_D24X8 = 77,
    D3DFMT_D32 = 81,
    D3DFMT_D15S1 = 73,
    D3DFMT_D16 = 80,
    D3DFMT_VERTEXDATA = 100,
    D3DFMT_INDEX16 = 101,
    D3DFMT_INDEX32 = 102
} D3DFORMAT;

typedef enum {
    D3DRESOURCETYPE_SURFACE = 1,
    D3DRESOURCETYPE_VOLUME = 2,
    D3DRESOURCETYPE_TEXTURE = 3,
    D3DRESOURCETYPE_VOLUMETEXTURE = 4,
    D3DRESOURCETYPE_CUBETEXTURE = 5,
    D3DRESOURCETYPE_VERTEXBUFFER = 6,
    D3DRESOURCETYPE_INDEXBUFFER = 7
} D3DRESOURCETYPE;

typedef enum {
    D3DPMISCCAPS_MASKZ = 0x00000001L,
    D3DPMISCCAPS_LINEPATTERNOFF = 0x00000002L,
    D3DPMISCCAPS_CULLNONE = 0x00000004L,
    D3DPMISCCAPS_CULLCW = 0x00000008L,
    D3DPMISCCAPS_CULLCCW = 0x00000010L,
    D3DPMISCCAPS_COLORWRITEENABLE = 0x00000020L,
    D3DPMISCCAPS_CLIPPLANES = 0x00000040L
} D3DPMISCCAPS;

typedef enum {
    D3DPRIMITIVETYPE_POINTLIST = 1,
    D3DPRIMITIVETYPE_LINELIST = 2,
    D3DPRIMITIVETYPE_LINESTRIP = 3,
    D3DPRIMITIVETYPE_TRIANGLELIST = 4,
    D3DPRIMITIVETYPE_TRIANGLESTRIP = 5,
    D3DPRIMITIVETYPE_TRIANGLEFAN = 6
} D3DPRIMITIVETYPE;

typedef enum {
    D3DTRANSFORMSTATETYPE_WORLD = 0,
    D3DTRANSFORMSTATETYPE_VIEW = 1,
    D3DTRANSFORMSTATETYPE_PROJECTION = 2,
    D3DTRANSFORMSTATETYPE_TEXTURE0 = 16,
    D3DTRANSFORMSTATETYPE_TEXTURE1 = 17,
    D3DTRANSFORMSTATETYPE_TEXTURE2 = 18,
    D3DTRANSFORMSTATETYPE_TEXTURE3 = 19
} D3DTRANSFORMSTATETYPE;

typedef enum {
    D3DRENDERSTATETYPE_ZENABLE = 1,
    D3DRENDERSTATETYPE_FILLMODE = 3,
    D3DRENDERSTATETYPE_SHADEMODE = 5,
    D3DRENDERSTATETYPE_LINEPATTERN = 10,
    D3DRENDERSTATETYPE_CULLMODE = 22,
    D3DRENDERSTATETYPE_FOGENABLE = 24,
    D3DRENDERSTATETYPE_FOGCOLOR = 26,
    D3DRENDERSTATETYPE_FOGSTART = 27,
    D3DRENDERSTATETYPE_FOGEND = 28,
    D3DRENDERSTATETYPE_FOGDENSITY = 29,
    D3DRENDERSTATETYPE_RANGEFOGENABLE = 48,
    D3DRENDERSTATETYPE_STENCILENABLE = 52,
    D3DRENDERSTATETYPE_STENCILFAIL = 53,
    D3DRENDERSTATETYPE_STENCILZFAIL = 54,
    D3DRENDERSTATETYPE_STENCILZPASS = 55,
    D3DRENDERSTATETYPE_STENCILREF = 56,
    D3DRENDERSTATETYPE_STENCILMASK = 57,
    D3DRENDERSTATETYPE_STENCILWRITEMASK = 58,
    D3DRENDERSTATETYPE_TEXTUREFACTOR = 60,
    D3DRENDERSTATETYPE_AMBIENT = 68,
    D3DRENDERSTATETYPE_COLORWRITEENABLE = 63,
    D3DRENDERSTATETYPE_BLENDENABLE = 27,
    D3DRENDERSTATETYPE_SRCBLEND = 19,
    D3DRENDERSTATETYPE_DESTBLEND = 20,
    D3DRENDERSTATETYPE_BLENDOP = 27,
    D3DRENDERSTATETYPE_ALPHABLENDENABLE = 65,
    D3DRENDERSTATETYPE_ALPHATESTENABLE = 67,
    D3DRENDERSTATETYPE_ALPHAREF = 69,
    D3DRENDERSTATETYPE_ALPHAFUNC = 70,
    D3DRENDERSTATETYPE_SEPARATEALPHABLENDENABLE = 206,
} D3DRENDERSTATETYPE;

typedef enum {
    D3DTEXTURESTAGESTATETYPE_COLOROP = 1,
    D3DTEXTURESTAGESTATETYPE_COLORARG1 = 2,
    D3DTEXTURESTAGESTATETYPE_COLORARG2 = 3,
    D3DTEXTURESTAGESTATETYPE_ALPHAOP = 4,
    D3DTEXTURESTAGESTATETYPE_ALPHAARG1 = 5,
    D3DTEXTURESTAGESTATETYPE_ALPHAARG2 = 6,
    D3DTEXTURESTAGESTATETYPE_BUMPENVMAT00 = 7,
    D3DTEXTURESTAGESTATETYPE_BUMPENVMAT01 = 8,
    D3DTEXTURESTAGESTATETYPE_BUMPENVMAT10 = 9,
    D3DTEXTURESTAGESTATETYPE_BUMPENVMAT11 = 10,
    D3DTEXTURESTAGESTATETYPE_TEXCOORDINDEX = 11,
    D3DTEXTURESTAGESTATETYPE_BUMPENVLSCALE = 22,
    D3DTEXTURESTAGESTATETYPE_BUMPENVLOFFSET = 23,
    D3DTEXTURESTAGESTATETYPE_TEXTURETRANSFORMFLAGS = 24,
    D3DTEXTURESTAGESTATETYPE_COLORARG0 = 26,
    D3DTEXTURESTAGESTATETYPE_ALPHAARG0 = 27,
    D3DTEXTURESTAGESTATETYPE_RESULTARG = 28
} D3DTEXTURESTAGESTATETYPE;

typedef enum {
    D3DSAMPLERSTATETYPE_MINFILTER = 0,
    D3DSAMPLERSTATETYPE_MAGFILTER = 1,
    D3DSAMPLERSTATETYPE_MIPFILTER = 2,
    D3DSAMPLERSTATETYPE_ADDRESSU = 3,
    D3DSAMPLERSTATETYPE_ADDRESSV = 4,
    D3DSAMPLERSTATETYPE_ADDRESSW = 5,
    D3DSAMPLERSTATETYPE_BORDERCOLOR = 6,
    D3DSAMPLERSTATETYPE_MAXMIPLEVEL = 8,
    D3DSAMPLERSTATETYPE_MAXANISOTROPY = 9
} D3DSAMPLERSTATETYPE;

typedef enum {
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
    D3DMULTISAMPLE_16_SAMPLES = 15
} D3DMULTISAMPLE_TYPE;

typedef enum {
    D3DPOOL_DEFAULT = 0,
    D3DPOOL_MANAGED = 1,
    D3DPOOL_SYSTEMMEM = 2,
    D3DPOOL_SCRATCH = 3
} D3DPOOL;

typedef enum {
    D3DBACKBUFFER_TYPE_MONO = 0,
    D3DBACKBUFFER_TYPE_EMULATED = 1
} D3DBACKBUFFER_TYPE;

typedef enum {
    D3DSTATEBLOCKTYPE_ALL = 0,
    D3DSTATEBLOCKTYPE_VERTEXSHADER = 1,
    D3DSTATEBLOCKTYPE_PIXELSHADER = 2
} D3DSTATEBLOCKTYPE;

typedef enum {
    D3DQUERYTYPE_VCACHE = 4,
    D3DQUERYTYPE_RESOURCEMANAGER = 5,
    D3DQUERYTYPE_VERTEXSTATS = 6,
    D3DQUERYTYPE_EVENT = 8,
    D3DQUERYTYPE_OCCLUSION = 9
} D3DQUERYTYPE;

typedef struct _D3DVIEWPORT8 {
    DWORD X;
    DWORD Y;
    DWORD Width;
    DWORD Height;
    DWORD MinZ;
    DWORD MaxZ;
} D3DVIEWPORT8;

typedef struct _D3DVIEWPORT9 {
    DWORD X;
    DWORD Y;
    DWORD Width;
    DWORD Height;
    float MinZ;
    float MaxZ;
} D3DVIEWPORT9;

typedef struct _D3DDISPLAYMODE {
    UINT Width;
    UINT Height;
    UINT RefreshRate;
    D3DFORMAT Format;
} D3DDISPLAYMODE;

typedef struct _D3DRECT {
    LONG x1;
    LONG y1;
    LONG x2;
    LONG y2;
} D3DRECT;

typedef struct _D3DGAMMARAMP {
    WORD red[256];
    WORD green[256];
    WORD blue[256];
} D3DGAMMARAMP;

typedef DWORD D3DCOLOR;

typedef enum {
    D3DLIGHTTYPE_POINT = 1,
    D3DLIGHTTYPE_SPOT = 2,
    D3DLIGHTTYPE_DIRECTIONAL = 3
} D3DLIGHTTYPE;

typedef struct _D3DLIGHT8 {
    D3DLIGHTTYPE Type;
    D3DCOLOR Color;
    FLOAT PositionX, PositionY, PositionZ;
    FLOAT DirectionX, DirectionY, DirectionZ;
    FLOAT Range;
    FLOAT Falloff;
    FLOAT Attenuation0;
    FLOAT Attenuation1;
    FLOAT Attenuation2;
    FLOAT Theta;
    FLOAT Phi;
} D3DLIGHT8;

typedef struct _D3DMATERIAL8 {
    D3DCOLOR Diffuse;
    D3DCOLOR Ambient;
    D3DCOLOR Specular;
    D3DCOLOR Emissive;
    FLOAT Power;
} D3DMATERIAL8;

typedef struct _D3DCLIPSTATUS8 {
    DWORD ClipUnion;
    DWORD ClipIntersection;
} D3DCLIPSTATUS8;

typedef struct _D3DDEVICE_CREATION_PARAMETERS {
    UINT AdapterOrdinal;
    D3DDEVTYPE DeviceType;
    HWND hFocusWindow;
    DWORD BehaviorFlags;
} D3DDEVICE_CREATION_PARAMETERS;

typedef struct _D3DRASTER_STATUS {
    BOOL InVBlank;
    UINT ScanLine;
} D3DRASTER_STATUS;

typedef enum {
    D3DBASIS_BEZIER = 0,
    D3DBASIS_BSPLINE = 1,
    D3DBASIS_CATMULLROM = 2
} D3DBASISTYPE;

typedef enum {
    D3DDEGREE_LINEAR = 1,
    D3DDEGREE_QUADRATIC = 2,
    D3DDEGREE_CUBIC = 3,
    D3DDEGREE_QUINTIC = 5
} D3DDEGREE;

typedef struct _D3DRECTPATCH_INFO {
    UINT Version;
    UINT NumSegsX;
    UINT NumSegsY;
    FLOAT PatchWidth;
    FLOAT PatchHeight;
    D3DBASISTYPE BasisType;
    D3DDEGREE Degree;
    BOOL Loop;
} D3DRECTPATCH_INFO;

typedef struct _D3DTRIPATCH_INFO {
    UINT Version;
    UINT NumSegsX;
    UINT NumSegsY;
    D3DBASISTYPE BasisType;
    D3DDEGREE Degree;
    BOOL Loop;
} D3DTRIPATCH_INFO;

// D3DVERTEXELEMENT9 (minimal)

typedef struct _D3DVERTEXELEMENT9 {
    WORD Stream;
    WORD Offset;
    BYTE Type;
    BYTE Method;
    BYTE Usage;
    BYTE UsageIndex;
} D3DVERTEXELEMENT9;

// PALETTEENTRY
typedef struct _PALETTEENTRY {
    BYTE peRed;
    BYTE peGreen;
    BYTE peBlue;
    BYTE peFlags;
} PALETTEENTRY, *PPALETTEENTRY;

// D3DCOLOR (ABGR packed)
typedef DWORD D3DCOLOR;

// D3DADAPTER_IDENTIFIER8 (minimal)
typedef struct _D3DADAPTER_IDENTIFIER8 {
    char Driver[512];
    char Description[512];
    DWORD DriverVersionLowPart;
    DWORD DriverVersionHighPart;
    DWORD VendorId;
    DWORD DeviceId;
    DWORD SubSysId;
    DWORD Revision;
    GUID DeviceIdentifier;
    DWORD WHQLLevel;
} D3DADAPTER_IDENTIFIER8;

// D3DCAPS8 (minimal)
typedef struct _D3DCAPS8 {
    DWORD DeviceType;
    UINT AdapterOrdinal;
    DWORD Caps;
    DWORD Caps2;
    DWORD Caps3;
    DWORD Caps4;
    DWORD PresentationIntervals;
    DWORD CursorCaps;
    DWORD DevCaps;
    DWORD MiscCaps;
    DWORD RasterCaps;
    DWORD ZCmpCaps;
    DWORD SrcBlendCaps;
    DWORD DestBlendCaps;
    DWORD AlphaCmpCaps;
    DWORD ShadeCaps;
    DWORD TextureCaps;
    DWORD TextureFilterCaps;
    DWORD CubeTextureFilterCaps;
    DWORD VolumeTextureFilterCaps;
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
    DWORD MaxUserClipPlanes;
    DWORD MaxActiveLightCount;
    DWORD MaxTextureBlendStages;
    DWORD MaxTextureAddressStages;
    DWORD MaxNpatchTessellationLevel;
    DWORD Reserved5;
    DWORD OcclusionQuerySupportMask;
    DWORD Reserved6;
    char Reserved7[4];
    float MaxPointSpriteSize;
    DWORD Reserved8;
    DWORD Reserved9;
    DWORD NumSimultaneousRTs;
    DWORD StretchRectFilterCaps;
    DWORD VS20Caps;
    DWORD PS20Caps;
    DWORD VertexTextureFilterCaps;
    DWORD Reserved10[2];
    DWORD MaxVShaderInstructionsExecuted;
    DWORD MaxPShaderInstructionsExecuted;
    DWORD MaxVertexShader30InstructionSlots;
    DWORD MaxPixelShader30InstructionSlots;
} D3DCAPS8;

// D3DCAPS9 (for compatibility)
typedef D3DCAPS8 D3DCAPS9;

// D3DMATRIX
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

// D3DPRESENT_PARAMETERS8
typedef struct _D3DPRESENT_PARAMETERS8 {
    UINT BackBufferWidth;
    UINT BackBufferHeight;
    D3DFORMAT BackBufferFormat;
    UINT BackBufferCount;
    D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD SwapEffect;
    HWND hDeviceWindow;
    BOOL Windowed;
    BOOL EnableAutoDepthStencil;
    D3DFORMAT AutoDepthStencilFormat;
    DWORD Flags;
    UINT FullScreen_RefreshRateInHz;
    UINT FullScreen_PresentationInterval;
} D3DPRESENT_PARAMETERS8;

// D3DPRESENT_PARAMETERS (for compatibility)
typedef D3DPRESENT_PARAMETERS8 D3DPRESENT_PARAMETERS;

// Forward declarations
interface IDirect3D8;
interface IDirect3DDevice8;
interface IDirect3DSurface8;
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

// IDirect3D8 interface
interface IDirect3D8 : public IUnknown {
    STDMETHOD(RegisterSoftwareDevice)(void* pInitializeFunction) PURE;
    STDMETHOD_(UINT, GetAdapterCount)() PURE;
    STDMETHOD(GetAdapterIdentifier)(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8* pIdentifier) PURE;
    STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter) PURE;
    STDMETHOD(EnumAdapterModes)(UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetAdapterDisplayMode)(UINT Adapter, D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(CheckDeviceType)(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) PURE;
    STDMETHOD(CheckDeviceFormat)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) PURE;
    STDMETHOD(CheckDeviceMultiSampleType)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType) PURE;
    STDMETHOD(CheckDepthStencilMatch)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) PURE;
    STDMETHOD(GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8* pCaps) PURE;
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter) PURE;
    STDMETHOD(CreateDevice)(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS8* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface) PURE;
};

// IDirect3DDevice8 interface
interface IDirect3DDevice8 : public IUnknown {
    STDMETHOD(TestCooperativeLevel)() PURE;
    STDMETHOD_(UINT, GetAvailableTextureMem)() PURE;
    STDMETHOD(ResourceManagerDiscardBytes)(DWORD Bytes) PURE;
    STDMETHOD(GetDirect3D)(IDirect3D8** ppD3D8) PURE;
    STDMETHOD(GetDeviceCaps)(D3DCAPS8* pCaps) PURE;
    STDMETHOD(GetDisplayMode)(D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetCreationParameters)(D3DDEVICE_CREATION_PARAMETERS* pParameters) PURE;
    STDMETHOD(SetCursorProperties)(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap) PURE;
    STDMETHOD_(void, SetCursorPosition)(int X, int Y, DWORD Flags) PURE;
    STDMETHOD_(BOOL, ShowCursor)(BOOL bShow) PURE;
    STDMETHOD(CreateAdditionalSwapChain)(D3DPRESENT_PARAMETERS8* pPresentationParameters, IDirect3DSwapChain8** pSwapChain) PURE;
    STDMETHOD(GetSwapChain)(UINT iSwapChain, IDirect3DSwapChain8** pSwapChain) PURE;
    STDMETHOD_(UINT, GetNumberOfSwapChains)() PURE;
    STDMETHOD(Reset)(D3DPRESENT_PARAMETERS8* pPresentationParameters) PURE;
    STDMETHOD(Present)(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) PURE;
    STDMETHOD(GetBackBuffer)(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer) PURE;
    STDMETHOD(GetRasterStatus)(D3DRASTER_STATUS* pRasterStatus) PURE;
    STDMETHOD_(void, SetGammaRamp)(DWORD Flags, CONST D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD_(void, GetGammaRamp)(D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD(CreateTexture)(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture) PURE;
    STDMETHOD(CreateVolumeTexture)(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8** ppVolumeTexture) PURE;
    STDMETHOD(CreateCubeTexture)(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8** ppCubeTexture) PURE;
    STDMETHOD(CreateVertexBuffer)(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer) PURE;
    STDMETHOD(CreateIndexBuffer)(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer) PURE;
    STDMETHOD(CreateRenderTarget)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppSurface) PURE;
    STDMETHOD(CreateDepthStencilSurface)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface) PURE;
    STDMETHOD(CreateImageSurface)(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface) PURE;
    STDMETHOD(CopyRects)(IDirect3DSurface8* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface8* pDestinationSurface, CONST POINT* pDestPointsArray) PURE;
    STDMETHOD(UpdateTexture)(IDirect3DBaseTexture8* pSourceTexture, IDirect3DBaseTexture8* pDestinationTexture) PURE;
    STDMETHOD(GetFrontBuffer)(IDirect3DSurface8* pDestSurface) PURE;
    STDMETHOD(SetRenderTarget)(IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil) PURE;
    STDMETHOD(GetRenderTarget)(IDirect3DSurface8** ppRenderTarget) PURE;
    STDMETHOD(GetDepthStencilSurface)(IDirect3DSurface8** ppZStencilSurface) PURE;
    STDMETHOD(BeginScene)() PURE;
    STDMETHOD(EndScene)() PURE;
    STDMETHOD(Clear)(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) PURE;
    STDMETHOD(SetTransform)(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) PURE;
    STDMETHOD(GetTransform)(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) PURE;
    STDMETHOD(MultiplyTransform)(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) PURE;
    STDMETHOD(SetViewport)(CONST D3DVIEWPORT8* pViewport) PURE;
    STDMETHOD(GetViewport)(D3DVIEWPORT8* pViewport) PURE;
    STDMETHOD(SetMaterial)(CONST D3DMATERIAL8* pMaterial) PURE;
    STDMETHOD(GetMaterial)(D3DMATERIAL8* pMaterial) PURE;
    STDMETHOD(SetLight)(DWORD Index, CONST D3DLIGHT8* pLight) PURE;
    STDMETHOD(GetLight)(DWORD Index, D3DLIGHT8* pLight) PURE;
    STDMETHOD(LightEnable)(DWORD Index, BOOL Enable) PURE;
    STDMETHOD(GetLightEnable)(DWORD Index, BOOL* pEnable) PURE;
    STDMETHOD(SetClipPlane)(DWORD Index, CONST float* pPlane) PURE;
    STDMETHOD(GetClipPlane)(DWORD Index, float* pPlane) PURE;
    STDMETHOD(SetRenderState)(D3DRENDERSTATETYPE State, DWORD Value) PURE;
    STDMETHOD(GetRenderState)(D3DRENDERSTATETYPE State, DWORD* pValue) PURE;
    STDMETHOD(BeginStateBlock)() PURE;
    STDMETHOD(EndStateBlock)(DWORD* pToken) PURE;
    STDMETHOD(ApplyStateBlock)(DWORD Token) PURE;
    STDMETHOD(CaptureStateBlock)(DWORD Token) PURE;
    STDMETHOD(DeleteStateBlock)(DWORD Token) PURE;
    STDMETHOD(CreateStateBlock)(D3DSTATEBLOCKTYPE Type, DWORD* pToken) PURE;
    STDMETHOD(SetClipStatus)(CONST D3DCLIPSTATUS8* pClipStatus) PURE;
    STDMETHOD(GetClipStatus)(D3DCLIPSTATUS8* pClipStatus) PURE;
    STDMETHOD(GetTexture)(DWORD Stage, IDirect3DBaseTexture8** ppTexture) PURE;
    STDMETHOD(SetTexture)(DWORD Stage, IDirect3DBaseTexture8* pTexture) PURE;
    STDMETHOD(GetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) PURE;
    STDMETHOD(SetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) PURE;
    STDMETHOD(GetSamplerState)(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) PURE;
    STDMETHOD(SetSamplerState)(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) PURE;
    STDMETHOD(ValidateDevice)(DWORD* pNumPasses) PURE;
    STDMETHOD(GetInfo)(DWORD DevInfoID, void* pDevInfoStruct, DWORD DevInfoStructSize) PURE;
    STDMETHOD(SetPaletteEntries)(UINT PaletteNumber, CONST PALETTEENTRY* pEntries) PURE;
    STDMETHOD(GetPaletteEntries)(UINT PaletteNumber, PALETTEENTRY* pEntries) PURE;
    STDMETHOD(SetCurrentTexturePalette)(UINT PaletteNumber) PURE;
    STDMETHOD(GetCurrentTexturePalette)(UINT* PaletteNumber) PURE;
    STDMETHOD(SetScissorRect)(CONST RECT* pRect) PURE;
    STDMETHOD(GetScissorRect)(RECT* pRect) PURE;
    STDMETHOD(SetSoftwareVertexProcessing)(BOOL bSoftware) PURE;
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)() PURE;
    STDMETHOD(SetNPatchMode)(float nSegments) PURE;
    STDMETHOD_(float, GetNPatchMode)() PURE;
    STDMETHOD(DrawPrimitive)(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) PURE;
    STDMETHOD(DrawIndexedPrimitive)(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount) PURE;
    STDMETHOD(DrawPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) PURE;
    STDMETHOD(DrawIndexedPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) PURE;
    STDMETHOD(ProcessVertices)(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags) PURE;
    STDMETHOD(CreateVertexShader)(CONST DWORD* pDeclaration, CONST DWORD* pFunction, DWORD* pHandle, DWORD Usage) PURE;
    STDMETHOD(SetVertexShader)(DWORD Handle) PURE;
    STDMETHOD(GetVertexShader)(DWORD* pHandle) PURE;
    STDMETHOD(DeleteVertexShader)(DWORD Handle) PURE;
    STDMETHOD(SetVertexShaderConstant)(UINT Register, CONST void* pConstantData, UINT ConstantCount) PURE;
    STDMETHOD(GetVertexShaderConstant)(UINT Register, void* pConstantData, UINT ConstantCount) PURE;
    STDMETHOD(GetVertexShaderDeclaration)(DWORD Handle, void* pData, UINT* pSizeOfData) PURE;
    STDMETHOD(GetVertexShaderFunction)(DWORD Handle, void* pData, UINT* pSizeOfData) PURE;
    STDMETHOD(SetStreamSource)(UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData, UINT Stride) PURE;
    STDMETHOD(GetStreamSource)(UINT StreamNumber, IDirect3DVertexBuffer8** ppStreamData, UINT* pStride) PURE;
    STDMETHOD(SetIndices)(IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex) PURE;
    STDMETHOD(GetIndices)(IDirect3DIndexBuffer8** ppIndexData, UINT* pBaseVertexIndex) PURE;
    STDMETHOD(CreatePixelShader)(CONST DWORD* pFunction, DWORD* pHandle) PURE;
    STDMETHOD(SetPixelShader)(DWORD Handle) PURE;
    STDMETHOD(GetPixelShader)(DWORD* pHandle) PURE;
    STDMETHOD(DeletePixelShader)(DWORD Handle) PURE;
    STDMETHOD(SetPixelShaderConstant)(UINT Register, CONST void* pConstantData, UINT ConstantCount) PURE;
    STDMETHOD(GetPixelShaderConstant)(UINT Register, void* pConstantData, UINT ConstantCount) PURE;
    STDMETHOD(GetPixelShaderFunction)(DWORD Handle, void* pData, UINT* pSizeOfData) PURE;
    STDMETHOD(DrawRectPatch)(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) PURE;
    STDMETHOD(DrawTriPatch)(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) PURE;
    STDMETHOD(DeletePatch)(UINT Handle) PURE;
    STDMETHOD(SetFVF)(DWORD FVF) PURE;
    STDMETHOD(GetFVF)(DWORD* pFVF) PURE;
};

// Stub interfaces (minimal implementations for compilation)
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

// IID definitions
#ifdef INITGUID
DEFINE_GUID(IID_IDirect3D8, 0x1DD9E8DA, 0x6C27, 0x41C8, 0xA7, 0xE7, 0x76, 0x15, 0xEE, 0xF7, 0x5C, 0x55);
DEFINE_GUID(IID_IDirect3DDevice8, 0x1B9CCB17, 0xE46D, 0x4375, 0xA1, 0xA9, 0x7C, 0xDF, 0xA4, 0xE8, 0x50, 0x38);
#else
extern const IID IID_IDirect3D8;
extern const IID IID_IDirect3DDevice8;
#endif
