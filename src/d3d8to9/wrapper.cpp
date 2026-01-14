#include "config.hpp"
#include "hooks.hpp"
#include <d3d9.h>
#include <initguid.h>

DEFINE_GUID(IID_IDirect3D8, 0x1DD9E8DA, 0x6C27, 0x41C8, 0xA7, 0xE7, 0x76, 0x15, 0xEE, 0xF7, 0x5C, 0x55);

static LONG g_RefCount = 0;

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
            Config::Log("[OFP-D3DX] Device created, installing hooks...\n");
            Hooks::Install(pDevice9);
            *ppReturnedDeviceInterface = pDevice9;
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
    Config::Log("[OFP-D3DX] Anisotropy: %lu, VSync: %d\n",
        Config::g_Config.AnisotropicFiltering,
        Config::g_Config.EnableVSync);

    return new CDirect3D8(pD3D9);
}
