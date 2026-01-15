#include <windows.h>
#include "d3d8_interfaces.h"
#include "d3d8_opengl_renderer.cpp"

class CDirect3DDevice8OpenGL;

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

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) {
        if (ppvObj == NULL) return E_INVALIDARG;
        if (riid == IID_IUnknown || riid == IID_IDirect3D8) {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef() {
        return InterlockedIncrement(&refCount);
    }

    virtual ULONG STDMETHODCALLTYPE Release() {
        ULONG ref = InterlockedDecrement(&refCount);
        if (ref == 0) delete this;
        return ref;
    }

    virtual UINT STDMETHODCALLTYPE GetAdapterCount() { return 1; }

    virtual HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT, DWORD, D3DADAPTER_IDENTIFIER8* pIdentifier) {
        strcpy_s(pIdentifier->Driver, "OpenGL Driver");
        strcpy_s(pIdentifier->Description, "OpenGL Renderer");
        return S_OK;
    }

    virtual UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT) { return 1; }

    virtual HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT, UINT, D3DDISPLAYMODE* pMode) {
        pMode->Width = 800;
        pMode->Height = 600;
        pMode->Format = D3DFMT_X8R8G8B8;
        pMode->RefreshRate = 60;
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT, D3DDISPLAYMODE* pMode) {
        return EnumAdapterModes(0, 0, pMode);
    }

    virtual HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, BOOL) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT, D3DDEVTYPE, D3DFORMAT, DWORD, D3DRESOURCETYPE, D3DFORMAT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT, D3DDEVTYPE, D3DFORMAT, BOOL, D3DMULTISAMPLE_TYPE) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, D3DFORMAT) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT, D3DDEVTYPE, D3DCAPS8* pCaps) {
        memset(pCaps, 0, sizeof(D3DCAPS8));
        pCaps->DeviceType = D3DDEVTYPE_HAL;
        return S_OK;
    }

    virtual HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT) { return NULL; }

    virtual HRESULT STDMETHODCALLTYPE CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS8* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface) {
        if (ppReturnedDeviceInterface == NULL) return D3DERR_INVALIDCALL;

        if (!renderer->Initialize(hFocusWindow)) {
            OutputDebugStringA("[OFP-D3DX OpenGL] Failed to initialize renderer\n");
            return E_FAIL;
        }

        renderer->ApplyAnisotropicFiltering();

        CDirect3DDevice8OpenGL* device = new CDirect3DDevice8OpenGL(renderer);
        *ppReturnedDeviceInterface = device;

        OutputDebugStringA("[OFP-D3DX OpenGL] OpenGL device created successfully\n");
        return S_OK;
    }
};

