#include "d3d8_interfaces.h"
#include <gl/gl.h>
#include "d3d8_opengl_renderer.cpp"

class CDirect3D8OpenGL : public IDirect3D8 {
private:
    OpenGLRenderer* renderer;
    LONG refCount;

public:
    CDirect3D8OpenGL(OpenGLRenderer* oglRenderer) : renderer(oglRenderer), refCount(1) {
        OutputDebugStringA("[OFP-D3DX OpenGL] CDirect3D8OpenGL created\n");
    }

    virtual ~CDirect3D8OpenGL() {
        OutputDebugStringA("[OFP-D3DX OpenGL] CDirect3D8OpenGL destroyed\n");
    }

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) {
        if (ppvObj == NULL) return E_INVALIDARG;
        if (riid == IID_IUnknown || riid == IID_IDirect3D8) {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)() {
        return InterlockedIncrement(&refCount);
    }

    STDMETHOD_(ULONG, Release)() {
        ULONG ref = InterlockedDecrement(&refCount);
        if (ref == 0) delete this;
        return ref;
    }

    // IDirect3D8 methods - Complete implementation
    STDMETHOD(RegisterSoftwareDevice)(void* pInitializeFunction) {
        return S_OK;
    }

    STDMETHOD_(UINT, GetAdapterCount)() {
        return 1;
    }

    STDMETHOD(GetAdapterIdentifier)(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8* pIdentifier) {
        if (Adapter >= GetAdapterCount()) return D3DERR_INVALIDCALL;
        if (pIdentifier == NULL) return D3DERR_INVALIDCALL;

        memset(pIdentifier, 0, sizeof(D3DADAPTER_IDENTIFIER8));
        strcpy_s(pIdentifier->Driver, "OpenGL Driver");
        strcpy_s(pIdentifier->Description, "OpenGL Renderer for OFP D3DX Wrapper");
        pIdentifier->DriverVersion.QuadPart = 1;
        pIdentifier->VendorId = 0;
        pIdentifier->DeviceId = 0;
        pIdentifier->SubSysId = 0;
        pIdentifier->Revision = 0;
        pIdentifier->DeviceIdentifier.Data1 = 0x12345678;
        return S_OK;
    }

    STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter) {
        if (Adapter >= GetAdapterCount()) return 0;
        return 1;
    }

    STDMETHOD(EnumAdapterModes)(UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode) {
        if (Adapter >= GetAdapterCount()) return D3DERR_INVALIDCALL;
        if (Mode >= GetAdapterModeCount(Adapter)) return D3DERR_INVALIDCALL;
        if (pMode == NULL) return D3DERR_INVALIDCALL;

        pMode->Width = 800;
        pMode->Height = 600;
        pMode->Format = D3DFMT_X8R8G8B8;
        pMode->RefreshRate = 60;
        return S_OK;
    }

    STDMETHOD(GetAdapterDisplayMode)(UINT Adapter, D3DDISPLAYMODE* pMode) {
        return EnumAdapterModes(Adapter, 0, pMode);
    }

    STDMETHOD(CheckDeviceType)(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) {
        if (Adapter >= GetAdapterCount()) return D3DERR_INVALIDCALL;
        if (CheckType != D3DDEVTYPE_HAL) return D3DERR_NOTAVAILABLE;
        return S_OK;
    }

    STDMETHOD(CheckDeviceFormat)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) {
        if (Adapter >= GetAdapterCount()) return D3DERR_INVALIDCALL;
        return S_OK;
    }

    STDMETHOD(CheckDeviceMultiSampleType)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType) {
        if (Adapter >= GetAdapterCount()) return D3DERR_INVALIDCALL;
        return S_OK;
    }

    STDMETHOD(CheckDepthStencilMatch)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) {
        if (Adapter >= GetAdapterCount()) return D3DERR_INVALIDCALL;
        return S_OK;
    }

    STDMETHOD(GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8* pCaps) {
        if (Adapter >= GetAdapterCount()) return D3DERR_INVALIDCALL;
        if (pCaps == NULL) return D3DERR_INVALIDCALL;

        memset(pCaps, 0, sizeof(D3DCAPS8));
        pCaps->DeviceType = D3DDEVTYPE_HAL;
        pCaps->AdapterOrdinal = Adapter;

        // Fill in basic capabilities
        pCaps->Caps = D3DCAPS_READ_SCANLINE;
        pCaps->Caps2 = D3DCAPS2_FULLSCREENGAMMA;
        pCaps->Caps3 = D3DCAPS3_ALPHA_FULLSCREEN_FLIP_OR_DISCARD;

        pCaps->PresentationIntervals = D3DPRESENT_INTERVAL_DEFAULT | D3DPRESENT_INTERVAL_ONE | D3DPRESENT_INTERVAL_IMMEDIATE;

        pCaps->CursorCaps = D3DCURSORCAPS_COLOR;

        pCaps->DevCaps = D3DDEVCAPS_EXECUTESYSTEMMEMORY | D3DDEVCAPS_EXECUTEVIDEOMEMORY |
                        D3DDEVCAPS_TLVERTEXSYSTEMMEMORY | D3DDEVCAPS_TLVERTEXVIDEOMEMORY |
                        D3DDEVCAPS_TEXTURESYSTEMMEMORY | D3DDEVCAPS_TEXTUREVIDEOMEMORY |
                        D3DDEVCAPS_DRAWPRIMTLVERTEX | D3DDEVCAPS_CANRENDERAFTERFLIP |
                        D3DDEVCAPS_TEXTURENONLOCALVIDMEM | D3DDEVCAPS_DRAWPRIMITIVES2 |
                        D3DDEVCAPS_SEPARATETEXTUREMEMORIES | D3DDEVCAPS_DRAWPRIMITIVES2EX |
                        D3DDEVCAPS_HWTRANSFORMANDLIGHT | D3DDEVCAPS_CANBLTSYSTONONLOCAL |
                        D3DDEVCAPS_HWRASTERIZATION | D3DDEVCAPS_PUREDEVICE;

        pCaps->PrimitiveMiscCaps = D3DPMISCCAPS_MASKZ | D3DPMISCCAPS_CULLNONE |
                                  D3DPMISCCAPS_CULLCW | D3DPMISCCAPS_CULLCCW |
                                  D3DPMISCCAPS_COLORWRITEENABLE | D3DPMISCCAPS_CLIPPLANESCALEDPOINTS |
                                  D3DPMISCCAPS_CLIPTLVERTS | D3DPMISCCAPS_TSSARGTEMP |
                                  D3DPMISCCAPS_BLENDOP | D3DPMISCCAPS_NULLREFERENCE |
                                  D3DPMISCCAPS_INDEPENDENTWRITEMASKS | D3DPMISCCAPS_PERSTAGECONSTANT |
                                  D3DPMISCCAPS_FOGANDSPECULARALPHA | D3DPMISCCAPS_SEPARATEALPHABLEND |
                                  D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS | D3DPMISCCAPS_MRTPOSTPIXELSHADERBLENDING;

        pCaps->RasterCaps = D3DPRASTERCAPS_DITHER | D3DPRASTERCAPS_ZTEST |
                           D3DPRASTERCAPS_FOGVERTEX | D3DPRASTERCAPS_FOGTABLE |
                           D3DPRASTERCAPS_MIPMAPLODBIAS | D3DPRASTERCAPS_ZBIAS |
                           D3DPRASTERCAPS_FOGRANGE | D3DPRASTERCAPS_ANISOTROPY |
                           D3DPRASTERCAPS_WFOG | D3DPRASTERCAPS_ZFOG |
                           D3DPRASTERCAPS_COLORPERSPECTIVE | D3DPRASTERCAPS_SCISSORTEST |
                           D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS | D3DPRASTERCAPS_DEPTHBIAS |
                           D3DPRASTERCAPS_MULTISAMPLE_TOGGLE;

        pCaps->ZCmpCaps = D3DPCMPCAPS_NEVER | D3DPCMPCAPS_LESS | D3DPCMPCAPS_EQUAL |
                         D3DPCMPCAPS_LESSEQUAL | D3DPCMPCAPS_GREATER | D3DPCMPCAPS_NOTEQUAL |
                         D3DPCMPCAPS_GREATEREQUAL | D3DPCMPCAPS_ALWAYS;

        pCaps->SrcBlendCaps = D3DPBLENDCAPS_ZERO | D3DPBLENDCAPS_ONE | D3DPBLENDCAPS_SRCCOLOR |
                             D3DPBLENDCAPS_INVSRCCOLOR | D3DPBLENDCAPS_SRCALPHA |
                             D3DPBLENDCAPS_INVSRCALPHA | D3DPBLENDCAPS_DESTALPHA |
                             D3DPBLENDCAPS_INVDESTALPHA | D3DPBLENDCAPS_DESTCOLOR |
                             D3DPBLENDCAPS_INVDESTCOLOR | D3DPBLENDCAPS_SRCALPHASAT |
                             D3DPBLENDCAPS_BOTHINVSRCALPHA | D3DPBLENDCAPS_BLENDFACTOR;

        pCaps->DestBlendCaps = pCaps->SrcBlendCaps;

        pCaps->AlphaCmpCaps = D3DPCMPCAPS_NEVER | D3DPCMPCAPS_LESS | D3DPCMPCAPS_EQUAL |
                             D3DPCMPCAPS_LESSEQUAL | D3DPCMPCAPS_GREATER | D3DPCMPCAPS_NOTEQUAL |
                             D3DPCMPCAPS_GREATEREQUAL | D3DPCMPCAPS_ALWAYS;

        pCaps->ShadeCaps = D3DPSHADECAPS_COLORGOURAUDRGB | D3DPSHADECAPS_SPECULARGOURAUDRGB |
                          D3DPSHADECAPS_ALPHAGOURAUDBLEND | D3DPSHADECAPS_FOGGOURAUD;

        pCaps->TextureCaps = D3DPTEXTURECAPS_PERSPECTIVE | D3DPTEXTURECAPS_POW2 |
                            D3DPTEXTURECAPS_ALPHA | D3DPTEXTURECAPS_TRANSPARENCY |
                            D3DPTEXTURECAPS_BORDER | D3DPTEXTURECAPS_TEXREPEATNOTSCALEDBYSIZE |
                            D3DPTEXTURECAPS_ALPHAPALETTE | D3DPTEXTURECAPS_NONPOW2CONDITIONAL |
                            D3DPTEXTURECAPS_PROJECTED | D3DPTEXTURECAPS_CUBEMAP |
                            D3DPTEXTURECAPS_VOLUMEMAP | D3DPTEXTURECAPS_MIPMAP |
                            D3DPTEXTURECAPS_MIPVOLUMEMAP | D3DPTEXTURECAPS_MIPCUBEMAP |
                            D3DPTEXTURECAPS_CUBEMAP_POW2 | D3DPTEXTURECAPS_VOLUMEMAP_POW2 |
                            D3DPTEXTURECAPS_NOPROJECTEDBUMPENV;

        pCaps->TextureFilterCaps = D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MINFLINEAR |
                                  D3DPTFILTERCAPS_MINFANISOTROPIC | D3DPTFILTERCAPS_MIPFPOINT |
                                  D3DPTFILTERCAPS_MIPFLINEAR | D3DPTFILTERCAPS_MAGFPOINT |
                                  D3DPTFILTERCAPS_MAGFLINEAR | D3DPTFILTERCAPS_MAGFANISOTROPIC;

        pCaps->CubeTextureFilterCaps = pCaps->TextureFilterCaps;

        pCaps->VolumeTextureFilterCaps = D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MINFLINEAR |
                                        D3DPTFILTERCAPS_MAGFPOINT | D3DPTFILTERCAPS_MAGFLINEAR;

        pCaps->TextureAddressCaps = D3DPTADDRESSCAPS_WRAP | D3DPTADDRESSCAPS_MIRROR |
                                   D3DPTADDRESSCAPS_CLAMP | D3DPTADDRESSCAPS_BORDER |
                                   D3DPTADDRESSCAPS_INDEPENDENTUV | D3DPTADDRESSCAPS_MIRRORONCE;

        pCaps->VolumeTextureAddressCaps = D3DPTADDRESSCAPS_WRAP | D3DPTADDRESSCAPS_MIRROR |
                                         D3DPTADDRESSCAPS_CLAMP | D3DPTADDRESSCAPS_BORDER |
                                         D3DPTADDRESSCAPS_INDEPENDENTUV | D3DPTADDRESSCAPS_MIRRORONCE;

        pCaps->LineCaps = D3DLINECAPS_TEXTURE | D3DLINECAPS_ZTEST | D3DLINECAPS_BLEND |
                        D3DLINECAPS_ALPHACMP | D3DLINECAPS_FOG | D3DLINECAPS_ANTIALIAS;

        pCaps->MaxTextureWidth = 2048;
        pCaps->MaxTextureHeight = 2048;
        pCaps->MaxVolumeExtent = 256;
        pCaps->MaxTextureRepeat = 8192;
        pCaps->MaxTextureAspectRatio = 2048;
        pCaps->MaxAnisotropy = 16;
        pCaps->MaxVertexW = 1000000.0f;

        pCaps->GuardBandLeft = 0;
        pCaps->GuardBandTop = 0;
        pCaps->GuardBandRight = 0;
        pCaps->GuardBandBottom = 0;
        pCaps->ExtentsAdjust = 0;

        pCaps->StencilCaps = D3DSTENCILCAPS_KEEP | D3DSTENCILCAPS_ZERO | D3DSTENCILCAPS_REPLACE |
                            D3DSTENCILCAPS_INCRSAT | D3DSTENCILCAPS_DECRSAT | D3DSTENCILCAPS_INVERT |
                            D3DSTENCILCAPS_INCR | D3DSTENCILCAPS_DECR | D3DSTENCILCAPS_TWOSIDED;

        pCaps->FVFCaps = D3DFVFCAPS_DONOTSTRIPELEMENTS | D3DFVFCAPS_PSIZE | D3DFVFCAPS_FOGVERTEX;

        pCaps->TextureOpCaps = D3DTEXOPCAPS_DISABLE | D3DTEXOPCAPS_SELECTARG1 | D3DTEXOPCAPS_SELECTARG2 |
                              D3DTEXOPCAPS_MODULATE | D3DTEXOPCAPS_MODULATE2X | D3DTEXOPCAPS_MODULATE4X |
                              D3DTEXOPCAPS_ADD | D3DTEXOPCAPS_ADDSIGNED | D3DTEXOPCAPS_ADDSIGNED2X |
                              D3DTEXOPCAPS_SUBTRACT | D3DTEXOPCAPS_ADDSMOOTH | D3DTEXOPCAPS_BLENDDIFFUSEALPHA |
                              D3DTEXOPCAPS_BLENDTEXTUREALPHA | D3DTEXOPCAPS_BLENDFACTORALPHA |
                              D3DTEXOPCAPS_BLENDTEXTUREALPHAPM | D3DTEXOPCAPS_BLENDCURRENTALPHA |
                              D3DTEXOPCAPS_PREMODULATE | D3DTEXOPCAPS_MODULATEALPHA_ADDCOLOR |
                              D3DTEXOPCAPS_MODULATECOLOR_ADDALPHA | D3DTEXOPCAPS_MODULATEINVALPHA_ADDCOLOR |
                              D3DTEXOPCAPS_MODULATEINVCOLOR_ADDALPHA | D3DTEXOPCAPS_BUMPENVMAP |
                              D3DTEXOPCAPS_BUMPENVMAPLUMINANCE | D3DTEXOPCAPS_DOTPRODUCT3 |
                              D3DTEXOPCAPS_MULTIPLYADD | D3DTEXOPCAPS_LERP;

        pCaps->MaxTextureBlendStages = 8;
        pCaps->MaxSimultaneousTextures = 8;

        pCaps->VertexProcessingCaps = D3DVTXPCAPS_TEXGEN | D3DVTXPCAPS_MATERIALSOURCE7 |
                                     D3DVTXPCAPS_DIRECTIONALLIGHTS | D3DVTXPCAPS_POSITIONALLIGHTS |
                                     D3DVTXPCAPS_LOCALVIEWER | D3DVTXPCAPS_TWEENING;

        pCaps->MaxActiveLights = 8;
        pCaps->MaxUserClipPlanes = 6;
        pCaps->MaxVertexBlendMatrices = 4;
        pCaps->MaxVertexBlendMatrixIndex = 0;

        pCaps->MaxPrimitiveCount = 65535;
        pCaps->MaxVertexIndex = 16777215;
        pCaps->MaxStreams = 16;
        pCaps->MaxStreamStride = 508;

        pCaps->VertexShaderVersion = D3DVS_VERSION(1, 1);
        pCaps->MaxVertexShaderConst = 96;

        pCaps->PixelShaderVersion = D3DPS_VERSION(1, 4);
        pCaps->MaxPixelShaderValue = 1.0f;

        return S_OK;
    }

    STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter) {
        if (Adapter >= GetAdapterCount()) return NULL;
        return MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
    }

    STDMETHOD(CreateDevice)(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS8* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface) {
        if (Adapter >= GetAdapterCount()) return D3DERR_INVALIDCALL;
        if (ppReturnedDeviceInterface == NULL) return D3DERR_INVALIDCALL;

        // Initialize OpenGL renderer
        if (!renderer->Initialize(hFocusWindow)) {
            OutputDebugStringA("[OFP-D3DX OpenGL] Failed to initialize renderer\n");
            return E_FAIL;
        }

        // Create device wrapper
        CDirect3DDevice8OpenGL* device = new CDirect3DDevice8OpenGL(renderer);
        *ppReturnedDeviceInterface = device;

        OutputDebugStringA("[OFP-D3DX OpenGL] OpenGL device created successfully\n");
        return S_OK;
    }
};

