#include "config.hpp"
#include "d3d8-wrapper.hpp"
#include <stdio.h>
#include <string.h>
#include <initguid.h>

static LONG g_RefCount = 0;

typedef struct _D3DCAPS8
{
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

class CDirect3D8 : public IUnknown
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

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
    {
        if (ppvObj == nullptr)
            return E_INVALIDARG;

        if (riid == IID_IUnknown || riid == __uuidof(IDirect3D8))
        {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)(THIS)
    {
        return InterlockedIncrement(&g_RefCount);
    }

    STDMETHOD_(ULONG, Release)(THIS)
    {
        ULONG RefCount = InterlockedDecrement(&g_RefCount);
        if (RefCount == 0)
            delete this;
        return RefCount;
    }

    STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction)
    {
        return m_pD3D9->RegisterSoftwareDevice(pInitializeFunction);
    }

    STDMETHOD_(UINT, GetAdapterCount)(THIS)
    {
        return m_pD3D9->GetAdapterCount();
    }

    STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
    {
        return m_pD3D9->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
    }

    STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter)
    {
        return m_pD3D9->GetAdapterModeCount(Adapter);
    }

    STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode)
    {
        return m_pD3D9->EnumAdapterModes(Adapter, Mode, pMode);
    }

    STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter, D3DDISPLAYMODE* pMode)
    {
        return m_pD3D9->GetAdapterDisplayMode(Adapter, pMode);
    }

    STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed)
    {
        return m_pD3D9->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
    }

    STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
    {
        return m_pD3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
    }

    STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType)
    {
        return m_pD3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType);
    }

    STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
    {
        return m_pD3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
    }

    STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8* pCaps)
    {
        D3DCAPS9 caps9;
        HRESULT hr = m_pD3D9->GetDeviceCaps(Adapter, DeviceType, &caps9);
        if (SUCCEEDED(hr) && pCaps)
        {
            memset(pCaps, 0, sizeof(D3DCAPS8));
            pCaps->Caps = caps9.Caps;
            pCaps->Caps2 = caps9.Caps2;
            pCaps->Caps3 = caps9.Caps3;
            pCaps->Caps4 = caps9.Caps4;
            pCaps->PresentationIntervals = caps9.PresentationIntervals;
            pCaps->CursorCaps = caps9.CursorCaps;
            pCaps->TextureCaps = caps9.TextureCaps;
            pCaps->TextureFilterCaps = caps9.TextureFilterCaps;
            pCaps->MaxTextureWidth = caps9.MaxTextureWidth;
            pCaps->MaxTextureHeight = caps9.MaxTextureHeight;
            pCaps->MaxAnisotropy = caps9.MaxAnisotropy;
            pCaps->MaxVertexW = caps9.MaxVertexW;
            pCaps->FVFCaps = caps9.FVFCaps;
            pCaps->TextureOpCaps = caps9.TextureOpCaps;
            pCaps->MaxTextureBlendStages = caps9.MaxTextureBlendStages;
            pCaps->MaxSimultaneousTextures = caps9.MaxSimultaneousTextures;
            pCaps->VertexProcessingCaps = caps9.VertexProcessingCaps;
            pCaps->MaxActiveLights = caps9.MaxActiveLights;
            pCaps->MaxUserClipPlanes = caps9.MaxUserClipPlanes;
            pCaps->MaxVertexBlendMatrices = caps9.MaxVertexBlendMatrices;
            pCaps->MaxPointSize = caps9.MaxPointSize;
            pCaps->MaxPrimitiveCount = caps9.MaxPrimitiveCount;
            pCaps->MaxVertexIndex = caps9.MaxVertexIndex;
            pCaps->MaxStreams = caps9.MaxStreams;
            pCaps->MaxStreamStride = caps9.MaxStreamStride;
            pCaps->VertexShaderVersion = caps9.VertexShaderVersion;
            pCaps->MaxVertexShaderConst = caps9.MaxVertexShaderConst;
            pCaps->PixelShaderVersion = caps9.PixelShaderVersion;
            pCaps->MaxPixelShaderValue = caps9.MaxPixelShaderValue;
        }
        return hr;
    }

    STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter)
    {
        return m_pD3D9->GetAdapterMonitor(Adapter);
    }

    STDMETHOD(CreateDevice)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, void* pPresentationParameters, void** ppReturnedDeviceInterface);
};