class CDirect3DDevice8OpenGL : public IDirect3DDevice8 {
private:
    OpenGLRenderer* renderer;
    LONG refCount;

public:
    CDirect3DDevice8OpenGL(OpenGLRenderer* ogl) : renderer(ogl), refCount(1) {}

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) {
        if (ppvObj == NULL) return E_INVALIDARG;
        if (riid == IID_IUnknown) {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef() {
        return InterlockedIncrement(&refCount);
    }

    virtual ULONG STDMETHODCALLTYPE Release() {
        ULONG ref = InterlockedDecrement(&refCount);
        if (ref == 0) delete this;
        return ref;
    }

    virtual HRESULT STDMETHODCALLTYPE TestCooperativeLevel() { return S_OK; }
    virtual UINT STDMETHODCALLTYPE GetAvailableTextureMem() { return 128 * 1024 * 1024; }
    virtual HRESULT STDMETHODCALLTYPE EvictManagedResources() { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS8*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT, UINT, IDirect3DSurface8*) { return S_OK; }
    virtual void STDMETHODCALLTYPE SetCursorPosition(int, int, DWORD) {}
    virtual BOOL STDMETHODCALLTYPE ShowCursor(BOOL) { return TRUE; }
    virtual UINT STDMETHODCALLTYPE GetNumberOfSwapChains() { return 0; }
    virtual HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS8*) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE Present(CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*) {
        if (renderer) renderer->RenderFrame();
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT, D3DBACKBUFFER_TYPE, IDirect3DSurface8**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE GetRasterStatus(D3DRASTER_STATUS*) { return S_OK; }
    virtual void STDMETHODCALLTYPE SetGammaRamp(DWORD, DWORD, CONST D3DGAMMARAMP*) {}
    virtual void STDMETHODCALLTYPE GetGammaRamp(D3DGAMMARAMP*) {}

    virtual HRESULT STDMETHODCALLTYPE CreateTexture(UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DTexture8**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT, UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DVolumeTexture8**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DCubeTexture8**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT, DWORD, DWORD, D3DPOOL, IDirect3DVertexBuffer8**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DIndexBuffer8**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface8**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface8**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurface(UINT, UINT, D3DFORMAT, D3DPOOL, IDirect3DSurface8**, HANDLE*) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE SetVertexDeclaration(IDirect3DVertexDeclaration9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexDeclaration(IDirect3DVertexDeclaration9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexShader(IDirect3DVertexShader9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF(UINT, CONST float*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantF(UINT, float*, UINT) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI(UINT, CONST int*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantI(UINT, int*, UINT) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB(UINT, CONST BOOL*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantB(UINT, BOOL*, UINT) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPixelShader(IDirect3DVertexShader9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF(UINT, CONST float*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantF(UINT, float*, UINT) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI(UINT, CONST int*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantI(UINT, int*, UINT) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB(UINT, CONST BOOL*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantB(UINT, BOOL*, UINT) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE, UINT, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE, UINT, CONST void*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE, UINT, UINT, UINT, CONST void*, D3DFORMAT, CONST void*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE ProcessVertices(UINT, UINT, UINT, IDirect3DVertexBuffer8*, IDirect3DVertexDeclaration9*, DWORD) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE SetVertexStream(UINT, IDirect3DVertexBuffer8*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexStream(UINT, IDirect3DVertexBuffer8**, UINT*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexStreamFreq(UINT, DWORD) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexStreamFreq(UINT, DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer8*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer8**, UINT*) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE SetViewport(CONST D3DVIEWPORT8* pViewport) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT8* pViewport) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE SetMaterial(CONST D3DMATERIAL8*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL8*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetLight(DWORD, CONST D3DLIGHT8*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetLight(DWORD, D3DLIGHT8*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE LightEnable(DWORD, BOOL) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD, BOOL*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD, CONST float*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD, float*) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE, DWORD) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE, DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetClipStatus(CONST D3DCLIPSTATUS8*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS8*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE GetTexture(DWORD, IDirect3DBaseTexture8**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetTexture(DWORD, IDirect3DBaseTexture8*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetSamplerState(DWORD, D3DSAMPLERSTATETYPE, DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD, D3DSAMPLERSTATETYPE, DWORD) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE SetScissorRect(CONST RECT*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetScissorRect(RECT*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetSoftwareVertexProcessing(BOOL) { return S_OK; }
    virtual BOOL STDMETHODCALLTYPE GetSoftwareVertexProcessing() { return FALSE; }
    virtual HRESULT STDMETHODCALLTYPE SetNPatchMode(float) { return S_OK; }
    virtual float STDMETHODCALLTYPE GetNPatchMode() { return 0.0f; }
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveStrided(D3DPRIMITIVETYPE, UINT, UINT, UINT, UINT, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT, CONST PALETTEENTRY*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT, PALETTEENTRY*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT) { return S_OK; }
    virtual UINT STDMETHODCALLTYPE GetCurrentTexturePalette() { return 0; }
    virtual HRESULT STDMETHODCALLTYPE SetFVF(DWORD) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetFVF(DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetStreamSource(UINT, IDirect3DVertexBuffer8*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetStreamSource(UINT, IDirect3DVertexBuffer8**, UINT*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT, DWORD) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetStreamSourceFreq(UINT, DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD, IDirect3DSurface8*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetRenderTarget(DWORD, IDirect3DSurface8**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface8*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface8**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DeletePatch(UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE CreateQuery(D3DQUERYTYPE, IDirect3DQuery9**) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE EndScene() {
        if (renderer) {
            renderer->RenderFrame();
        }
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE BeginScene() { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE Clear(DWORD, CONST D3DRECT*, DWORD, D3DCOLOR, float, DWORD) { return S_OK; }
};

extern "C" __declspec(dllexport) IDirect3D8* WINAPI Direct3DCreate8(UINT SDKVersion) {
    OutputDebugStringA("[OFP-D3DX OpenGL] Direct3DCreate8 called\n");

    OpenGLRenderer* renderer = new OpenGLRenderer();
    CDirect3D8OpenGL* bridge = new CDirect3D8OpenGL(renderer);

    return bridge;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        OutputDebugStringA("[OFP-D3DX OpenGL] DLL attached to process\n");
        break;
    case DLL_PROCESS_DETACH:
        OutputDebugStringA("[OFP-D3DX OpenGL] DLL detached from process\n");
        break;
    }
    return TRUE;
}
