#include <Windows.h>
#include <d3d9.h>
#include <stdio.h>

#pragma comment(lib, "d3d9.lib")

struct Config
{
    DWORD AnisotropicFiltering = 16;
    DWORD FPSUpdateInterval = 1;
    DWORD FPSPosX = 10;
    DWORD FPSPosY = 10;
    DWORD DefaultFOV = 90;
    DWORD FOVToggleKey = 45;
    FLOAT AimingFOVMultiplier = 0.6f;
    BOOL EnableVSync = FALSE;
    BOOL ShowFPS = TRUE;
    BOOL EnableFOV = TRUE;
    BOOL AllowDynamicFOV = FALSE;
};

static Config g_Config;
static DWORD g_FrameCount = 0;
static DWORD g_LastFPSTime = 0;
static DWORD g_CurrentFPS = 0;
static BOOL g_FOVToggleState = TRUE;
static DWORD g_LastKeyCheck = 0;
static BOOL g_LastKeyState = FALSE;

void LoadConfig()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    char* p = strrchr(path, '\\');
    if (p) p[1] = '\0';
    strcat_s(path, "d3dx.ini");

    g_Config.AnisotropicFiltering = GetPrivateProfileIntA("d3d9", "AnisotropicFiltering", 16, path);
    g_Config.EnableVSync = GetPrivateProfileIntA("d3d9", "EnableVSync", 0, path);
    g_Config.ShowFPS = GetPrivateProfileIntA("FPS", "ShowFPS", 1, path);
    g_Config.FPSUpdateInterval = GetPrivateProfileIntA("FPS", "FPSUpdateInterval", 1, path);
    g_Config.FPSPosX = GetPrivateProfileIntA("FPS", "FPSPosX", 10, path);
    g_Config.FPSPosY = GetPrivateProfileIntA("FPS", "FPSPosY", 10, path);
    g_Config.EnableFOV = GetPrivateProfileIntA("FOV", "EnableFOV", 1, path);
    g_Config.DefaultFOV = GetPrivateProfileIntA("FOV", "DefaultFOV", 90, path);
    g_Config.AllowDynamicFOV = GetPrivateProfileIntA("FOV", "AllowDynamicFOV", 0, path);
    g_Config.FOVToggleKey = GetPrivateProfileIntA("FOV", "FOVToggleKey", 45, path);
    g_Config.AimingFOVMultiplier = (FLOAT)GetPrivateProfileIntA("FOV", "AimingFOVMultiplier", 60, path) / 100.0f;
}

DEFINE_GUID(IID_IDirect3D8, 0x1DD9E8DA, 0x6C27, 0x41C8, 0xA7, 0xE7, 0x76, 0x15, 0xEE, 0xF7, 0x5C, 0x55);

interface IDirect3D8 : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE RegisterSoftwareDevice(void*) = 0;
    virtual UINT STDMETHODCALLTYPE GetAdapterCount(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT, DWORD, void*) = 0;
    virtual UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT, UINT, void*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT, void*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, BOOL) = 0;
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT, D3DDEVTYPE, D3DFORMAT, DWORD, D3DRESOURCETYPE, D3DFORMAT) = 0;
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT, D3DDEVTYPE, D3DFORMAT, BOOL, D3DMULTISAMPLE_TYPE) = 0;
    virtual HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, D3DFORMAT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT, D3DDEVTYPE, void*) = 0;
    virtual HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateDevice(UINT, D3DDEVTYPE, HWND, DWORD, void*, void**) = 0;
};