class CDirect3DDevice8 : public IUnknown
{
private:
    IDirect3DDevice9* m_pDevice9;
    CDirect3D8* m_pD3D8;
    LONG m_RefCount;

public:
    CDirect3DDevice8(IDirect3DDevice9* pDevice9, CDirect3D8* pD3D8)
        : m_pDevice9(pDevice9), m_pD3D8(pD3D8), m_RefCount(1)
    {
        if (m_pD3D8)
            m_pD3D8->AddRef();
        if (m_pDevice9)
            m_pDevice9->AddRef();
    }

    virtual ~CDirect3DDevice8()
    {
        if (m_pDevice9)
            m_pDevice9->Release();
        if (m_pD3D8)
            m_pD3D8->Release();
    }

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
    {
        if (ppvObj == nullptr)
            return E_INVALIDARG;

        if (riid == IID_IUnknown || riid == __uuidof(IDirect3DDevice8))
        {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)(THIS)
    {
        return InterlockedIncrement(&m_RefCount);
    }

    STDMETHOD_(ULONG, Release)(THIS)
    {
        ULONG RefCount = InterlockedDecrement(&m_RefCount);
        if (RefCount == 0)
            delete this;
        return RefCount;
    }

    STDMETHOD(TestCooperativeLevel)(THIS)
    {
        return m_pDevice9->TestCooperativeLevel();
    }

    STDMETHOD_(UINT, GetAvailableTextureMem)(THIS)
    {
        return m_pDevice9->GetAvailableTextureMem();
    }

    STDMETHOD(ResourceManagerDiscardBytes)(THIS_ DWORD Bytes)
    {
        return m_pDevice9->EvictManagedResources();
    }

    STDMETHOD(GetDirect3D)(THIS_ IDirect3D8** ppD3D9)
    {
        if (ppD3D9 == nullptr)
            return D3DERR_INVALIDCALL;
        m_pD3D8->AddRef();
        *ppD3D9 = m_pD3D8;
        return D3D_OK;
    }

    STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS8* pCaps)
    {
        D3DCAPS9 caps9;
        HRESULT hr = m_pDevice9->GetDeviceCaps(&caps9);
        if (SUCCEEDED(hr) && pCaps)
        {
            memset(pCaps, 0, sizeof(D3DCAPS8));
            pCaps->Caps = caps9.Caps;
            pCaps->Caps2 = caps9.Caps2;
            pCaps->Caps3 = caps9.Caps3;
            pCaps->MaxTextureWidth = caps9.MaxTextureWidth;
            pCaps->MaxTextureHeight = caps9.MaxTextureHeight;
            pCaps->MaxAnisotropy = caps9.MaxAnisotropy;
            pCaps->VertexProcessingCaps = caps9.VertexProcessingCaps;
            pCaps->MaxActiveLights = caps9.MaxActiveLights;
            pCaps->MaxUserClipPlanes = caps9.MaxUserClipPlanes;
            pCaps->MaxVertexBlendMatrices = caps9.MaxVertexBlendMatrices;
            pCaps->MaxPointSize = caps9.MaxPointSize;
            pCaps->MaxPrimitiveCount = caps9.MaxPrimitiveCount;
            pCaps->MaxVertexIndex = caps9.MaxVertexIndex;
            pCaps->VertexShaderVersion = caps9.VertexShaderVersion;
            pCaps->PixelShaderVersion = caps9.PixelShaderVersion;
        }
        return hr;
    }

    STDMETHOD(GetDisplayMode)(THIS_ D3DDISPLAYMODE* pMode)
    {
        return m_pDevice9->GetDisplayMode(pMode);
    }

    STDMETHOD(SetRenderState)(THIS_ DWORD State, DWORD Value)
    {
        return m_pDevice9->SetRenderState((D3DRENDERSTATETYPE)State, Value);
    }

    STDMETHOD(GetRenderState)(THIS_ DWORD State, DWORD* pValue)
    {
        return m_pDevice9->GetRenderState((D3DRENDERSTATETYPE)State, pValue);
    }

    STDMETHOD(BeginScene)(THIS)
    {
        return m_pDevice9->BeginScene();
    }

    STDMETHOD(EndScene)(THIS)
    {
        return m_pDevice9->EndScene();
    }

    STDMETHOD(Clear)(THIS_ DWORD Count, void* pRects, DWORD Flags, DWORD Color, float Z, DWORD Stencil)
    {
        return m_pDevice9->Clear(Count, (D3DRECT*)pRects, Flags, Color, Z, Stencil);
    }

