#include "config.hpp"
#include <d3d9.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

static HMODULE g_hRealD3D8 = nullptr;
static BOOL g_bInitialized = FALSE;
static BOOL g_bUsingD3D9 = FALSE;
static IDirect3D9* g_pD3D9 = nullptr;

typedef IDirect3D8* (WINAPI* Direct3DCreate8_t)(UINT SDKVersion);
static Direct3DCreate8_t g_pRealDirect3DCreate8 = nullptr;

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (!g_bInitialized)
        {
            g_bInitialized = TRUE;
            Config::Load();
            Config::Log("OFP D3DX Wrapper: Loaded\n");

            if (Config::g_Config.EnableWrapper && Config::g_Config.D3d8to9)
            {
                Config::Log("OFP D3DX Wrapper: Attempting D3D9 initialization...\n");
                g_pD3D9 = Direct3DCreate9(220);
                if (g_pD3D9)
                {
                    g_bUsingD3D9 = TRUE;
                    Config::Log("OFP D3DX Wrapper: D3D9 initialized successfully\n");
                }
                else
                {
                    Config::Log("OFP D3DX Wrapper: D3D9 failed, using fallback\n");
                }
            }
        }
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        Config::Log("OFP D3DX Wrapper: Unloading...\n");
        if (g_pD3D9)
        {
            g_pD3D9->Release();
            g_pD3D9 = nullptr;
        }
    }
    return TRUE;
}