interface IDirect3DDevice8 : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void**) = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef(void) = 0;
    virtual ULONG STDMETHODCALLTYPE Release(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE TestCooperativeLevel(void) = 0;
    virtual UINT STDMETHODCALLTYPE GetAvailableTextureMem(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE EvictManagedResources(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT, UINT, IDirect3DSurface9*) = 0;
    virtual void STDMETHODCALLTYPE SetCursorPosition(int, int, DWORD) = 0;
    virtual BOOL STDMETHODCALLTYPE ShowCursor(BOOL) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS*, IDirect3DSwapChain9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSwapChain(UINT, IDirect3DSwapChain9**) = 0;
    virtual UINT STDMETHODCALLTYPE GetNumberOfSwapChains(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS*) = 0;
    virtual HRESULT STDMETHODCALLTYPE Present(RECT*, RECT*, HWND, RGNDATA*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT, D3DBACKBUFFER_TYPE, IDirect3DSurface9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRasterStatus(UINT, D3DRASTER_STATUS*) = 0;
    virtual void STDMETHODCALLTYPE SetGammaRamp(DWORD, DWORD, D3DGAMMARAMP*) = 0;
    virtual void STDMETHODCALLTYPE GetGammaRamp(UINT, D3DGAMMARAMP*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateTexture(UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DTexture9**, HANDLE*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT, UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DVolumeTexture9**, HANDLE*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DCubeTexture9**, HANDLE*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT, DWORD, DWORD, D3DPOOL, IDirect3DVertexBuffer9**, HANDLE*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DIndexBuffer9**, HANDLE*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface9**, HANDLE*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface9**, HANDLE*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurface(UINT, UINT, D3DFORMAT, D3DPOOL, IDirect3DSurface9**, HANDLE*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateVertexDeclaration(D3DVERTEXELEMENT9*, IDirect3DVertexDeclaration9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(DWORD*, IDirect3DVertexShader9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(DWORD*, IDirect3DPixelShader9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetVertexDeclaration(IDirect3DVertexDeclaration9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVertexDeclaration(IDirect3DVertexDeclaration9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVertexShader(IDirect3DVertexShader9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF(UINT, float*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantF(UINT, float*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI(UINT, int*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantI(UINT, int*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB(UINT, BOOL*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantB(UINT, BOOL*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPixelShader(IDirect3DPixelShader9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF(UINT, float*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantF(UINT, float*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI(UINT, int*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantI(UINT, int*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB(UINT, BOOL*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantB(UINT, BOOL*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE, UINT, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE, UINT, void*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE, UINT, UINT, UINT, void*, D3DFORMAT, void*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE ProcessVertices(UINT, UINT, UINT, IDirect3DVertexBuffer9*, IDirect3DVertexDeclaration9*, DWORD) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetVertexStream(UINT, IDirect3DVertexBuffer9*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVertexStream(UINT, IDirect3DVertexBuffer9**, UINT*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetVertexStreamFreq(UINT, DWORD) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVertexStreamFreq(UINT, DWORD*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetViewport(D3DVIEWPORT9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetMaterial(D3DMATERIAL9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLight(DWORD, D3DLIGHT9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLight(DWORD, D3DLIGHT9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE LightEnable(DWORD, BOOL) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD, BOOL*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD, float*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD, float*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE, DWORD) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE, DWORD*) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE, IDirect3DStateBlock9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE BeginStateBlock(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE EndStateBlock(IDirect3DStateBlock9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetClipStatus(D3DCLIPSTATUS9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTexture(DWORD, IDirect3DBaseTexture9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetTexture(DWORD, IDirect3DBaseTexture9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD, D3DTEXTURESTAGETYPE, DWORD) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSamplerState(DWORD, D3DSAMPLERSTATETYPE, DWORD*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD, D3DSAMPLERSTATETYPE, DWORD) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetScissorRect(RECT*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetScissorRect(RECT*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetSoftwareVertexProcessing(BOOL) = 0;
    virtual BOOL STDMETHODCALLTYPE GetSoftwareVertexProcessing(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetNPatchMode(float) = 0;
    virtual float STDMETHODCALLTYPE GetNPatchMode(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveStrided(D3DPRIMITIVETYPE, UINT, UINT, UINT, UINT, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT, PALETTEENTRY*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT, PALETTEENTRY*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT) = 0;
    virtual UINT STDMETHODCALLTYPE GetCurrentTexturePalette(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetFVF(DWORD) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetFVF(DWORD*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetStreamSource(UINT, IDirect3DVertexBuffer9*, UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetStreamSource(UINT, IDirect3DVertexBuffer9**, UINT*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT, DWORD) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetStreamSourceFreq(UINT, DWORD*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD, IDirect3DSurface9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRenderTarget(DWORD, IDirect3DSurface9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface9*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*) = 0;
    virtual HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*) = 0;
    virtual HRESULT STDMETHODCALLTYPE DeletePatch(UINT) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateQuery(D3DQUERYTYPE, IDirect3DQuery9**) = 0;
    virtual HRESULT STDMETHODCALLTYPE EndScene(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE BeginScene(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE Clear(DWORD, D3DRECT*, DWORD, D3DCOLOR, float, DWORD) = 0;
};

class CDirect3DDevice8Impl : public IDirect3DDevice8
{
private:
    IDirect3DDevice9* m_pDevice;
    LONG m_RefCount;

public:
    CDirect3DDevice8Impl(IDirect3DDevice9* pDevice) : m_pDevice(pDevice), m_RefCount(1)
    {
        if (m_pDevice) m_pDevice->AddRef();
        g_LastFPSTime = GetTickCount();
    }

    virtual ~CDirect3DDevice8Impl()
    {
        if (m_pDevice) m_pDevice->Release();
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj)
    {
        return m_pDevice->QueryInterface(riid, ppvObj);
    }

    virtual ULONG STDMETHODCALLTYPE AddRef(void)
    {
        return InterlockedIncrement(&m_RefCount);
    }

    virtual ULONG STDMETHODCALLTYPE Release(void)
    {
        ULONG ref = InterlockedDecrement(&m_RefCount);
        if (ref == 0) delete this;
        return ref;
    }

    virtual HRESULT STDMETHODCALLTYPE TestCooperativeLevel(void)
    {
        return m_pDevice->TestCooperativeLevel();
    }

    virtual UINT STDMETHODCALLTYPE GetAvailableTextureMem(void)
    {
        return m_pDevice->GetAvailableTextureMem();
    }

    virtual HRESULT STDMETHODCALLTYPE EvictManagedResources(void)
    {
        return m_pDevice->EvictManagedResources();
    }

    virtual HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D9** ppD3D9)
    {
        return m_pDevice->GetDirect3D(ppD3D9);
    }

    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS9* pCaps)
    {
        return m_pDevice->GetDeviceCaps(pCaps);
    }

    virtual HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE* pMode)
    {
        return m_pDevice->GetDisplayMode(0, pMode);
    }

    virtual HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
    {
        return m_pDevice->GetCreationParameters(pParameters);
    }

    virtual HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT a, UINT b, IDirect3DSurface9* c)
    {
        return m_pDevice->SetCursorProperties(a, b, c);
    }

    virtual void STDMETHODCALLTYPE SetCursorPosition(int X, int Y, DWORD Flags)
    {
        m_pDevice->SetCursorPosition(X, Y, Flags);
    }

    virtual BOOL STDMETHODCALLTYPE ShowCursor(BOOL bShow)
    {
        return m_pDevice->ShowCursor(bShow);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* p, IDirect3DSwapChain9** pp)
    {
        return m_pDevice->CreateAdditionalSwapChain(p, pp);
    }

    virtual HRESULT STDMETHODCALLTYPE GetSwapChain(UINT i, IDirect3DSwapChain9** pp)
    {
        return m_pDevice->GetSwapChain(i, pp);
    }

    virtual UINT STDMETHODCALLTYPE GetNumberOfSwapChains(void)
    {
        return m_pDevice->GetNumberOfSwapChains();
    }

    virtual HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS* p)
    {
        return m_pDevice->Reset(p);
    }

    virtual HRESULT STDMETHODCALLTYPE Present(RECT* a, RECT* b, HWND c, RGNDATA* d)
    {
        return m_pDevice->Present(a, b, c, d);
    }

    virtual HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT a, D3DBACKBUFFER_TYPE b, IDirect3DSurface9** pp)
    {
        return m_pDevice->GetBackBuffer(a, b, pp);
    }

    virtual HRESULT STDMETHODCALLTYPE GetRasterStatus(UINT a, D3DRASTER_STATUS* p)
    {
        return m_pDevice->GetRasterStatus(a, p);
    }

    virtual void STDMETHODCALLTYPE SetGammaRamp(DWORD a, DWORD b, D3DGAMMARAMP* p)
    {
        m_pDevice->SetGammaRamp(a, b, p);
    }

    virtual void STDMETHODCALLTYPE GetGammaRamp(UINT a, D3DGAMMARAMP* p)
    {
        m_pDevice->GetGammaRamp(a, p);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateTexture(UINT w, UINT h, UINT l, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DTexture9** pp, HANDLE* h)
    {
        return m_pDevice->CreateTexture(w, h, l, u, f, p, pp, h);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT w, UINT h, UINT d, UINT l, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DVolumeTexture9** pp, HANDLE* h)
    {
        return m_pDevice->CreateVolumeTexture(w, h, d, l, u, f, p, pp, h);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT s, UINT l, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DCubeTexture9** pp, HANDLE* h)
    {
        return m_pDevice->CreateCubeTexture(s, l, u, f, p, pp, h);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT l, DWORD u, DWORD f, D3DPOOL p, IDirect3DVertexBuffer9** pp, HANDLE* h)
    {
        return m_pDevice->CreateVertexBuffer(l, u, f, p, pp, h);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT l, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DIndexBuffer9** pp, HANDLE* h)
    {
        return m_pDevice->CreateIndexBuffer(l, u, f, p, pp, h);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE m, DWORD q, BOOL l, IDirect3DSurface9** pp, HANDLE* h)
    {
        return m_pDevice->CreateRenderTarget(w, h, f, m, q, l, pp, h);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE m, DWORD q, BOOL d, IDirect3DSurface9** pp, HANDLE* h)
    {
        return m_pDevice->CreateDepthStencilSurface(w, h, f, m, q, d, pp, h);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurface(UINT w, UINT h, D3DFORMAT f, D3DPOOL p, IDirect3DSurface9** pp, HANDLE* h)
    {
        return m_pDevice->CreateOffscreenPlainSurface(w, h, f, p, pp, h);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateVertexDeclaration(D3DVERTEXELEMENT9* e, IDirect3DVertexDeclaration9** pp)
    {
        return m_pDevice->CreateVertexDeclaration(e, pp);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(DWORD* f, IDirect3DVertexShader9** pp)
    {
        return m_pDevice->CreateVertexShader(f, pp);
    }

    virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(DWORD* f, IDirect3DPixelShader9** pp)
    {
        return m_pDevice->CreatePixelShader(f, pp);
    }

    virtual HRESULT STDMETHODCALLTYPE SetVertexDeclaration(IDirect3DVertexDeclaration9* p)
    {
        return m_pDevice->SetVertexDeclaration(p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetVertexDeclaration(IDirect3DVertexDeclaration9** pp)
    {
        return m_pDevice->GetVertexDeclaration(pp);
    }

    virtual HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9* p)
    {
        return m_pDevice->SetVertexShader(p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetVertexShader(IDirect3DVertexShader9** pp)
    {
        return m_pDevice->GetVertexShader(pp);
    }

    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF(UINT r, float* p, UINT c)
    {
        return m_pDevice->SetVertexShaderConstantF(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantF(UINT r, float* p, UINT c)
    {
        return m_pDevice->GetVertexShaderConstantF(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI(UINT r, int* p, UINT c)
    {
        return m_pDevice->SetVertexShaderConstantI(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantI(UINT r, int* p, UINT c)
    {
        return m_pDevice->GetVertexShaderConstantI(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB(UINT r, BOOL* p, UINT c)
    {
        return m_pDevice->SetVertexShaderConstantB(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantB(UINT r, BOOL* p, UINT c)
    {
        return m_pDevice->GetVertexShaderConstantB(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9* p)
    {
        return m_pDevice->SetPixelShader(p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetPixelShader(IDirect3DPixelShader9** pp)
    {
        return m_pDevice->GetPixelShader(pp);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF(UINT r, float* p, UINT c)
    {
        return m_pDevice->SetPixelShaderConstantF(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantF(UINT r, float* p, UINT c)
    {
        return m_pDevice->GetPixelShaderConstantF(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI(UINT r, int* p, UINT c)
    {
        return m_pDevice->SetPixelShaderConstantI(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantI(UINT r, int* p, UINT c)
    {
        return m_pDevice->GetPixelShaderConstantI(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB(UINT r, BOOL* p, UINT c)
    {
        return m_pDevice->SetPixelShaderConstantB(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantB(UINT r, BOOL* p, UINT c)
    {
        return m_pDevice->GetPixelShaderConstantB(r, p, c);
    }

    virtual HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE t, UINT s, UINT c)
    {
        return m_pDevice->DrawPrimitive(t, s, c);
    }

    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE t, INT b, UINT m, UINT n, UINT s, UINT p)
    {
        return m_pDevice->DrawIndexedPrimitive(t, b, m, n, s, p);
    }

    virtual HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE t, UINT c, void* d, UINT s)
    {
        return m_pDevice->DrawPrimitiveUP(t, c, d, s);
    }

    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE t, UINT m, UINT n, UINT c, void* i, D3DFORMAT f, void* v, UINT s)
    {
        return m_pDevice->DrawIndexedPrimitiveUP(t, m, n, c, i, f, v, s);
    }

    virtual HRESULT STDMETHODCALLTYPE ProcessVertices(UINT s, UINT d, UINT c, IDirect3DVertexBuffer9* b, IDirect3DVertexDeclaration9* dcl, DWORD f)
    {
        return m_pDevice->ProcessVertices(s, d, c, b, dcl, f);
    }

    virtual HRESULT STDMETHODCALLTYPE SetVertexStream(UINT s, IDirect3DVertexBuffer9* b, UINT st)
    {
        return m_pDevice->SetVertexStream(s, b, st);
    }

    virtual HRESULT STDMETHODCALLTYPE GetVertexStream(UINT s, IDirect3DVertexBuffer9** b, UINT* st)
    {
        return m_pDevice->GetVertexStream(s, b, st);
    }

    virtual HRESULT STDMETHODCALLTYPE SetVertexStreamFreq(UINT s, DWORD f)
    {
        return m_pDevice->SetVertexStreamFreq(s, f);
    }

    virtual HRESULT STDMETHODCALLTYPE GetVertexStreamFreq(UINT s, DWORD* f)
    {
        return m_pDevice->GetVertexStreamFreq(s, f);
    }

    virtual HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer9* b)
    {
        return m_pDevice->SetIndices(b);
    }

    virtual HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer9** pp)
    {
        return m_pDevice->GetIndices(pp);
    }

    virtual void UpdateFPS()
    {
        DWORD now = GetTickCount();
        g_FrameCount++;
        if (now - g_LastFPSTime >= g_Config.FPSUpdateInterval * 1000)
        {
            g_CurrentFPS = g_FrameCount;
            g_FrameCount = 0;
            g_LastFPSTime = now;
        }
    }

    virtual void CheckFOVKey()
    {
        if (!g_Config.EnableFOV || g_Config.FOVToggleKey == 0) return;

        DWORD now = GetTickCount();
        if (now - g_LastKeyCheck < 100) return;
        g_LastKeyCheck = now;

        BOOL pressed = (GetAsyncKeyState((int)g_Config.FOVToggleKey) & 0x8000) != 0;
        if (pressed && !g_LastKeyState)
        {
            g_FOVToggleState = !g_FOVToggleState;
        }
        g_LastKeyState = pressed;
    }

    virtual void ApplyAnisotropic()
    {
        if (g_Config.AnisotropicFiltering > 0)
        {
            for (UINT i = 0; i < 8; i++)
            {
                m_pDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
                m_pDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
                m_pDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, g_Config.AnisotropicFiltering);
            }
        }
    }

    virtual D3DVIEWPORT9 AdjustFOV(D3DVIEWPORT9 vp)
    {
        if (!g_Config.AllowDynamicFOV) return vp;

        CheckFOVKey();

        float targetFOV = (float)g_Config.DefaultFOV;
        if (!g_FOVToggleState)
        {
            targetFOV *= g_Config.AimingFOVMultiplier;
        }

        vp.Width = (DWORD)(vp.Height / (2.0f * tan(targetFOV * 3.14159f / 360.0f)));
        vp.X = (DWORD)((vp.Width - vp.Height * 4.0f / 3.0f) / 2);

        return vp;
    }

    virtual HRESULT STDMETHODCALLTYPE SetViewport(D3DVIEWPORT9* pViewport)
    {
        D3DVIEWPORT9 vp = *pViewport;
        vp = AdjustFOV(vp);
        return m_pDevice->SetViewport(&vp);
    }

    virtual HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT9* pViewport)
    {
        return m_pDevice->GetViewport(pViewport);
    }

    virtual HRESULT STDMETHODCALLTYPE SetMaterial(D3DMATERIAL9* p)
    {
        return m_pDevice->SetMaterial(p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL9* p)
    {
        return m_pDevice->GetMaterial(p);
    }

    virtual HRESULT STDMETHODCALLTYPE SetLight(DWORD i, D3DLIGHT9* l)
    {
        return m_pDevice->SetLight(i, l);
    }

    virtual HRESULT STDMETHODCALLTYPE GetLight(DWORD i, D3DLIGHT9* l)
    {
        return m_pDevice->GetLight(i, l);
    }

    virtual HRESULT STDMETHODCALLTYPE LightEnable(DWORD i, BOOL e)
    {
        return m_pDevice->LightEnable(i, e);
    }

    virtual HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD i, BOOL* e)
    {
        return m_pDevice->GetLightEnable(i, e);
    }

    virtual HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD i, float* p)
    {
        return m_pDevice->SetClipPlane(i, p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD i, float* p)
    {
        return m_pDevice->GetClipPlane(i, p);
    }

    virtual HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE s, DWORD v)
    {
        return m_pDevice->SetRenderState(s, v);
    }

    virtual HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE s, DWORD* v)
    {
        return m_pDevice->GetRenderState(s, v);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE t, IDirect3DStateBlock9** pp)
    {
        return m_pDevice->CreateStateBlock(t, pp);
    }

    virtual HRESULT STDMETHODCALLTYPE BeginStateBlock(void)
    {
        return m_pDevice->BeginStateBlock();
    }

    virtual HRESULT STDMETHODCALLTYPE EndStateBlock(IDirect3DStateBlock9** pp)
    {
        return m_pDevice->EndStateBlock(pp);
    }

    virtual HRESULT STDMETHODCALLTYPE SetClipStatus(D3DCLIPSTATUS9* p)
    {
        return m_pDevice->SetClipStatus(p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS9* p)
    {
        return m_pDevice->GetClipStatus(p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetTexture(DWORD s, IDirect3DBaseTexture9** pp)
    {
        return m_pDevice->GetTexture(s, pp);
    }

    virtual HRESULT STDMETHODCALLTYPE SetTexture(DWORD s, IDirect3DBaseTexture9* p)
    {
        return m_pDevice->SetTexture(s, p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD s, D3DTEXTURESTAGESTATETYPE t, DWORD* v)
    {
        return m_pDevice->GetTextureStageState(s, t, v);
    }

    virtual HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD s, D3DTEXTURESTAGETYPE t, DWORD v)
    {
        return m_pDevice->SetTextureStageState(s, t, v);
    }

    virtual HRESULT STDMETHODCALLTYPE GetSamplerState(DWORD s, D3DSAMPLERSTATETYPE t, DWORD* v)
    {
        return m_pDevice->GetSamplerState(s, t, v);
    }

    virtual HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD s, D3DSAMPLERSTATETYPE t, DWORD v)
    {
        return m_pDevice->SetSamplerState(s, t, v);
    }

    virtual HRESULT STDMETHODCALLTYPE SetScissorRect(RECT* p)
    {
        return m_pDevice->SetScissorRect(p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetScissorRect(RECT* p)
    {
        return m_pDevice->GetScissorRect(p);
    }

    virtual HRESULT STDMETHODCALLTYPE SetSoftwareVertexProcessing(BOOL b)
    {
        return m_pDevice->SetSoftwareVertexProcessing(b);
    }

    virtual BOOL STDMETHODCALLTYPE GetSoftwareVertexProcessing(void)
    {
        return m_pDevice->GetSoftwareVertexProcessing();
    }

    virtual HRESULT STDMETHODCALLTYPE SetNPatchMode(float n)
    {
        return m_pDevice->SetNPatchMode(n);
    }

    virtual float STDMETHODCALLTYPE GetNPatchMode(void)
    {
        return m_pDevice->GetNPatchMode();
    }

    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveStrided(D3DPRIMITIVETYPE t, UINT b, UINT m, UINT n, UINT s, UINT p)
    {
        return m_pDevice->DrawIndexedPrimitiveStrided(t, b, m, n, s, p);
    }

    virtual HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD* p)
    {
        return m_pDevice->ValidateDevice(p);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT p, PALETTEENTRY* e)
    {
        return m_pDevice->SetPaletteEntries(p, e);
    }

    virtual HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT p, PALETTEENTRY* e)
    {
        return m_pDevice->GetPaletteEntries(p, e);
    }

    virtual UINT STDMETHODCALLTYPE GetCurrentTexturePalette(void)
    {
        return m_pDevice->GetCurrentTexturePalette();
    }

    virtual HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT p)
    {
        return m_pDevice->SetCurrentTexturePalette(p);
    }

    virtual HRESULT STDMETHODCALLTYPE SetFVF(DWORD f)
    {
        return m_pDevice->SetFVF(f);
    }

    virtual HRESULT STDMETHODCALLTYPE GetFVF(DWORD* f)
    {
        return m_pDevice->GetFVF(f);
    }

    virtual HRESULT STDMETHODCALLTYPE SetStreamSource(UINT s, IDirect3DVertexBuffer9* b, UINT st)
    {
        return m_pDevice->SetStreamSource(s, b, st);
    }

    virtual HRESULT STDMETHODCALLTYPE GetStreamSource(UINT s, IDirect3DVertexBuffer9** b, UINT* st)
    {
        return m_pDevice->GetStreamSource(s, b, st);
    }

    virtual HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT s, DWORD f)
    {
        return m_pDevice->SetStreamSourceFreq(s, f);
    }

    virtual HRESULT STDMETHODCALLTYPE GetStreamSourceFreq(UINT s, DWORD* f)
    {
        return m_pDevice->GetStreamSourceFreq(s, f);
    }

    virtual HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD i, IDirect3DSurface9* p)
    {
        return m_pDevice->SetRenderTarget(i, p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetRenderTarget(DWORD i, IDirect3DSurface9** pp)
    {
        return m_pDevice->GetRenderTarget(i, pp);
    }

    virtual HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface9* p)
    {
        return m_pDevice->SetDepthStencilSurface(p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface9** pp)
    {
        return m_pDevice->GetDepthStencilSurface(pp);
    }

    virtual HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE s, D3DMATRIX* p)
    {
        return m_pDevice->SetTransform(s, p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE s, D3DMATRIX* p)
    {
        return m_pDevice->GetTransform(s, p);
    }

    virtual HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE s, D3DMATRIX* p)
    {
        return m_pDevice->MultiplyTransform(s, p);
    }

    virtual HRESULT STDMETHODCALLTYPE DeletePatch(UINT h)
    {
        return m_pDevice->DeletePatch(h);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateQuery(D3DQUERYTYPE t, IDirect3DQuery9** pp)
    {
        return m_pDevice->CreateQuery(t, pp);
    }

    virtual HRESULT STDMETHODCALLTYPE EndScene(void)
    {
        UpdateFPS();
        return m_pDevice->EndScene();
    }

    virtual HRESULT STDMETHODCALLTYPE BeginScene(void)
    {
        return m_pDevice->BeginScene();
    }

    virtual HRESULT STDMETHODCALLTYPE Clear(DWORD c, D3DRECT* r, DWORD f, D3DCOLOR col, float z, DWORD s)
    {
        return m_pDevice->Clear(c, r, f, col, z, s);
    }
};

class CDirect3D8Impl : public IDirect3D8
{
private:
    IDirect3D9* m_pD3D;
    LONG m_RefCount;

public:
    CDirect3D8Impl(IDirect3D9* pD3D) : m_pD3D(pD3D), m_RefCount(1)
    {
        if (m_pD3D) m_pD3D->AddRef();
        LoadConfig();
    }

    virtual ~CDirect3D8Impl()
    {
        if (m_pD3D) m_pD3D->Release();
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj)
    {
        if (ppvObj == NULL) return E_INVALIDARG;
        if (riid == IID_IUnknown || riid == IID_IDirect3D8)
        {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef(void)
    {
        return InterlockedIncrement(&m_RefCount);
    }

    virtual ULONG STDMETHODCALLTYPE Release(void)
    {
        ULONG ref = InterlockedDecrement(&m_RefCount);
        if (ref == 0) delete this;
        return ref;
    }

    virtual UINT STDMETHODCALLTYPE GetAdapterCount(void)
    {
        return m_pD3D->GetAdapterCount();
    }

    virtual HRESULT STDMETHODCALLTYPE RegisterSoftwareDevice(void* p)
    {
        return m_pD3D->RegisterSoftwareDevice(p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT a, DWORD f, void* p)
    {
        return m_pD3D->GetAdapterIdentifier(a, f, (D3DADAPTER_IDENTIFIER9*)p);
    }

    virtual UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT a)
    {
        return m_pD3D->GetAdapterModeCount(a, D3DFMT_UNKNOWN);
    }

    virtual HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT a, UINT m, void* p)
    {
        return m_pD3D->EnumAdapterModes(a, D3DFMT_UNKNOWN, m, (D3DDISPLAYMODE*)p);
    }

    virtual HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT a, void* p)
    {
        return m_pD3D->GetAdapterDisplayMode(a, (D3DDISPLAYMODE*)p);
    }

    virtual HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT a, D3DDEVTYPE t, D3DFORMAT d, D3DFORMAT b, BOOL w)
    {
        return m_pD3D->CheckDeviceType(a, t, d, b, w);
    }

    virtual HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT a, D3DDEVTYPE t, D3DFORMAT f, DWORD u, D3DRESOURCETYPE r, D3DFORMAT c)
    {
        return m_pD3D->CheckDeviceFormat(a, t, f, u, r, c);
    }

    virtual HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT a, D3DDEVTYPE t, D3DFORMAT f, BOOL w, D3DMULTISAMPLE_TYPE m)
    {
        return m_pD3D->CheckDeviceMultiSampleType(a, t, f, w, m, NULL);
    }

    virtual HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT a, D3DDEVTYPE t, D3DFORMAT f, D3DFORMAT r, D3DFORMAT s)
    {
        return m_pD3D->CheckDepthStencilMatch(a, t, f, r, s);
    }

    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT a, D3DDEVTYPE t, void* p)
    {
        D3DCAPS9 caps;
        return m_pD3D->GetDeviceCaps(a, t, &caps);
    }

    virtual HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT a)
    {
        return m_pD3D->GetAdapterMonitor(a);
    }

    virtual HRESULT STDMETHODCALLTYPE CreateDevice(UINT a, D3DDEVTYPE t, HWND h, DWORD b, void* pp, void** ppOut)
    {
        if (ppOut == NULL) return D3DERR_INVALIDCALL;

        D3DPRESENT_PARAMETERS pp9 = {};
        if (pp)
        {
            D3DPRESENT_PARAMETERS* pp8 = (D3DPRESENT_PARAMETERS*)pp;
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
            pp9.PresentationInterval = g_Config.EnableVSync ? pp8->PresentationInterval : D3DPRESENT_INTERVAL_IMMEDIATE;
        }

        IDirect3DDevice9* pDevice9 = NULL;
        HRESULT hr = m_pD3D->CreateDevice(a, t, h, b, &pp9, &pDevice9);

        if (SUCCEEDED(hr) && pDevice9)
        {
            CDirect3DDevice8Impl* pDevice = new CDirect3DDevice8Impl(pDevice9);
            pDevice9->Release();
            pDevice->ApplyAnisotropic();
            *ppOut = pDevice;
        }

        return hr;
    }
};

extern "C" __declspec(dllexport) IDirect3D8* WINAPI Direct3DCreate8(UINT SDKVersion)
{
    IDirect3D9* pD3D9 = Direct3DCreate9(220);
    if (!pD3D9) return NULL;

    CDirect3D8Impl* pD3D8 = new CDirect3D8Impl(pD3D9);
    pD3D9->Release();
    return pD3D8;
}
