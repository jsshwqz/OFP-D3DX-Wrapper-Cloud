// Minimal D3D8 interface definitions
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

// D3D8 OpenGL Bridge Implementation
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

    virtual HRESULT STDMETHODCALLTYPE RegisterSoftwareDevice(void*) { return S_OK; }
    virtual UINT STDMETHODCALLTYPE GetAdapterCount(void) { return 1; }

    virtual HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT, DWORD, void* pIdentifier) {
        D3DADAPTER_IDENTIFIER9* pAdapter = (D3DADAPTER_IDENTIFIER9*)pIdentifier;
        strcpy_s(pAdapter->Driver, "OpenGL Driver");
        strcpy_s(pAdapter->Description, "OpenGL Renderer");
        return S_OK;
    }

    virtual UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT) { return 1; }

    virtual HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT, UINT, void* pMode) {
        D3DDISPLAYMODE* pDisplayMode = (D3DDISPLAYMODE*)pMode;
        pDisplayMode->Width = 800;
        pDisplayMode->Height = 600;
        pDisplayMode->Format = D3DFMT_X8R8G8B8;
        pDisplayMode->RefreshRate = 60;
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT, void* pMode) {
        return EnumAdapterModes(0, 0, pMode);
    }

    virtual HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, BOOL) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT, D3DDEVTYPE, D3DFORMAT, DWORD, D3DRESOURCETYPE, D3DFORMAT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT, D3DDEVTYPE, D3DFORMAT, BOOL, D3DMULTISAMPLE_TYPE) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, D3DFORMAT) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT, D3DDEVTYPE, void* pCaps) {
        D3DCAPS9* pD3DCaps = (D3DCAPS9*)pCaps;
        memset(pD3DCaps, 0, sizeof(D3DCAPS9));
        pD3DCaps->DeviceType = D3DDEVTYPE_HAL;
        return S_OK;
    }

    virtual HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT) { return NULL; }

    virtual HRESULT STDMETHODCALLTYPE CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, void* pPresentationParameters, void** ppReturnedDeviceInterface) {
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

// Device implementation
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

    // Minimal implementations
    virtual HRESULT STDMETHODCALLTYPE TestCooperativeLevel() { return S_OK; }
    virtual UINT STDMETHODCALLTYPE GetAvailableTextureMem() { return 128 * 1024 * 1024; }
    virtual HRESULT STDMETHODCALLTYPE EvictManagedResources() { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT, UINT, IDirect3DSurface9*) { return S_OK; }
    virtual void STDMETHODCALLTYPE SetCursorPosition(int, int, DWORD) {}
    virtual BOOL STDMETHODCALLTYPE ShowCursor(BOOL) { return TRUE; }
    virtual HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS*, IDirect3DSwapChain9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE GetSwapChain(UINT, IDirect3DSwapChain9**) { return E_NOTIMPL; }
    virtual UINT STDMETHODCALLTYPE GetNumberOfSwapChains(void) { return 0; }
    virtual HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS*) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE Present(RECT*, RECT*, HWND, RGNDATA*) {
        if (renderer) renderer->RenderFrame();
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT, D3DBACKBUFFER_TYPE, IDirect3DSurface9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE GetRasterStatus(UINT, D3DRASTER_STATUS*) { return S_OK; }
    virtual void STDMETHODCALLTYPE SetGammaRamp(DWORD, DWORD, D3DGAMMARAMP*) {}
    virtual void STDMETHODCALLTYPE GetGammaRamp(UINT, D3DGAMMARAMP*) {}

    virtual HRESULT STDMETHODCALLTYPE CreateTexture(UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DTexture9**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT, UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DVolumeTexture9**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DCubeTexture9**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT, DWORD, DWORD, D3DPOOL, IDirect3DVertexBuffer9**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DIndexBuffer9**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface9**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface9**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurface(UINT, UINT, D3DFORMAT, D3DPOOL, IDirect3DSurface9**, HANDLE*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateVertexDeclaration(D3DVERTEXELEMENT9*, IDirect3DVertexDeclaration9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(DWORD*, IDirect3DVertexShader9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(DWORD*, IDirect3DPixelShader9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexDeclaration(IDirect3DVertexDeclaration9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexDeclaration(IDirect3DVertexDeclaration9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexShader(IDirect3DVertexShader9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF(UINT, float*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantF(UINT, float*, UINT) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI(UINT, int*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantI(UINT, int*, UINT) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB(UINT, BOOL*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantB(UINT, BOOL*, UINT) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPixelShader(IDirect3DPixelShader9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF(UINT, float*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantF(UINT, float*, UINT) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI(UINT, int*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantI(UINT, int*, UINT) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB(UINT, BOOL*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantB(UINT, BOOL*, UINT) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE, UINT, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE, UINT, void*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE, UINT, UINT, UINT, void*, D3DFORMAT, void*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE ProcessVertices(UINT, UINT, UINT, IDirect3DVertexBuffer9*, IDirect3DVertexDeclaration9*, DWORD) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE SetVertexStream(UINT, IDirect3DVertexBuffer9*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexStream(UINT, IDirect3DVertexBuffer9**, UINT*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetVertexStreamFreq(UINT, DWORD) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetVertexStreamFreq(UINT, DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer9**) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE SetViewport(D3DVIEWPORT9* pViewport) {
        // This is where FOV adjustment would be applied
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE SetMaterial(D3DMATERIAL9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL9*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetLight(DWORD, D3DLIGHT9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetLight(DWORD, D3DLIGHT9*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE LightEnable(DWORD, BOOL) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD, BOOL*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD, float*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD, float*) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE, DWORD) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE, DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE, IDirect3DStateBlock9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE BeginStateBlock(void) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE EndStateBlock(IDirect3DStateBlock9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetClipStatus(D3DCLIPSTATUS9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS9*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE GetTexture(DWORD, IDirect3DBaseTexture9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetTexture(DWORD, IDirect3DBaseTexture9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD, D3DTEXTURESTAGETYPE, DWORD) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE GetSamplerState(DWORD, D3DSAMPLERSTATETYPE, DWORD*) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
        // This is where anisotropic filtering would be applied
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE SetScissorRect(RECT*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetScissorRect(RECT*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetSoftwareVertexProcessing(BOOL) { return S_OK; }
    virtual BOOL STDMETHODCALLTYPE GetSoftwareVertexProcessing(void) { return FALSE; }
    virtual HRESULT STDMETHODCALLTYPE SetNPatchMode(float) { return S_OK; }
    virtual float STDMETHODCALLTYPE GetNPatchMode(void) { return 0.0f; }
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveStrided(D3DPRIMITIVETYPE, UINT, UINT, UINT, UINT, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT, PALETTEENTRY*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT, PALETTEENTRY*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT) { return S_OK; }
    virtual UINT STDMETHODCALLTYPE GetCurrentTexturePalette(void) { return 0; }
    virtual HRESULT STDMETHODCALLTYPE SetFVF(DWORD) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetFVF(DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetStreamSource(UINT, IDirect3DVertexBuffer9*, UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetStreamSource(UINT, IDirect3DVertexBuffer9**, UINT*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT, DWORD) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetStreamSourceFreq(UINT, DWORD*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD, IDirect3DSurface9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetRenderTarget(DWORD, IDirect3DSurface9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface9*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface9**) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*) { return E_NOTIMPL; }
    virtual HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DeletePatch(UINT) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE CreateQuery(D3DQUERYTYPE, IDirect3DQuery9**) { return E_NOTIMPL; }

    virtual HRESULT STDMETHODCALLTYPE EndScene(void) {
        if (renderer) {
            renderer->RenderFrame();
        }
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE BeginScene(void) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE Clear(DWORD, D3DRECT*, DWORD, D3DCOLOR, float, DWORD) { return S_OK; }
};

// Main export function
extern "C" __declspec(dllexport) IDirect3D8* WINAPI Direct3DCreate8(UINT SDKVersion)
{
    OutputDebugStringA("[OFP-D3DX OpenGL] Direct3DCreate8 called\n");

    // Create OpenGL renderer
    OpenGLRenderer* renderer = new OpenGLRenderer();

    // Create D3D8 bridge
    CDirect3D8OpenGL* bridge = new CDirect3D8OpenGL(renderer);

    return bridge;
}

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        OutputDebugStringA("[OFP-D3DX OpenGL] DLL attached to process\n");
        break;
    case DLL_PROCESS_DETACH:
        OutputDebugStringA("[OFP-D3DX OpenGL] DLL detached from process\n");
        break;
    }
    return TRUE;
}