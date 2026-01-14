#include "config.hpp"
#include <d3d9.h>
#include <initguid.h>
#include <stdio.h>
#include <time.h>

DEFINE_GUID(IID_IDirect3D8, 0x1DD9E8DA, 0x6C27, 0x41C8, 0xA7, 0xE7, 0x76, 0x15, 0xEE, 0xF7, 0x5C, 0x55);

static LONG g_RefCount = 0;

static DWORD g_FrameCount = 0;
static DWORD g_LastFPSTime = 0;
static DWORD g_CurrentFPS = 0;
static BOOL g_FOVToggleState = TRUE;
static DWORD g_LastFOVCheck = 0;
static BOOL g_LastKeyState = FALSE;

interface IDirect3D8 : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE RegisterSoftwareDevice(void* pInitializeFunction) = 0;
    virtual UINT STDMETHODCALLTYPE GetAdapterCount(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT Adapter, DWORD Flags, void* pIdentifier) = 0;
    virtual UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT Adapter) = 0;
    virtual HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT Adapter, UINT Mode, void* pMode) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT Adapter, void* pMode) = 0;
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) = 0;
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) = 0;
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType) = 0;
    virtual HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, void* pCaps) = 0;
    virtual HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, void* pPresentationParameters, void** ppReturnedDeviceInterface) = 0;
};

class CDirect3DDevice8 : public IDirect3DDevice9
{
private:
    IDirect3DDevice9* m_pDevice9;

public:
    CDirect3DDevice8(IDirect3DDevice9* pDevice9) : m_pDevice9(pDevice9)
    {
        if (m_pDevice9)
            m_pDevice9->AddRef();

        Config::Load();
        g_LastFPSTime = GetTickCount();
        g_FrameCount = 0;
        g_CurrentFPS = 0;

        Config::Log("[OFP-D3DX] CDirect3DDevice8 initialized - FOV Toggle Key: %lu, AllowDynamicFOV: %d\n",
            Config::g_Config.FOVToggleKey, Config::g_Config.AllowDynamicFOV);
    }

    virtual ~CDirect3DDevice8()
    {
        if (m_pDevice9)
            m_pDevice9->Release();
    }

    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
    {
        return m_pDevice9->QueryInterface(riid, ppvObj);
    }

    STDMETHOD_(ULONG, AddRef)()
    {
        return InterlockedIncrement(&g_RefCount);
    }

    STDMETHOD_(ULONG, Release)()
    {
        ULONG RefCount = InterlockedDecrement(&g_RefCount);
        if (RefCount == 0)
            delete this;
        return RefCount;
    }

    STDMETHOD(TestCooperativeLevel)()
    {
        return m_pDevice9->TestCooperativeLevel();
    }

    STDMETHOD_(UINT, GetAvailableTextureMem)()
    {
        return m_pDevice9->GetAvailableTextureMem();
    }

    STDMETHOD(EvictManagedResources)()
    {
        return m_pDevice9->EvictManagedResources();
    }

    STDMETHOD(GetDirect3D)(IDirect3D9** ppD3D9)
    {
        return m_pDevice9->GetDirect3D(ppD3D9);
    }

    STDMETHOD(GetDeviceCaps)(D3DCAPS9* pCaps)
    {
        return m_pDevice9->GetDeviceCaps(pCaps);
    }

    STDMETHOD(GetDisplayMode)(UINT Adapter, D3DDISPLAYMODE* pMode)
    {
        return m_pDevice9->GetDisplayMode(Adapter, pMode);
    }

    STDMETHOD(GetCreationParameters)(D3DDEVICE_CREATION_PARAMETERS* pParameters)
    {
        return m_pDevice9->GetCreationParameters(pParameters);
    }