extern "C" __declspec(dllexport) IDirect3D8* WINAPI Direct3DCreate8(UINT SDKVersion)
{
    Config::Log("OFP D3DX Wrapper: Direct3DCreate8(SDKVersion=%u)\n", SDKVersion);

    if (!Config::g_Config.EnableWrapper || !Config::g_Config.D3d8to9)
    {
        char sysPath[MAX_PATH];
        GetSystemDirectoryA(sysPath, MAX_PATH);
        strcat(sysPath, "\\d3d8.dll");
        HMODULE hD3D8 = LoadLibraryA(sysPath);
        if (hD3D8)
        {
            Direct3DCreate8_t pCreate = (Direct3DCreate8_t)GetProcAddress(hD3D8, "Direct3DCreate8");
            if (pCreate)
            {
                IDirect3D8* pD3D8 = pCreate(SDKVersion);
                Config::Log("OFP D3DX Wrapper: Using real d3d8.dll (fallback)\n");
                return pD3D8;
            }
        }
        return nullptr;
    }

    if (g_bUsingD3D9 && g_pD3D9)
    {
        Config::Log("OFP D3DX Wrapper: Using D3D9 path - creating simple wrapper\n");

        struct SimpleD3D8 : public IDirect3D8
        {
            IDirect3D9* pD3D9;
            LONG ref;

            SimpleD3D8(IDirect3D9* d9) : pD3D9(d9), ref(1) { if (pD3D9) pD3D9->AddRef(); }
            ~SimpleD3D8() { if (pD3D9) pD3D9->Release(); }

            STDMETHOD(QueryInterface)(REFIID riid, void** ppv) {
                if (riid == IID_IUnknown || riid == __uuidof(IDirect3D8)) {
                    *ppv = this; AddRef(); return S_OK;
                }
                return E_NOINTERFACE;
            }
            STDMETHOD_(ULONG, AddRef)() { return InterlockedIncrement(&ref); }
            STDMETHOD_(ULONG, Release)() {
                ULONG r = InterlockedDecrement(&ref);
                if (r == 0) delete this;
                return r;
            }

            STDMETHOD(RegisterSoftwareDevice)(void* p) { return pD3D9->RegisterSoftwareDevice(p); }
            STDMETHOD_(UINT, GetAdapterCount)() { return pD3D9->GetAdapterCount(); }
            STDMETHOD(GetAdapterIdentifier)(UINT a, DWORD f, void* p) { return pD3D9->GetAdapterIdentifier(a, f, (D3DADAPTER_IDENTIFIER9*)p); }
            STDMETHOD_(UINT, GetAdapterModeCount)(UINT a) { return pD3D9->GetAdapterModeCount(a, D3DFMT_UNKNOWN); }
            STDMETHOD(EnumAdapterModes)(UINT a, UINT m, void* p) { return pD3D9->EnumAdapterModes(a, D3DFMT_UNKNOWN, m, (D3DDISPLAYMODE*)p); }
            STDMETHOD(GetAdapterDisplayMode)(UINT a, void* p) { return pD3D9->GetAdapterDisplayMode(a, (D3DDISPLAYMODE*)p); }
            STDMETHOD(CheckDeviceType)(UINT a, D3DDEVTYPE t, D3DFORMAT f1, D3DFORMAT f2, BOOL w) { return pD3D9->CheckDeviceType(a, t, f1, f2, w); }
            STDMETHOD(CheckDeviceFormat)(UINT a, D3DDEVTYPE t, D3DFORMAT f1, DWORD u, D3DRESOURCETYPE r, D3DFORMAT f2) { return pD3D9->CheckDeviceFormat(a, t, f1, u, r, f2); }
            STDMETHOD(CheckDeviceMultiSampleType)(UINT a, D3DDEVTYPE t, D3DFORMAT f, BOOL w, D3DMULTISAMPLE_TYPE m) { return pD3D9->CheckDeviceMultiSampleType(a, t, f, w, m, nullptr); }
            STDMETHOD(CheckDepthStencilMatch)(UINT a, D3DDEVTYPE t, D3DFORMAT f1, D3DFORMAT f2, D3DFORMAT f3) { return pD3D9->CheckDepthStencilMatch(a, t, f1, f2, f3); }
            STDMETHOD(GetDeviceCaps)(UINT a, D3DDEVTYPE t, void* p) { return pD3D9->GetDeviceCaps(a, t, (D3DCAPS9*)p); }
            STDMETHOD_(void*, GetAdapterMonitor)(UINT a) { return (void*)pD3D9->GetAdapterMonitor(a); }

            STDMETHOD(CreateDevice)(UINT a, D3DDEVTYPE t, HWND h, DWORD bf, void* pp, void** pd)
            {
                D3DPRESENT_PARAMETERS d3dpp = {0};
                if (pp)
                {
                    d3dpp.BackBufferWidth = ((D3DPRESENT_PARAMETERS*)pp)->BackBufferWidth;
                    d3dpp.BackBufferHeight = ((D3DPRESENT_PARAMETERS*)pp)->BackBufferHeight;
                    d3dpp.BackBufferFormat = ((D3DPRESENT_PARAMETERS*)pp)->BackBufferFormat;
                    d3dpp.BackBufferCount = ((D3DPRESENT_PARAMETERS*)pp)->BackBufferCount;
                    d3dpp.MultiSampleType = ((D3DPRESENT_PARAMETERS*)pp)->MultiSampleType;
                    d3dpp.SwapEffect = ((D3DPRESENT_PARAMETERS*)pp)->SwapEffect;
                    d3dpp.hDeviceWindow = ((D3DPRESENT_PARAMETERS*)pp)->hDeviceWindow;
                    d3dpp.Windowed = ((D3DPRESENT_PARAMETERS*)pp)->Windowed;
                    d3dpp.EnableAutoDepthStencil = ((D3DPRESENT_PARAMETERS*)pp)->EnableAutoDepthStencil;
                    d3dpp.AutoDepthStencilFormat = ((D3DPRESENT_PARAMETERS*)pp)->AutoDepthStencilFormat;
                }
                d3dpp.FullScreen_PresentationInterval = Config::g_Config.EnableVSync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

                IDirect3DDevice9* pDevice9 = nullptr;
                HRESULT hr = pD3D9->CreateDevice(a, t, h, bf, &d3dpp, &pDevice9);
                if (SUCCEEDED(hr) && pDevice9 && pd)
                {
                    *pd = new SimpleDevice8(pDevice9, this);
                    pDevice9->Release();
                }
                return hr;
            }
        };

        struct SimpleDevice8 : public IUnknown
        {
            IDirect3DDevice9* pDev;
            SimpleD3D8* pD3D8;
            LONG ref;

            SimpleDevice8(IDirect3DDevice9* d, SimpleD3D8* d8) : pDev(d), pD3D8(d8), ref(1) {
                if (pDev) pDev->AddRef();
                if (pD3D8) pD3D8->AddRef();
            }
            ~SimpleDevice8() {
                if (pDev) pDev->Release();
                if (pD3D8) pD3D8->Release();
            }

            STDMETHOD(QueryInterface)(REFIID riid, void** pp) {
                if (riid == IID_IUnknown || riid == __uuidof(IDirect3DDevice8)) {
                    *pp = this; AddRef(); return S_OK;
                }
                return E_NOINTERFACE;
            }
            STDMETHOD_(ULONG, AddRef)() { return InterlockedIncrement(&ref); }
            STDMETHOD_(ULONG, Release)() {
                ULONG r = InterlockedDecrement(&ref);
                if (r == 0) delete this;
                return r;
            }

            STDMETHOD(TestCooperativeLevel)() { return pDev->TestCooperativeLevel(); }
            STDMETHOD_(UINT, GetAvailableTextureMem)() { return pDev->GetAvailableTextureMem(); }
            STDMETHOD(ResourceManagerDiscardBytes)(DWORD b) { return pDev->EvictManagedResources(); }
            STDMETHOD(GetDirect3D)(IDirect3D8** p) { *p = pD3D8; pD3D8->AddRef(); return S_OK; }
            STDMETHOD(GetDeviceCaps)(void* c) { return pDev->GetDeviceCaps((D3DCAPS9*)c); }
            STDMETHOD(GetDisplayMode)(void* m) { return pDev->GetDisplayMode((D3DDISPLAYMODE*)m); }
            STDMETHOD(GetCreationParameters)(void* p) { return pDev->GetCreationParameters((D3DDEVICE_CREATION_PARAMETERS*)p); }

            STDMETHOD(CreateTexture)(UINT w, UINT h, UINT l, DWORD u, DWORD f, DWORD p, IDirect3DTexture8** pt)
            {
                IDirect3DTexture9* t9 = nullptr;
                D3DPOOL pool = (D3DPOOL)p;
                if (pool == D3DPOOL_DEFAULT && u == 0) pool = D3DPOOL_MANAGED;
                if (Config::g_Config.AnisotropicFiltering > 1 && u == 0) u = D3DUSAGE_DYNAMIC;

                UINT levels = (l == 0) ? 1 : l;
                HRESULT hr = pDev->CreateTexture(w, h, levels, u, (D3DFORMAT)f, pool, &t9, nullptr);
                if (SUCCEEDED(hr) && t9 && pt)
                {
                    *pt = new SimpleTexture8(t9);
                    t9->Release();
                }
                return hr;
            }

            STDMETHOD(SetTexture)(DWORD s, IDirect3DTexture8* t)
            {
                if (Config::g_Config.AnisotropicFiltering > 1 && t)
                {
                    pDev->SetSamplerState(s, D3DSAMP_MAXANISOTROPY, Config::g_Config.AnisotropicFiltering);
                    pDev->SetSamplerState(s, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
                    pDev->SetSamplerState(s, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
                }
                return pDev->SetTexture(s, t ? ((SimpleTexture8*)t)->GetRealTexture() : nullptr);
            }

            STDMETHOD(SetRenderState)(DWORD s, DWORD v) { return pDev->SetRenderState((D3DRENDERSTATETYPE)s, v); }
            STDMETHOD(GetRenderState)(DWORD s, DWORD* v) { return pDev->GetRenderState((D3DRENDERSTATETYPE)s, v); }
            STDMETHOD(BeginScene)() { return pDev->BeginScene(); }
            STDMETHOD(EndScene)() { return pDev->EndScene(); }
            STDMETHOD(Clear)(DWORD c, void* r, DWORD f, DWORD col, float z, DWORD st) { return pDev->Clear(c, (D3DRECT*)r, f, col, z, st); }
            STDMETHOD(DrawPrimitive)(DWORD pt, DWORD sv, DWORD pc) { return pDev->DrawPrimitive((D3DPRIMITIVETYPE)pt, sv, pc); }
            STDMETHOD(DrawIndexedPrimitive)(DWORD pt, DWORD mi, DWORD nv, DWORD si, DWORD pc) { return pDev->DrawIndexedPrimitive((D3DPRIMITIVETYPE)pt, mi, nv, si, pc); }
            STDMETHOD(SetVertexShader)(DWORD h) { return pDev->SetVertexShader(h); }
            STDMETHOD_(DWORD, GetVertexShader)() { return pDev->GetVertexShader(); }
            STDMETHOD(SetPixelShader)(DWORD h) { return pDev->SetPixelShader(h); }
            STDMETHOD_(DWORD, GetPixelShader)() { return pDev->GetPixelShader(); }
        };

        struct SimpleTexture8 : public IDirect3DTexture8
        {
            IDirect3DTexture9* tex;
            LONG ref;
            SimpleTexture8(IDirect3DTexture9* t) : tex(t), ref(1) { if (tex) tex->AddRef(); }
            ~SimpleTexture8() { if (tex) tex->Release(); }
            IDirect3DTexture9* GetRealTexture() { return tex; }

            STDMETHOD(QueryInterface)(REFIID riid, void** pp) {
                if (riid == IID_IUnknown || riid == __uuidof(IDirect3DTexture8)) {
                    *pp = this; AddRef(); return S_OK;
                }
                return E_NOINTERFACE;
            }
            STDMETHOD_(ULONG, AddRef)() { return InterlockedIncrement(&ref); }
            STDMETHOD_(ULONG, Release)() {
                ULONG r = InterlockedDecrement(&ref);
                if (r == 0) delete this;
                return r;
            }

            STDMETHOD(GetDevice)(IDirect3DDevice8** p) { return E_NOTIMPL; }
            STDMETHOD_(DWORD, SetPriority)(DWORD pr) { return tex->SetPriority(pr); }
            STDMETHOD_(DWORD, GetPriority)() { return tex->GetPriority(); }
            STDMETHOD_(void, PreLoad)() { tex->PreLoad(); }
            STDMETHOD_(DWORD, GetType)() { return D3DRTYPE_TEXTURE; }
            STDMETHOD_(DWORD, GetLevelCount)() { return tex->GetLevelCount(); }
            STDMETHOD(GetLevelDesc)(DWORD l, void* d) { D3DSURFACE_DESC s; tex->GetLevelDesc(l, &s); return S_OK; }
            STDMETHOD(GetSurfaceLevel)(DWORD l, IDirect3DSurface8** s) {
                IDirect3DSurface9* s9 = nullptr; tex->GetSurfaceLevel(l, &s9);
                if (s9 && s) *s = new SimpleSurface8(s9);
                if (s9) s9->Release();
                return S_OK;
            }
            STDMETHOD(LockRect)(DWORD l, void* r, void* rc, DWORD f) { return tex->LockRect(l, (D3DLOCKED_RECT*)r, (RECT*)rc, f); }
            STDMETHOD(UnlockRect)(DWORD l) { return tex->UnlockRect(l); }
        };

        struct SimpleSurface8 : public IDirect3DSurface8
        {
            IDirect3DSurface9* surf;
            LONG ref;
            SimpleSurface8(IDirect3DSurface9* s) : surf(s), ref(1) { if (surf) surf->AddRef(); }
            ~SimpleSurface8() { if (surf) surf->Release(); }

            STDMETHOD(QueryInterface)(REFIID riid, void** pp) {
                if (riid == IID_IUnknown || riid == __uuidof(IDirect3DSurface8)) {
                    *pp = this; AddRef(); return S_OK;
                }
                return E_NOINTERFACE;
            }
            STDMETHOD_(ULONG, AddRef)() { return InterlockedIncrement(&ref); }
            STDMETHOD_(ULONG, Release)() {
                ULONG r = InterlockedDecrement(&ref);
                if (r == 0) delete this;
                return r;
            }

            STDMETHOD(GetDevice)(IDirect3DDevice8** p) { return E_NOTIMPL; }
            STDMETHOD_(DWORD, SetPriority)(DWORD pr) { return surf->SetPriority(pr); }
            STDMETHOD_(DWORD, GetPriority)() { return surf->GetPriority(); }
            STDMETHOD_(void, PreLoad)() { surf->PreLoad(); }
            STDMETHOD_(DWORD, GetType)() { return D3DRTYPE_SURFACE; }
            STDMETHOD(GetDesc)(void* d) { D3DSURFACE_DESC s; surf->GetDesc(&s); return S_OK; }
            STDMETHOD(LockRect)(void* r, void* rc, DWORD f) { return surf->LockRect((D3DLOCKED_RECT*)r, (RECT*)rc, f); }
            STDMETHOD(UnlockRect)() { return surf->UnlockRect(); }
        };

        return new SimpleD3D8(g_pD3D9);
    }

    char sysPath[MAX_PATH];
    GetSystemDirectoryA(sysPath, MAX_PATH);
    strcat(sysPath, "\\d3d8.dll");
    HMODULE hD3D8 = LoadLibraryA(sysPath);
    if (hD3D8)
    {
        Direct3DCreate8_t pCreate = (Direct3DCreate8_t)GetProcAddress(hD3D8, "Direct3DCreate8");
        if (pCreate)
        {
            Config::Log("OFP D3DX Wrapper: Using real d3d8.dll\n");
            return pCreate(SDKVersion);
        }
    }
    return nullptr;
}