    STDMETHOD(SetViewport)(THIS_ void* pViewport)
    {
        D3DVIEWPORT9 vp;
        vp.X = ((D3DVIEWPORT8*)pViewport)->X;
        vp.Y = ((D3DVIEWPORT8*)pViewport)->Y;
        vp.Width = ((D3DVIEWPORT8*)pViewport)->Width;
        vp.Height = ((D3DVIEWPORT8*)pViewport)->Height;
        vp.MinZ = ((D3DVIEWPORT8*)pViewport)->MinZ;
        vp.MaxZ = ((D3DVIEWPORT8*)pViewport)->MaxZ;
        return m_pDevice9->SetViewport(&vp);
    }

    STDMETHOD(SetTexture)(THIS_ DWORD Stage, void* pTexture)
    {
        if (Config::g_Config.AnisotropicFiltering > 1 && pTexture != nullptr)
        {
            m_pDevice9->SetSamplerState(Stage, D3DSAMP_MAXANISOTROPY, Config::g_Config.AnisotropicFiltering);
            m_pDevice9->SetSamplerState(Stage, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
            m_pDevice9->SetSamplerState(Stage, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
        }
        return m_pDevice9->SetTexture(Stage, (IDirect3DBaseTexture9*)pTexture);
    }

    STDMETHOD(DrawPrimitive)(THIS_ DWORD PrimitiveType, DWORD StartVertex, DWORD PrimitiveCount)
    {
        return m_pDevice9->DrawPrimitive((D3DPRIMITIVETYPE)PrimitiveType, StartVertex, PrimitiveCount);
    }

    STDMETHOD(DrawIndexedPrimitive)(THIS_ DWORD PrimitiveType, DWORD MinIndex, DWORD NumVertices, DWORD StartIndex, DWORD PrimitiveCount)
    {
        return m_pDevice9->DrawIndexedPrimitive((D3DPRIMITIVETYPE)PrimitiveType, MinIndex, NumVertices, StartIndex, PrimitiveCount);
    }

    STDMETHOD(SetVertexShader)(THIS_ DWORD Handle)
    {
        return m_pDevice9->SetVertexShader(Handle);
    }

    STDMETHOD_(DWORD, GetVertexShader)(THIS)
    {
        return m_pDevice9->GetVertexShader();
    }

    STDMETHOD(SetPixelShader)(THIS_ DWORD Handle)
    {
        return m_pDevice9->SetPixelShader(Handle);
    }

    STDMETHOD_(DWORD, GetPixelShader)(THIS)
    {
        return m_pDevice9->GetPixelShader();
    }
};

STDMETHODIMP CDirect3D8::CreateDevice(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, void* pPresentationParameters, void** ppReturnedDeviceInterface)
{
    if (ppReturnedDeviceInterface == nullptr)
        return D3DERR_INVALIDCALL;

    D3DPRESENT_PARAMETERS pp9;
    memset(&pp9, 0, sizeof(pp9));
    pp9.BackBufferWidth = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->BackBufferWidth;
    pp9.BackBufferHeight = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->BackBufferHeight;
    pp9.BackBufferFormat = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->BackBufferFormat;
    pp9.BackBufferCount = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->BackBufferCount;
    pp9.MultiSampleType = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->MultiSampleType;
    pp9.SwapEffect = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->SwapEffect;
    pp9.hDeviceWindow = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->hDeviceWindow;
    pp9.Windowed = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->Windowed;
    pp9.EnableAutoDepthStencil = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->EnableAutoDepthStencil;
    pp9.AutoDepthStencilFormat = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->AutoDepthStencilFormat;
    pp9.Flags = ((D3DPRESENT_PARAMETERS*)pPresentationParameters)->Flags;
    pp9.FullScreen_RefreshRateInHz = 0;
    pp9.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

    if (Config::g_Config.EnableVSync)
        pp9.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    else
        pp9.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    IDirect3DDevice9* pDevice9 = nullptr;
    HRESULT hr = m_pD3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, &pp9, &pDevice9);
    if (SUCCEEDED(hr) && pDevice9)
    {
        *ppReturnedDeviceInterface = new CDirect3DDevice8(pDevice9, this);
        pDevice9->Release();
    }
    return hr;
}

IDirect3D8* CreateD3D8Wrapper(IDirect3D9* pD3D9)
{
    if (pD3D9 == nullptr)
        return nullptr;
    return new CDirect3D8(pD3D9);
}