    STDMETHOD(SetCursorProperties)(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
    {
        return m_pDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
    }

    STDMETHOD_(void, SetCursorPosition)(int X, int Y, DWORD Flags)
    {
        m_pDevice9->SetCursorPosition(X, Y, Flags);
    }

    STDMETHOD_(BOOL, ShowCursor)(BOOL bShow)
    {
        return m_pDevice9->ShowCursor(bShow);
    }

    STDMETHOD(CreateAdditionalSwapChain)(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
    {
        return m_pDevice9->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
    }

    STDMETHOD(GetSwapChain)(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
    {
        return m_pDevice9->GetSwapChain(iSwapChain, pSwapChain);
    }

    STDMETHOD_(UINT, GetNumberOfSwapChains)()
    {
        return m_pDevice9->GetNumberOfSwapChains();
    }

    STDMETHOD(Reset)(D3DPRESENT_PARAMETERS* pPresentationParameters)
    {
        return m_pDevice9->Reset(pPresentationParameters);
    }

    STDMETHOD(Present)(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
    {
        return m_pDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    }

    STDMETHOD(GetBackBuffer)(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
    {
        return m_pDevice9->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
    }

    STDMETHOD(GetRasterStatus)(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
    {
        return m_pDevice9->GetRasterStatus(iSwapChain, pRasterStatus);
    }

    STDMETHOD(SetDialogBoxMode)(BOOL bEnableDialogs)
    {
        return m_pDevice9->SetDialogBoxMode(bEnableDialogs);
    }

    STDMETHOD_(void, SetGammaRamp)(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
    {
        m_pDevice9->SetGammaRamp(iSwapChain, Flags, pRamp);
    }

    STDMETHOD_(void, GetGammaRamp)(UINT iSwapChain, D3DGAMMARAMP* pRamp)
    {
        m_pDevice9->GetGammaRamp(iSwapChain, pRamp);
    }

    STDMETHOD(CreateTexture)(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
    {
        return m_pDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
    }

    STDMETHOD(CreateVolumeTexture)(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
    {
        return m_pDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
    }

    STDMETHOD(CreateCubeTexture)(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
    {
        return m_pDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
    }

    STDMETHOD(CreateVertexBuffer)(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
    {
        return m_pDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
    }

    STDMETHOD(CreateIndexBuffer)(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
    {
        return m_pDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
    }

    STDMETHOD(CreateRenderTarget)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
    {
        return m_pDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
    }

    STDMETHOD(CreateDepthStencilSurface)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
    {
        return m_pDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
    }

    STDMETHOD(CreateOffscreenPlainSurface)(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
    {
        return m_pDevice9->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
    }

    STDMETHOD(CreateVertexDeclaration)(CONST D3DVERTEXELEMENT9* pElements, IDirect3DVertexDeclaration9** ppDecl)
    {
        return m_pDevice9->CreateVertexDeclaration(pElements, ppDecl);
    }

    STDMETHOD(CreateVertexShader)(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
    {
        return m_pDevice9->CreateVertexShader(pFunction, ppShader);
    }

    STDMETHOD(CreatePixelShader)(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
    {
        return m_pDevice9->CreatePixelShader(pFunction, ppShader);
    }

    STDMETHOD(SetVertexDeclaration)(IDirect3DVertexDeclaration9* pDecl)
    {
        return m_pDevice9->SetVertexDeclaration(pDecl);
    }

    STDMETHOD(GetVertexDeclaration)(IDirect3DVertexDeclaration9** ppDecl)
    {
        return m_pDevice9->GetVertexDeclaration(ppDecl);
    }

    STDMETHOD(SetVertexShader)(IDirect3DVertexShader9* pShader)
    {
        return m_pDevice9->SetVertexShader(pShader);
    }

    STDMETHOD(GetVertexShader)(IDirect3DVertexShader9** ppShader)
    {
        return m_pDevice9->GetVertexShader(ppShader);
    }

    STDMETHOD(SetVertexShaderConstantF)(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
    {
        return m_pDevice9->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(GetVertexShaderConstantF)(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
    {
        return m_pDevice9->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(SetVertexShaderConstantI)(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
    {
        return m_pDevice9->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(GetVertexShaderConstantI)(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
    {
        return m_pDevice9->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(SetVertexShaderConstantB)(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
    {
        return m_pDevice9->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(GetVertexShaderConstantB)(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
    {
        return m_pDevice9->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(SetPixelShader)(IDirect3DPixelShader9* pShader)
    {
        return m_pDevice9->SetPixelShader(pShader);
    }

    STDMETHOD(GetPixelShader)(IDirect3DPixelShader9** ppShader)
    {
        return m_pDevice9->GetPixelShader(ppShader);
    }

    STDMETHOD(SetPixelShaderConstantF)(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
    {
        return m_pDevice9->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(GetPixelShaderConstantF)(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
    {
        return m_pDevice9->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(SetPixelShaderConstantI)(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
    {
        return m_pDevice9->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(GetPixelShaderConstantI)(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
    {
        return m_pDevice9->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(SetPixelShaderConstantB)(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
    {
        return m_pDevice9->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(GetPixelShaderConstantB)(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
    {
        return m_pDevice9->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(DrawPrimitive)(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
    {
        return m_pDevice9->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
    }

    STDMETHOD(DrawIndexedPrimitive)(D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
    {
        return m_pDevice9->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
    }

    STDMETHOD(DrawPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
    {
        return m_pDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    }

    STDMETHOD(DrawIndexedPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
    {
        return m_pDevice9->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
    }

    STDMETHOD(ProcessVertices)(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
    {
        return m_pDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
    }

    STDMETHOD(SetVertexStream)(UINT Stream, IDirect3DVertexBuffer9* pStreamData, UINT Stride)
    {
        return m_pDevice9->SetVertexStream(Stream, pStreamData, Stride);
    }

    STDMETHOD(GetVertexStream)(UINT Stream, IDirect3DVertexBuffer9** ppStreamData, UINT* pStride)
    {
        return m_pDevice9->GetVertexStream(Stream, ppStreamData, pStride);
    }

    STDMETHOD(SetVertexStreamFreq)(UINT Stream, DWORD Frequency)
    {
        return m_pDevice9->SetVertexStreamFreq(Stream, Frequency);
    }

    STDMETHOD(GetVertexStreamFreq)(UINT Stream, DWORD* pFrequency)
    {
        return m_pDevice9->GetVertexStreamFreq(Stream, pFrequency);
    }

    STDMETHOD(SetIndices)(IDirect3DIndexBuffer9* pIndexData)
    {
        return m_pDevice9->SetIndices(pIndexData);
    }

    STDMETHOD(GetIndices)(IDirect3DIndexBuffer9** ppIndexData)
    {
        return m_pDevice9->GetIndices(ppIndexData);
    }

    STDMETHOD(SetViewport)(CONST D3DVIEWPORT9* pViewport)
    {
        D3DVIEWPORT9 vp = *pViewport;

        if (Config::g_Config.AllowDynamicFOV)
        {
            DWORD currentTime = GetTickCount();
            if (currentTime - g_LastFOVCheck >= 100)
            {
                g_LastFOVCheck = currentTime;

                BOOL keyPressed = (GetAsyncKeyState((int)Config::g_Config.FOVToggleKey) & 0x8000) != 0;

                if (keyPressed && !g_LastKeyState)
                {
                    g_FOVToggleState = !g_FOVToggleState;
                    Config::Log("[OFP-D3DX] FOV toggle: %s\n", g_FOVToggleState ? "ON" : "OFF");
                }
                g_LastKeyState = keyPressed;
            }

            float targetFOV = (float)Config::g_Config.DefaultFOV;
            if (!g_FOVToggleState)
            {
                targetFOV *= Config::g_Config.AimingFOVMultiplier;
            }

            vp.Width = (DWORD)(vp.Height / (2.0f * tan(targetFOV * 3.14159f / 360.0f)));
            vp.X = (DWORD)((vp.Width - vp.Height * 4.0f / 3.0f) / 2);
        }

        return m_pDevice9->SetViewport(&vp);
    }

    STDMETHOD(GetViewport)(D3DVIEWPORT9* pViewport)
    {
        return m_pDevice9->GetViewport(pViewport);
    }

    STDMETHOD(SetMaterial)(CONST D3DMATERIAL9* pMaterial)
    {
        return m_pDevice9->SetMaterial(pMaterial);
    }

    STDMETHOD(GetMaterial)(D3DMATERIAL9* pMaterial)
    {
        return m_pDevice9->GetMaterial(pMaterial);
    }

    STDMETHOD(SetLight)(DWORD Index, CONST D3DLIGHT9* pLight)
    {
        return m_pDevice9->SetLight(Index, pLight);
    }

    STDMETHOD(GetLight)(DWORD Index, D3DLIGHT9* pLight)
    {
        return m_pDevice9->GetLight(Index, pLight);
    }

    STDMETHOD(LightEnable)(DWORD Index, BOOL Enable)
    {
        return m_pDevice9->LightEnable(Index, Enable);
    }

    STDMETHOD(GetLightEnable)(DWORD Index, BOOL* pEnable)
    {
        return m_pDevice9->GetLightEnable(Index, pEnable);
    }

    STDMETHOD(SetClipPlane)(DWORD Index, CONST float* pPlane)
    {
        return m_pDevice9->SetClipPlane(Index, pPlane);
    }

    STDMETHOD(GetClipPlane)(DWORD Index, float* pPlane)
    {
        return m_pDevice9->GetClipPlane(Index, pPlane);
    }

    STDMETHOD(SetRenderState)(D3DRENDERSTATETYPE State, DWORD Value)
    {
        return m_pDevice9->SetRenderState(State, Value);
    }

    STDMETHOD(GetRenderState)(D3DRENDERSTATETYPE State, DWORD* pValue)
    {
        return m_pDevice9->GetRenderState(State, pValue);
    }

    STDMETHOD(CreateStateBlock)(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
    {
        return m_pDevice9->CreateStateBlock(Type, ppSB);
    }

    STDMETHOD(BeginStateBlock)()
    {
        return m_pDevice9->BeginStateBlock();
    }

    STDMETHOD(EndStateBlock)(IDirect3DStateBlock9** ppSB)
    {
        return m_pDevice9->EndStateBlock(ppSB);
    }

    STDMETHOD(SetClipStatus)(CONST D3DCLIPSTATUS9* pClipStatus)
    {
        return m_pDevice9->SetClipStatus(pClipStatus);
    }

    STDMETHOD(GetClipStatus)(D3DCLIPSTATUS9* pClipStatus)
    {
        return m_pDevice9->GetClipStatus(pClipStatus);
    }

    STDMETHOD(GetTexture)(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
    {
        return m_pDevice9->GetTexture(Stage, ppTexture);
    }

    STDMETHOD(SetTexture)(DWORD Stage, IDirect3DBaseTexture9* pTexture)
    {
        return m_pDevice9->SetTexture(Stage, pTexture);
    }

    STDMETHOD(GetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
    {
        return m_pDevice9->GetTextureStageState(Stage, Type, pValue);
    }

    STDMETHOD(SetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
    {
        if (Type == D3DTSS_MINFILTER && Config::g_Config.AnisotropicFiltering > 0)
        {
            return m_pDevice9->SetTextureStageState(Stage, Type, D3DTEXF_ANISOTROPIC);
        }
        return m_pDevice9->SetTextureStageState(Stage, Type, Value);
    }

    STDMETHOD(GetSamplerState)(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
    {
        return m_pDevice9->GetSamplerState(Sampler, Type, pValue);
    }

    STDMETHOD(SetSamplerState)(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
    {
        if (Type == D3DSAMP_MAXANISOTROPY && Config::g_Config.AnisotropicFiltering > 0)
        {
            return m_pDevice9->SetSamplerState(Sampler, Type, Config::g_Config.AnisotropicFiltering);
        }
        return m_pDevice9->SetSamplerState(Sampler, Type, Value);
    }

    STDMETHOD(SetScissorRect)(CONST RECT* pRect)
    {
        return m_pDevice9->SetScissorRect(pRect);
    }

    STDMETHOD(GetScissorRect)(RECT* pRect)
    {
        return m_pDevice9->GetScissorRect(pRect);
    }

    STDMETHOD(SetSoftwareVertexProcessing)(BOOL bSoftware)
    {
        return m_pDevice9->SetSoftwareVertexProcessing(bSoftware);
    }

    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)()
    {
        return m_pDevice9->GetSoftwareVertexProcessing();
    }

    STDMETHOD(SetNPatchMode)(float nSegments)
    {
        return m_pDevice9->SetNPatchMode(nSegments);
    }

    STDMETHOD_(float, GetNPatchMode)()
    {
        return m_pDevice9->GetNPatchMode();
    }

    STDMETHOD(DrawIndexedPrimitiveStrided)(D3DPRIMITIVETYPE PrimitiveType, UINT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT startIndex, UINT primCount)
    {
        return m_pDevice9->DrawIndexedPrimitiveStrided(PrimitiveType, BaseVertexIndex, MinIndex, NumVertices, startIndex, primCount);
    }

    STDMETHOD(ValidateDevice)(DWORD* pNumPasses)
    {
        return m_pDevice9->ValidateDevice(pNumPasses);
    }

    STDMETHOD(SetPaletteEntries)(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
    {
        return m_pDevice9->SetPaletteEntries(PaletteNumber, pEntries);
    }

    STDMETHOD(GetPaletteEntries)(UINT PaletteNumber, PALETTEENTRY* pEntries)
    {
        return m_pDevice9->GetPaletteEntries(PaletteNumber, pEntries);
    }

    STDMETHOD(SetCurrentTexturePalette)(UINT PaletteNumber)
    {
        return m_pDevice9->SetCurrentTexturePalette(PaletteNumber);
    }

    STDMETHOD_(UINT, GetCurrentTexturePalette)()
    {
        return m_pDevice9->GetCurrentTexturePalette();
    }

    STDMETHOD(SetFVF)(DWORD FVF)
    {
        return m_pDevice9->SetFVF(FVF);
    }

    STDMETHOD(GetFVF)(DWORD* pFVF)
    {
        return m_pDevice9->GetFVF(pFVF);
    }

    STDMETHOD(SetStreamSource)(UINT Stream, IDirect3DVertexBuffer9* pStreamData, UINT Stride)
    {
        return m_pDevice9->SetStreamSource(Stream, pStreamData, Stride);
    }

    STDMETHOD(GetStreamSource)(UINT Stream, IDirect3DVertexBuffer9** ppStreamData, UINT* pStride)
    {
        return m_pDevice9->GetStreamSource(Stream, ppStreamData, pStride);
    }

    STDMETHOD(SetStreamSourceFreq)(UINT Stream, DWORD Frequency)
    {
        return m_pDevice9->SetStreamSourceFreq(Stream, Frequency);
    }

    STDMETHOD(GetStreamSourceFreq)(UINT Stream, DWORD* pFrequency)
    {
        return m_pDevice9->GetStreamSourceFreq(Stream, pFrequency);
    }

    STDMETHOD(SetRenderTarget)(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
    {
        return m_pDevice9->SetRenderTarget(RenderTargetIndex, pRenderTarget);
    }

    STDMETHOD(GetRenderTarget)(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
    {
        return m_pDevice9->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
    }

    STDMETHOD(SetDepthStencilSurface)(IDirect3DSurface9* pNewZStencil)
    {
        return m_pDevice9->SetDepthStencilSurface(pNewZStencil);
    }

    STDMETHOD(GetDepthStencilSurface)(IDirect3DSurface9** ppZStencilSurface)
    {
        return m_pDevice9->GetDepthStencilSurface(ppZStencilSurface);
    }

    STDMETHOD(SetTransform)(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
    {
        return m_pDevice9->SetTransform(State, pMatrix);
    }

    STDMETHOD(GetTransform)(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
    {
        return m_pDevice9->GetTransform(State, pMatrix);
    }

    STDMETHOD(MultiplyTransform)(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
    {
        return m_pDevice9->MultiplyTransform(State, pMatrix);
    }

    STDMETHOD(DeletePatch)(UINT Handle)
    {
        return m_pDevice9->DeletePatch(Handle);
    }

    STDMETHOD(CreateQuery)(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
    {
        return m_pDevice9->CreateQuery(Type, ppQuery);
    }

    void UpdateFPS()
    {
        DWORD currentTime = GetTickCount();
        g_FrameCount++;

        DWORD elapsed = currentTime - g_LastFPSTime;
        if (elapsed >= (Config::g_Config.FPSUpdateInterval * 1000))
        {
            g_CurrentFPS = (g_FrameCount * 1000) / elapsed;
            g_FrameCount = 0;
            g_LastFPSTime = currentTime;

            Config::Log("[OFP-D3DX] FPS: %u\n", g_CurrentFPS);
        }
    }

    STDMETHOD(EndScene)()
    {
        UpdateFPS();
        return m_pDevice9->EndScene();
    }

    STDMETHOD(BeginScene)()
    {
        return m_pDevice9->BeginScene();
    }

    STDMETHOD(Clear)(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
    {
        return m_pDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil);
    }

    STDMETHOD(ColorFill)(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR Color)
    {
        return m_pDevice9->ColorFill(pSurface, pRect, Color);
    }

    STDMETHOD(DrawRectPatch)(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pPatchInfo)
    {
        return m_pDevice9->DrawRectPatch(Handle, pNumSegs, pPatchInfo);
    }

    STDMETHOD(DrawTriPatch)(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pPatchInfo)
    {
        return m_pDevice9->DrawTriPatch(Handle, pNumSegs, pPatchInfo);
    }

    STDMETHOD(GetFrontBufferData)(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
    {
        return m_pDevice9->GetFrontBufferData(iSwapChain, pDestSurface);
    }

    STDMETHOD(GetRenderTargetData)(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
    {
        return m_pDevice9->GetRenderTargetData(pRenderTarget, pDestSurface);
    }

    STDMETHOD(StretchRect)(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
    {
        return m_pDevice9->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
    }

    STDMETHOD(UpdateSurface)(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST POINT* pDestPoint)
    {
        return m_pDevice9->UpdateSurface(pSourceSurface, pSourceRect, pDestSurface, pDestPoint);
    }

    STDMETHOD(UpdateTexture)(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
    {
        return m_pDevice9->UpdateTexture(pSourceTexture, pDestinationTexture);
    }
};

class CDirect3D8 : public IDirect3D8
{
private:
    IDirect3D9* m_pD3D9;

public:
    CDirect3D8(IDirect3D9* pD3D9) : m_pD3D9(pD3D9)
    {
        if (m_pD3D9)
            m_pD3D9->AddRef();
    }

    virtual ~CDirect3D8()
    {
        if (m_pD3D9)
            m_pD3D9->Release();
    }

    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
    {
        if (ppvObj == nullptr)
            return E_INVALIDARG;

        if (riid == IID_IUnknown || riid == IID_IDirect3D8)
        {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)()
    {
        return InterlockedIncrement(&g_RefCount);
    }

    STDMETHOD_(ULONG, Release)()
    {
        ULONG RefCount = InterlockedDecrement(&g_RefCount);
        if (RefCount == 0)
            delete this;
        return RefCount;
    }

    STDMETHOD(RegisterSoftwareDevice)(void* pInitializeFunction)
    {
        return m_pD3D9->RegisterSoftwareDevice(pInitializeFunction);
    }

    STDMETHOD_(UINT, GetAdapterCount)()
    {
        return m_pD3D9->GetAdapterCount();
    }

    STDMETHOD(GetAdapterIdentifier)(UINT Adapter, DWORD Flags, void* pIdentifier)
    {
        return m_pD3D9->GetAdapterIdentifier(Adapter, Flags, (D3DADAPTER_IDENTIFIER9*)pIdentifier);
    }

    STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter)
    {
        return m_pD3D9->GetAdapterModeCount(Adapter, D3DFMT_UNKNOWN);
    }

    STDMETHOD(EnumAdapterModes)(UINT Adapter, UINT Mode, void* pMode)
    {
        return m_pD3D9->EnumAdapterModes(Adapter, D3DFMT_UNKNOWN, Mode, (D3DDISPLAYMODE*)pMode);
    }

    STDMETHOD(GetAdapterDisplayMode)(UINT Adapter, void* pMode)
    {
        return m_pD3D9->GetAdapterDisplayMode(Adapter, (D3DDISPLAYMODE*)pMode);
    }

    STDMETHOD(CheckDeviceType)(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed)
    {
        return m_pD3D9->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
    }

    STDMETHOD(CheckDeviceFormat)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
    {
        return m_pD3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
    }

    STDMETHOD(CheckDeviceMultiSampleType)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType)
    {
        return m_pD3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, nullptr);
    }

    STDMETHOD(CheckDepthStencilMatch)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
    {
        return m_pD3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
    }

    STDMETHOD(GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, void* pCaps)
    {
        D3DCAPS9 caps9;
        return m_pD3D9->GetDeviceCaps(Adapter, DeviceType, &caps9);
    }

    STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter)
    {
        return m_pD3D9->GetAdapterMonitor(Adapter);
    }

    STDMETHOD(CreateDevice)(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, void* pPresentationParameters, void** ppReturnedDeviceInterface)
    {
        if (ppReturnedDeviceInterface == nullptr)
            return D3DERR_INVALIDCALL;

        D3DPRESENT_PARAMETERS pp9 = {0};
        if (pPresentationParameters)
        {
            D3DPRESENT_PARAMETERS* pp8 = (D3DPRESENT_PARAMETERS*)pPresentationParameters;
            pp9.BackBufferWidth = pp8->BackBufferWidth;
            pp9.BackBufferHeight = pp8->BackBufferHeight;
            pp9.BackBufferFormat = pp8->BackBufferFormat;
            pp9.BackBufferCount = pp8->BackBufferCount;
            pp9.MultiSampleType = pp8->MultiSampleType;
            pp9.MultiSampleQuality = pp8->MultiSampleQuality;
            pp9.SwapEffect = pp8->SwapEffect;
            pp9.hDeviceWindow = pp8->hDeviceWindow;
            pp9.Windowed = pp8->Windowed;
            pp9.EnableAutoDepthStencil = pp8->EnableAutoDepthStencil;
            pp9.AutoDepthStencilFormat = pp8->AutoDepthStencilFormat;
            pp9.Flags = pp8->Flags;
            pp9.FullScreen_RefreshRateInHz = pp8->FullScreen_RefreshRateInHz;
            pp9.PresentationInterval = Config::g_Config.EnableVSync ? pp8->PresentationInterval : D3DPRESENT_INTERVAL_IMMEDIATE;
        }

        IDirect3DDevice9* pDevice9 = nullptr;
        HRESULT hr = m_pD3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, &pp9, &pDevice9);

        if (SUCCEEDED(hr) && pDevice9)
        {
            *ppReturnedDeviceInterface = new CDirect3DDevice8(pDevice9);
            pDevice9->Release();
            Config::Log("[OFP-D3DX] Device created with wrapper (FPS: %lu, FOV: %lu)\n",
                Config::g_Config.FPSUpdateInterval, Config::g_Config.DefaultFOV);
        }

        return hr;
    }
};

extern "C" __declspec(dllexport) IDirect3D8* CreateD3D8Wrapper(IDirect3D9* pD3D9)
{
    if (pD3D9 == nullptr)
        return nullptr;

    Config::Load();

    Config::Log("[OFP-D3DX] CreateD3D8Wrapper called\n");
    Config::Log("[OFP-D3DX] Anisotropy: %lu, VSync: %d, RemoveNightFilter: %d\n",
        Config::g_Config.AnisotropicFiltering,
        Config::g_Config.EnableVSync,
        Config::g_Config.RemoveNightFilter);

    return new CDirect3D8(pD3D9);
}