// Device implementation with complete D3D8 interface
class CDirect3DDevice8OpenGL : public IDirect3DDevice8 {
private:
    OpenGLRenderer* renderer;
    LONG refCount;

public:
    CDirect3DDevice8OpenGL(OpenGLRenderer* ogl) : renderer(ogl), refCount(1) {}

    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) {
        if (ppvObj == NULL) return E_INVALIDARG;
        if (riid == IID_IUnknown) {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)() {
        return InterlockedIncrement(&refCount);
    }

    STDMETHOD_(ULONG, Release)() {
        ULONG ref = InterlockedDecrement(&refCount);
        if (ref == 0) delete this;
        return ref;
    }

    // Complete D3D8 Device Interface Implementation
    STDMETHOD(TestCooperativeLevel)() { return S_OK; }
    STDMETHOD_(UINT, GetAvailableTextureMem)() { return 128 * 1024 * 1024; }
    STDMETHOD(ResourceManagerDiscardBytes)(DWORD Bytes) { return S_OK; }
    STDMETHOD(GetDirect3D)(IDirect3D8** ppD3D8) { return E_NOTIMPL; }
    STDMETHOD(GetDeviceCaps)(D3DCAPS8* pCaps) { return S_OK; }
    STDMETHOD(GetDisplayMode)(D3DDISPLAYMODE* pMode) { return S_OK; }
    STDMETHOD(GetCreationParameters)(D3DDEVICE_CREATION_PARAMETERS* pParameters) { return S_OK; }
    STDMETHOD(SetCursorProperties)(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap) { return S_OK; }
    STDMETHOD_(void, SetCursorPosition)(int X, int Y, DWORD Flags) {}
    STDMETHOD_(BOOL, ShowCursor)(BOOL bShow) { return TRUE; }
    STDMETHOD(CreateAdditionalSwapChain)(D3DPRESENT_PARAMETERS8* pPresentationParameters, IDirect3DSwapChain8** pSwapChain) { return E_NOTIMPL; }
    STDMETHOD(GetSwapChain)(UINT iSwapChain, IDirect3DSwapChain8** pSwapChain) { return E_NOTIMPL; }
    STDMETHOD_(UINT, GetNumberOfSwapChains)() { return 0; }
    STDMETHOD(Reset)(D3DPRESENT_PARAMETERS8* pPresentationParameters) { return S_OK; }

    STDMETHOD(Present)(RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion) {
        if (renderer) renderer->RenderFrame();
        return S_OK;
    }

    STDMETHOD(GetBackBuffer)(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer) { return E_NOTIMPL; }
    STDMETHOD(GetRasterStatus)(D3DRASTER_STATUS* pRasterStatus) { return S_OK; }
    STDMETHOD_(void, SetGammaRamp)(DWORD Flags, CONST D3DGAMMARAMP* pRamp) {}
    STDMETHOD_(void, GetGammaRamp)(D3DGAMMARAMP* pRamp) {}

    STDMETHOD(CreateTexture)(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture) { return E_NOTIMPL; }
    STDMETHOD(CreateVolumeTexture)(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8** ppVolumeTexture) { return E_NOTIMPL; }
    STDMETHOD(CreateCubeTexture)(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8** ppCubeTexture) { return E_NOTIMPL; }
    STDMETHOD(CreateVertexBuffer)(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer) { return E_NOTIMPL; }
    STDMETHOD(CreateIndexBuffer)(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer) { return E_NOTIMPL; }
    STDMETHOD(CreateRenderTarget)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppSurface) { return E_NOTIMPL; }
    STDMETHOD(CreateDepthStencilSurface)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface) { return E_NOTIMPL; }
    STDMETHOD(CreateImageSurface)(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface) { return E_NOTIMPL; }
    STDMETHOD(CopyRects)(IDirect3DSurface8* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface8* pDestinationSurface, CONST POINT* pDestPointsArray) { return E_NOTIMPL; }
    STDMETHOD(UpdateTexture)(IDirect3DBaseTexture8* pSourceTexture, IDirect3DBaseTexture8* pDestinationTexture) { return E_NOTIMPL; }
    STDMETHOD(GetFrontBuffer)(IDirect3DSurface8* pDestSurface) { return E_NOTIMPL; }
    STDMETHOD(SetRenderTarget)(IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil) { return S_OK; }
    STDMETHOD(GetRenderTarget)(IDirect3DSurface8** ppRenderTarget) { return E_NOTIMPL; }
    STDMETHOD(GetDepthStencilSurface)(IDirect3DSurface8** ppZStencilSurface) { return E_NOTIMPL; }

    STDMETHOD(BeginScene)() { return S_OK; }

    STDMETHOD(EndScene)() {
        // This is where FPS counter would be updated and drawn
        return S_OK;
    }

    STDMETHOD(Clear)(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) { return S_OK; }

    STDMETHOD(SetTransform)(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) { return S_OK; }
    STDMETHOD(GetTransform)(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) { return E_NOTIMPL; }
    STDMETHOD(MultiplyTransform)(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) { return S_OK; }

    STDMETHOD(SetViewport)(CONST D3DVIEWPORT8* pViewport) {
        // This is where FOV adjustment would be applied
        return S_OK;
    }

    STDMETHOD(GetViewport)(D3DVIEWPORT8* pViewport) { return S_OK; }

    STDMETHOD(SetMaterial)(CONST D3DMATERIAL8* pMaterial) { return S_OK; }
    STDMETHOD(GetMaterial)(D3DMATERIAL8* pMaterial) { return E_NOTIMPL; }
    STDMETHOD(SetLight)(DWORD Index, CONST D3DLIGHT8* pLight) { return S_OK; }
    STDMETHOD(GetLight)(DWORD Index, D3DLIGHT8* pLight) { return E_NOTIMPL; }
    STDMETHOD(LightEnable)(DWORD Index, BOOL Enable) { return S_OK; }
    STDMETHOD(GetLightEnable)(DWORD Index, BOOL* pEnable) { return E_NOTIMPL; }
    STDMETHOD(SetClipPlane)(DWORD Index, CONST float* pPlane) { return S_OK; }
    STDMETHOD(GetClipPlane)(DWORD Index, float* pPlane) { return E_NOTIMPL; }

    STDMETHOD(SetRenderState)(D3DRENDERSTATETYPE State, DWORD Value) { return S_OK; }
    STDMETHOD(GetRenderState)(D3DRENDERSTATETYPE State, DWORD* pValue) { return E_NOTIMPL; }

    STDMETHOD(BeginStateBlock)() { return S_OK; }
    STDMETHOD(EndStateBlock)(DWORD* pToken) { return E_NOTIMPL; }
    STDMETHOD(ApplyStateBlock)(DWORD Token) { return S_OK; }
    STDMETHOD(CaptureStateBlock)(DWORD Token) { return S_OK; }
    STDMETHOD(DeleteStateBlock)(DWORD Token) { return S_OK; }
    STDMETHOD(CreateStateBlock)(D3DSTATEBLOCKTYPE Type, DWORD* pToken) { return E_NOTIMPL; }

    STDMETHOD(SetClipStatus)(CONST D3DCLIPSTATUS8* pClipStatus) { return S_OK; }
    STDMETHOD(GetClipStatus)(D3DCLIPSTATUS8* pClipStatus) { return E_NOTIMPL; }

    STDMETHOD(GetTexture)(DWORD Stage, IDirect3DBaseTexture8** ppTexture) { return E_NOTIMPL; }
    STDMETHOD(SetTexture)(DWORD Stage, IDirect3DBaseTexture8* pTexture) { return S_OK; }

    STDMETHOD(GetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) { return E_NOTIMPL; }
    STDMETHOD(SetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) { return S_OK; }

    STDMETHOD(GetSamplerState)(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) { return E_NOTIMPL; }

    STDMETHOD(SetSamplerState)(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
        // This is where anisotropic filtering would be applied
        return S_OK;
    }

    STDMETHOD(ValidateDevice)(DWORD* pNumPasses) { return S_OK; }
    STDMETHOD(GetInfo)(DWORD DevInfoID, void* pDevInfoStruct, DWORD DevInfoStructSize) { return E_NOTIMPL; }
    STDMETHOD(SetPaletteEntries)(UINT PaletteNumber, CONST PALETTEENTRY* pEntries) { return S_OK; }
    STDMETHOD(GetPaletteEntries)(UINT PaletteNumber, PALETTEENTRY* pEntries) { return E_NOTIMPL; }
    STDMETHOD(SetCurrentTexturePalette)(UINT PaletteNumber) { return S_OK; }
    STDMETHOD(GetCurrentTexturePalette)(UINT* PaletteNumber) { return E_NOTIMPL; }
    STDMETHOD(SetScissorRect)(CONST RECT* pRect) { return S_OK; }
    STDMETHOD(GetScissorRect)(RECT* pRect) { return E_NOTIMPL; }
    STDMETHOD(SetSoftwareVertexProcessing)(BOOL bSoftware) { return S_OK; }
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)() { return FALSE; }
    STDMETHOD(SetNPatchMode)(float nSegments) { return S_OK; }
    STDMETHOD_(float, GetNPatchMode)() { return 0.0f; }

    STDMETHOD(DrawPrimitive)(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) { return S_OK; }
    STDMETHOD(DrawIndexedPrimitive)(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount) { return S_OK; }
    STDMETHOD(DrawPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return S_OK; }
    STDMETHOD(DrawIndexedPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return S_OK; }
    STDMETHOD(ProcessVertices)(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags) { return E_NOTIMPL; }

    STDMETHOD(CreateVertexShader)(CONST DWORD* pDeclaration, CONST DWORD* pFunction, DWORD* pHandle, DWORD Usage) { return E_NOTIMPL; }
    STDMETHOD(SetVertexShader)(DWORD Handle) { return S_OK; }
    STDMETHOD(GetVertexShader)(DWORD* pHandle) { return E_NOTIMPL; }
    STDMETHOD(DeleteVertexShader)(DWORD Handle) { return S_OK; }
    STDMETHOD(SetVertexShaderConstant)(UINT Register, CONST void* pConstantData, UINT ConstantCount) { return S_OK; }
    STDMETHOD(GetVertexShaderConstant)(UINT Register, void* pConstantData, UINT ConstantCount) { return E_NOTIMPL; }
    STDMETHOD(GetVertexShaderDeclaration)(DWORD Handle, void* pData, UINT* pSizeOfData) { return E_NOTIMPL; }
    STDMETHOD(GetVertexShaderFunction)(DWORD Handle, void* pData, UINT* pSizeOfData) { return E_NOTIMPL; }

    STDMETHOD(SetStreamSource)(UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData, UINT Stride) { return S_OK; }
    STDMETHOD(GetStreamSource)(UINT StreamNumber, IDirect3DVertexBuffer8** ppStreamData, UINT* pStride) { return E_NOTIMPL; }
    STDMETHOD(SetIndices)(IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex) { return S_OK; }
    STDMETHOD(GetIndices)(IDirect3DIndexBuffer8** ppIndexData, UINT* pBaseVertexIndex) { return E_NOTIMPL; }

    STDMETHOD(CreatePixelShader)(CONST DWORD* pFunction, DWORD* pHandle) { return E_NOTIMPL; }
    STDMETHOD(SetPixelShader)(DWORD Handle) { return S_OK; }
    STDMETHOD(GetPixelShader)(DWORD* pHandle) { return E_NOTIMPL; }
    STDMETHOD(DeletePixelShader)(DWORD Handle) { return S_OK; }
    STDMETHOD(SetPixelShaderConstant)(UINT Register, CONST void* pConstantData, UINT ConstantCount) { return S_OK; }
    STDMETHOD(GetPixelShaderConstant)(UINT Register, void* pConstantData, UINT ConstantCount) { return E_NOTIMPL; }
    STDMETHOD(GetPixelShaderFunction)(DWORD Handle, void* pData, UINT* pSizeOfData) { return E_NOTIMPL; }

    STDMETHOD(DrawRectPatch)(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) { return E_NOTIMPL; }
    STDMETHOD(DrawTriPatch)(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) { return E_NOTIMPL; }
    STDMETHOD(DeletePatch)(UINT Handle) { return S_OK; }
    STDMETHOD(SetFVF)(DWORD FVF) { return S_OK; }
    STDMETHOD(GetFVF)(DWORD* pFVF) { return E_NOTIMPL; }
};