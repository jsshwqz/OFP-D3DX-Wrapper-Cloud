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

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) override
    {
        return m_pDevice9->QueryInterface(riid, ppvObj);
    }

    STDMETHOD_(ULONG, AddRef)(THIS) override
    {
        return InterlockedIncrement(&g_RefCount);
    }

    STDMETHOD_(ULONG, Release)(THIS) override
    {
        ULONG RefCount = InterlockedDecrement(&g_RefCount);
        if (RefCount == 0)
            delete this;
        return RefCount;
    }

    STDMETHOD(TestCooperativeLevel)(THIS) override
    {
        return m_pDevice9->TestCooperativeLevel();
    }

    STDMETHOD_(UINT, GetAvailableTextureMem)(THIS) override
    {
        return m_pDevice9->GetAvailableTextureMem();
    }

    STDMETHOD(EvictManagedTextures)(THIS) override
    {
        return m_pDevice9->EvictManagedTextures();
    }

    STDMETHOD(GetDirect3D)(THIS_ IDirect3D9** ppD3D9) override
    {
        return m_pDevice9->GetDirect3D(ppD3D9);
    }

    STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS9* pCaps) override
    {
        return m_pDevice9->GetDeviceCaps(pCaps);
    }

    STDMETHOD(GetDisplayMode)(THIS_ D3DDISPLAYMODE* pMode) override
    {
        return m_pDevice9->GetDisplayMode(pMode);
    }

    STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS* pParameters) override
    {
        return m_pDevice9->GetCreationParameters(pParameters);
    }

    STDMETHOD(SetCursorProperties)(THIS_ UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) override
    {
        return m_pDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
    }

    STDMETHOD_(void, SetCursorPosition)(THIS_ int X, int Y, DWORD Flags) override
    {
        m_pDevice9->SetCursorPosition(X, Y, Flags);
    }

    STDMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow) override
    {
        return m_pDevice9->ShowCursor(bShow);
    }

    STDMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) override
    {
        return m_pDevice9->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
    }

    STDMETHOD(GetSwapChain)(THIS_ UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) override
    {
        return m_pDevice9->GetSwapChain(iSwapChain, pSwapChain);
    }

    STDMETHOD_(UINT, GetNumberOfSwapChains)(THIS) override
    {
        return m_pDevice9->GetNumberOfSwapChains();
    }

    STDMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) override
    {
        return m_pDevice9->Reset(pPresentationParameters);
    }

    STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) override
    {
        return m_pDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    }

    STDMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) override
    {
        return m_pDevice9->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
    }

    STDMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) override
    {
        return m_pDevice9->GetRasterStatus(iSwapChain, pRasterStatus);
    }

    STDMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) override
    {
        return m_pDevice9->SetDialogBoxMode(bEnableDialogs);
    }

    STDMETHOD(SetGammaRamp)(THIS_ UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp) override
    {
        return m_pDevice9->SetGammaRamp(iSwapChain, Flags, pRamp);
    }

    STDMETHOD(GetGammaRamp)(THIS_ UINT iSwapChain, D3DGAMMARAMP* pRamp) override
    {
        return m_pDevice9->GetGammaRamp(iSwapChain, pRamp);
    }

    STDMETHOD(CreateTexture)(THIS_ UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) override
    {
        return m_pDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
    }

    STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) override
    {
        return m_pDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
    }

    STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) override
    {
        return m_pDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
    }

    STDMETHOD(CreateTextureFromFile)(THIS_ LPCSTR pSrcFile, IDirect3DTexture9** ppTexture) override
    {
        return m_pDevice9->CreateTextureFromFile(pSrcFile, ppTexture);
    }

    STDMETHOD(CreateTextureFromFileEx)(THIS_ LPCSTR pSrcFile, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, IDirect3DTexture9** ppTexture) override
    {
        return m_pDevice9->CreateTextureFromFileEx(pSrcFile, Width, Height, Levels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
    }

    STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) override
    {
        return m_pDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
    }

    STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) override
    {
        return m_pDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
    }

    STDMETHOD(CreateRenderTarget)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) override
    {
        return m_pDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
    }

    STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) override
    {
        return m_pDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
    }

    STDMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) override
    {
        return m_pDevice9->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
    }

    STDMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pElements, IDirect3DVertexDeclaration9** ppDecl) override
    {
        return m_pDevice9->CreateVertexDeclaration(pElements, ppDecl);
    }

    STDMETHOD(CreateVertexShader)(THIS_ CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader) override
    {
        return m_pDevice9->CreateVertexShader(pFunction, ppShader);
    }

    STDMETHOD(CreatePixelShader)(THIS_ CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader) override
    {
        return m_pDevice9->CreatePixelShader(pFunction, ppShader);
    }

    STDMETHOD(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl) override
    {
        return m_pDevice9->SetVertexDeclaration(pDecl);
    }

    STDMETHOD(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl) override
    {
        return m_pDevice9->GetVertexDeclaration(ppDecl);
    }

    STDMETHOD(SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader) override
    {
        return m_pDevice9->SetVertexShader(pShader);
    }

    STDMETHOD(GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader) override
    {
        return m_pDevice9->GetVertexShader(ppShader);
    }

    STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) override
    {
        return m_pDevice9->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) override
    {
        return m_pDevice9->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) override
    {
        return m_pDevice9->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) override
    {
        return m_pDevice9->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount) override
    {
        return m_pDevice9->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) override
    {
        return m_pDevice9->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader) override
    {
        return m_pDevice9->SetPixelShader(pShader);
    }

    STDMETHOD(GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader) override
    {
        return m_pDevice9->GetPixelShader(ppShader);
    }

    STDMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) override
    {
        return m_pDevice9->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) override
    {
        return m_pDevice9->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) override
    {
        return m_pDevice9->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) override
    {
        return m_pDevice9->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount) override
    {
        return m_pDevice9->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) override
    {
        return m_pDevice9->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) override
    {
        return m_pDevice9->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
    }

    STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) override
    {
        return m_pDevice9->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
    }

    STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) override
    {
        return m_pDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    }

    STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) override
    {
        return m_pDevice9->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
    }

    STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) override
    {
        return m_pDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
    }

    STDMETHOD(SetVertexStream)(THIS_ UINT Stream, IDirect3DVertexBuffer9* pStreamData, UINT Stride) override
    {
        return m_pDevice9->SetVertexStream(Stream, pStreamData, Stride);
    }

    STDMETHOD(GetVertexStream)(THIS_ UINT Stream, IDirect3DVertexBuffer9** ppStreamData, UINT* pStride) override
    {
        return m_pDevice9->GetVertexStream(Stream, ppStreamData, pStride);
    }

    STDMETHOD(SetVertexStreamFreq)(THIS_ UINT Stream, DWORD Frequency) override
    {
        return m_pDevice9->SetVertexStreamFreq(Stream, Frequency);
    }

    STDMETHOD(GetVertexStreamFreq)(THIS_ UINT Stream, DWORD* pFrequency) override
    {
        return m_pDevice9->GetVertexStreamFreq(Stream, pFrequency);
    }

    STDMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData) override
    {
        return m_pDevice9->SetIndices(pIndexData);
    }

    STDMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData) override
    {
        return m_pDevice9->GetIndices(ppIndexData);
    }

    STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport) override
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

    STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT9* pViewport) override
    {
        return m_pDevice9->GetViewport(pViewport);
    }

    STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) override
    {
        return m_pDevice9->SetMaterial(pMaterial);
    }

    STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) override
    {
        return m_pDevice9->GetMaterial(pMaterial);
    }

    STDMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT9* pLight) override
    {
        return m_pDevice9->SetLight(Index, pLight);
    }

    STDMETHOD(GetLight)(THIS_ DWORD Index, D3DLIGHT9* pLight) override
    {
        return m_pDevice9->GetLight(Index, pLight);
    }

    STDMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable) override
    {
        return m_pDevice9->LightEnable(Index, Enable);
    }

    STDMETHOD(GetLightEnable)(THIS_ DWORD Index, BOOL* pEnable) override
    {
        return m_pDevice9->GetLightEnable(Index, pEnable);
    }

    STDMETHOD(SetClipPlane)(THIS_ DWORD Index, CONST float* pPlane) override
    {
        return m_pDevice9->SetClipPlane(Index, pPlane);
    }

    STDMETHOD(GetClipPlane)(THIS_ DWORD Index, float* pPlane) override
    {
        return m_pDevice9->GetClipPlane(Index, pPlane);
    }

    STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD Value) override
    {
        if (State == D3DRS_ZENABLE && Config::g_Config.RemoveNightFilter)
        {
            Config::Log("[OFP-D3DX] Night filter disabled\n");
        }
        return m_pDevice9->SetRenderState(State, Value);
    }

    STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD* pValue) override
    {
        return m_pDevice9->GetRenderState(State, pValue);
    }

    STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) override
    {
        return m_pDevice9->CreateStateBlock(Type, ppSB);
    }

    STDMETHOD(BeginStateBlock)(THIS) override
    {
        return m_pDevice9->BeginStateBlock();
    }

    STDMETHOD(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB) override
    {
        return m_pDevice9->EndStateBlock(ppSB);
    }

    STDMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) override
    {
        return m_pDevice9->SetClipStatus(pClipStatus);
    }

    STDMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) override
    {
        return m_pDevice9->GetClipStatus(pClipStatus);
    }

    STDMETHOD(GetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9** ppTexture) override
    {
        return m_pDevice9->GetTexture(Stage, ppTexture);
    }

    STDMETHOD(SetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9* pTexture) override
    {
        return m_pDevice9->SetTexture(Stage, pTexture);
    }

    STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) override
    {
        return m_pDevice9->GetTextureStageState(Stage, Type, pValue);
    }

    STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) override
    {
        if (Type == D3DTSS_MINFILTER && Config::g_Config.AnisotropicFiltering > 0)
        {
            return m_pDevice9->SetTextureStageState(Stage, Type, D3DTEXF_ANISOTROPIC);
        }
        return m_pDevice9->SetTextureStageState(Stage, Type, Value);
    }

    STDMETHOD(GetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) override
    {
        return m_pDevice9->GetSamplerState(Sampler, Type, pValue);
    }

    STDMETHOD(SetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) override
    {
        if (Type == D3DSAMP_MAXANISOTROPY && Config::g_Config.AnisotropicFiltering > 0)
        {
            return m_pDevice9->SetSamplerState(Sampler, Type, Config::g_Config.AnisotropicFiltering);
        }
        return m_pDevice9->SetSamplerState(Sampler, Type, Value);
    }

    STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect) override
    {
        return m_pDevice9->SetScissorRect(pRect);
    }

    STDMETHOD(GetScissorRect)(THIS_ RECT* pRect) override
    {
        return m_pDevice9->GetScissorRect(pRect);
    }

    STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) override
    {
        return m_pDevice9->SetSoftwareVertexProcessing(bSoftware);
    }

    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS) override
    {
        return m_pDevice9->GetSoftwareVertexProcessing();
    }

    STDMETHOD(SetNPatchMode)(THIS_ float nSegments) override
    {
        return m_pDevice9->SetNPatchMode(nSegments);
    }

    STDMETHOD_(float, GetNPatchMode)(THIS) override
    {
        return m_pDevice9->GetNPatchMode();
    }

    STDMETHOD(DrawIndexedPrimitiveStrided)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT startIndex, UINT primCount) override
    {
        return m_pDevice9->DrawIndexedPrimitiveStrided(PrimitiveType, BaseVertexIndex, MinIndex, NumVertices, startIndex, primCount);
    }

    STDMETHOD(SetTextureStageStateOld)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) override
    {
        return m_pDevice9->SetTextureStageStateOld(Stage, Type, Value);
    }

    STDMETHOD(GetTextureStageStateOld)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) override
    {
        return m_pDevice9->GetTextureStageStateOld(Stage, Type, pValue);
    }

    STDMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses) override
    {
        return m_pDevice9->ValidateDevice(pNumPasses);
    }

    STDMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber, CONST PALETTEENTRY* pEntries) override
    {
        return m_pDevice9->SetPaletteEntries(PaletteNumber, pEntries);
    }

    STDMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber, PALETTEENTRY* pEntries) override
    {
        return m_pDevice9->GetPaletteEntries(PaletteNumber, pEntries);
    }

    STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) override
    {
        return m_pDevice9->SetCurrentTexturePalette(PaletteNumber);
    }

    STDMETHOD_(UINT, GetCurrentTexturePalette)(THIS) override
    {
        return m_pDevice9->GetCurrentTexturePalette();
    }

    STDMETHOD(SetScissorRectOld)(THIS_ CONST RECT* pRect) override
    {
        return m_pDevice9->SetScissorRectOld(pRect);
    }

    STDMETHOD(GetScissorRectOld)(THIS_ RECT* pRect) override
    {
        return m_pDevice9->GetScissorRectOld(pRect);
    }

    STDMETHOD(ShowCursor)(THIS_ BOOL bShow, BOOL bUseAlpha) override
    {
        return m_pDevice9->ShowCursor(bShow, bUseAlpha);
    }

    STDMETHOD(GetGammaRampOld)(THIS_ D3DGAMMARAMP* pRamp) override
    {
        return m_pDevice9->GetGammaRampOld(pRamp);
    }

    STDMETHOD(SetGammaRampOld)(THIS_ DWORD Flags, CONST D3DGAMMARAMP* pRamp) override
    {
        return m_pDevice9->SetGammaRampOld(Flags, pRamp);
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

    STDMETHOD(EndScene)(THIS) override
    {
        UpdateFPS();
        return m_pDevice9->EndScene();
    }

    STDMETHOD(BeginScene)(THIS) override
    {
        return m_pDevice9->BeginScene();
    }

    STDMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) override
    {
        return m_pDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil);
    }

    STDMETHOD(ColorFill)(THIS_ IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR Color) override
    {
        return m_pDevice9->ColorFill(pSurface, pRect, Color);
    }

    STDMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) override
    {
        return m_pDevice9->CreateQuery(Type, ppQuery);
    }

    STDMETHOD(DeletePatch)(THIS_ UINT Handle) override
    {
        return m_pDevice9->DeletePatch(Handle);
    }

    STDMETHOD(DrawRectPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pPatchInfo) override
    {
        return m_pDevice9->DrawRectPatch(Handle, pNumSegs, pPatchInfo);
    }

    STDMETHOD(DrawTriPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pPatchInfo) override
    {
        return m_pDevice9->DrawTriPatch(Handle, pNumSegs, pPatchInfo);
    }

    STDMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain, IDirect3DSurface9* pDestSurface) override
    {
        return m_pDevice9->GetFrontBufferData(iSwapChain, pDestSurface);
    }

    STDMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) override
    {
        return m_pDevice9->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
    }

    STDMETHOD(SetRenderTarget)(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) override
    {
        return m_pDevice9->SetRenderTarget(RenderTargetIndex, pRenderTarget);
    }

    STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppDepthStencilSurface) override
    {
        return m_pDevice9->GetDepthStencilSurface(ppDepthStencilSurface);
    }

    STDMETHOD(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil) override
    {
        return m_pDevice9->SetDepthStencilSurface(pNewZStencil);
    }

    STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) override
    {
        return m_pDevice9->GetTransform(State, pMatrix);
    }

    STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) override
    {
        return m_pDevice9->SetTransform(State, pMatrix);
    }

    STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) override
    {
        return m_pDevice9->MultiplyTransform(State, pMatrix);
    }

    STDMETHOD(GetFVF)(THIS_ DWORD* pFVF) override
    {
        return m_pDevice9->GetFVF(pFVF);
    }

    STDMETHOD(SetFVF)(THIS_ DWORD FVF) override
    {
        return m_pDevice9->SetFVF(FVF);
    }

    STDMETHOD(GetStreamSource)(THIS_ UINT Stream, IDirect3DVertexBuffer9** ppStreamData, UINT* pStride) override
    {
        return m_pDevice9->GetStreamSource(Stream, ppStreamData, pStride);
    }

    STDMETHOD(SetStreamSource)(THIS_ UINT Stream, IDirect3DVertexBuffer9* pStreamData, UINT Stride) override
    {
        return m_pDevice9->SetStreamSource(Stream, pStreamData, Stride);
    }

    STDMETHOD(GetStreamSourceFreq)(THIS_ UINT Stream, DWORD* pFrequency) override
    {
        return m_pDevice9->GetStreamSourceFreq(Stream, pFrequency);
    }

    STDMETHOD(SetStreamSourceFreq)(THIS_ UINT Stream, DWORD Frequency) override
    {
        return m_pDevice9->SetStreamSourceFreq(Stream, Frequency);
    }

    STDMETHOD(GetCurrentTexturePalette)(THIS_ UINT* pPaletteNumber) override
    {
        return m_pDevice9->GetCurrentTexturePalette(pPaletteNumber);
    }

    STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) override
    {
        return m_pDevice9->SetCurrentTexturePalette(PaletteNumber);
    }

    STDMETHOD(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) override
    {
        return m_pDevice9->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
    }

    STDMETHOD(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestinationPoint) override
    {
        return m_pDevice9->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestinationPoint);
    }

    STDMETHOD(UpdateTexture)(THIS_ IDirect3DSurface9* pSourceTexture, IDirect3DSurface9* pDestinationTexture) override
    {
        return m_pDevice9->UpdateTexture(pSourceTexture, pDestinationTexture);
    }

    STDMETHOD(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) override
    {
        return m_pDevice9->GetRenderTargetData(pRenderTarget, pDestSurface);
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

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) override
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

    STDMETHOD_(ULONG, AddRef)(THIS) override
    {
        return InterlockedIncrement(&g_RefCount);
    }

    STDMETHOD_(ULONG, Release)(THIS) override
    {
        ULONG RefCount = InterlockedDecrement(&g_RefCount);
        if (RefCount == 0)
            delete this;
        return RefCount;
    }

    STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction) override
    {
        return m_pD3D9->RegisterSoftwareDevice(pInitializeFunction);
    }

    STDMETHOD_(UINT, GetAdapterCount)(THIS) override
    {
        return m_pD3D9->GetAdapterCount();
    }

    STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter, DWORD Flags, void* pIdentifier) override
    {
        return m_pD3D9->GetAdapterIdentifier(Adapter, Flags, (D3DADAPTER_IDENTIFIER9*)pIdentifier);
    }

    STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter) override
    {
        return m_pD3D9->GetAdapterModeCount(Adapter, D3DFMT_UNKNOWN);
    }

    STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter, UINT Mode, void* pMode) override
    {
        return m_pD3D9->EnumAdapterModes(Adapter, D3DFMT_UNKNOWN, Mode, (D3DDISPLAYMODE*)pMode);
    }

    STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter, void* pMode) override
    {
        return m_pD3D9->GetAdapterDisplayMode(Adapter, (D3DDISPLAYMODE*)pMode);
    }

    STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) override
    {
        return m_pD3D9->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
    }

    STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) override
    {
        return m_pD3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
    }

    STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType) override
    {
        return m_pD3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, nullptr);
    }

    STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) override
    {
        return m_pD3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
    }

    STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, void* pCaps) override
    {
        D3DCAPS9 caps9;
        return m_pD3D9->GetDeviceCaps(Adapter, DeviceType, &caps9);
    }

    STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter) override
    {
        return m_pD3D9->GetAdapterMonitor(Adapter);
    }

    STDMETHOD(CreateDevice)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, void* pPresentationParameters, void** ppReturnedDeviceInterface) override
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
