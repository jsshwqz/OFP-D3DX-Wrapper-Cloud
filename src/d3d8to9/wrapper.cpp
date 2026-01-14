#include "config.hpp"
#include <d3d9.h>
#include <initguid.h>
#include <string.h>

DEFINE_GUID(IID_IDirect3D8, 0x1DD9E8DA, 0x6C27, 0x41C8, 0xA7, 0xE7, 0x76, 0x15, 0xEE, 0xF7, 0x5C, 0x55);
DEFINE_GUID(IID_IDirect3DDevice8, 0x1B026D48, 0x1159, 0x4E94, 0xA1, 0xB9, 0xD8, 0x9F, 0xE7, 0x7A, 0xE9, 0x50);
DEFINE_GUID(IID_IDirect3DTexture8, 0xE13E509C, 0x9D33, 0x4B6D, 0xA7, 0xE7, 0x6E, 0xC5, 0x4F, 0x4D, 0xD0, 0x5A);

static LONG g_RefCount = 0;

interface IDirect3DTexture8;
interface IDirect3DSurface8;

MIDL_INTERFACE("9D3FDD92-E9C9-42D6-BE5B-8B2B5F7E2F8D")
IDirect3DTexture8 : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetDevice(void** ppDevice) = 0;
    virtual DWORD STDMETHODCALLTYPE SetPriority(DWORD Priority) = 0;
    virtual DWORD STDMETHODCALLTYPE GetPriority(void) = 0;
    virtual void STDMETHODCALLTYPE PreLoad(void) = 0;
    virtual DWORD STDMETHODCALLTYPE GetType(void) = 0;
    virtual DWORD STDMETHODCALLTYPE GetLevelCount(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLevelDesc(DWORD Level, void* pDesc) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSurfaceLevel(DWORD Level, void** ppSurfaceLevel) = 0;
    virtual HRESULT STDMETHODCALLTYPE LockRect(DWORD Level, void* pLockedRect, void* pRect, DWORD Flags) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnlockRect(DWORD Level) = 0;
};

MIDL_INTERFACE("36C5F7E4-7B9F-4E9E-B3F5-5B2E7D3C5E9A")
IDirect3DSurface8 : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetDevice(void** ppDevice) = 0;
    virtual DWORD STDMETHODCALLTYPE SetPriority(DWORD Priority) = 0;
    virtual DWORD STDMETHODCALLTYPE GetPriority(void) = 0;
    virtual void STDMETHODCALLTYPE PreLoad(void) = 0;
    virtual DWORD STDMETHODCALLTYPE GetType(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDesc(void* pDesc) = 0;
    virtual HRESULT STDMETHODCALLTYPE LockRect(void* pLockedRect, void* pRect, DWORD Flags) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnlockRect(void) = 0;
};

class CDirect3DTexture8 : public IDirect3DTexture8
{
private:
    IDirect3DTexture9* m_pTexture9;
    LONG m_RefCount;

public:
    CDirect3DTexture8(IDirect3DTexture9* pTexture9) : m_pTexture9(pTexture9), m_RefCount(1)
    {
        if (m_pTexture9)
            m_pTexture9->AddRef();
    }

    virtual ~CDirect3DTexture8()
    {
        if (m_pTexture9)
            m_pTexture9->Release();
    }

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
    {
        if (ppvObj == nullptr) return E_INVALIDARG;
        if (riid == IID_IUnknown || riid == IID_IDirect3DTexture8)
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
        if (RefCount == 0) delete this;
        return RefCount;
    }

    STDMETHOD(GetDevice)(void** ppDevice) { return E_NOTIMPL; }
    STDMETHOD_(DWORD, SetPriority)(DWORD Priority) { return m_pTexture9->SetPriority(Priority); }
    STDMETHOD_(DWORD, GetPriority)(void) { return m_pTexture9->GetPriority(); }
    STDMETHOD_(void, PreLoad)(void) { m_pTexture9->PreLoad(); }
    STDMETHOD_(DWORD, GetType)(void) { return D3DRTYPE_TEXTURE; }
    STDMETHOD_(DWORD, GetLevelCount)(void) { return m_pTexture9->GetLevelCount(); }
    STDMETHOD(GetLevelDesc)(DWORD Level, void* pDesc)
    {
        D3DSURFACE_DESC desc;
        HRESULT hr = m_pTexture9->GetLevelDesc(Level, &desc);
        if (SUCCEEDED(hr) && pDesc)
        {
            memset(pDesc, 0, sizeof(void*));
            ((D3DSURFACE_DESC*)pDesc)->Format = desc.Format;
            ((D3DSURFACE_DESC*)pDesc)->Type = desc.Type;
            ((D3DSURFACE_DESC*)pDesc)->Usage = desc.Usage;
            ((D3DSURFACE_DESC*)pDesc)->Pool = desc.Pool;
            ((D3DSURFACE_DESC*)pDesc)->Width = desc.Width;
            ((D3DSURFACE_DESC*)pDesc)->Height = desc.Height;
        }
        return hr;
    }

    STDMETHOD(GetSurfaceLevel)(DWORD Level, void** ppSurfaceLevel)
    {
        IDirect3DSurface9* pSurface9 = nullptr;
        HRESULT hr = m_pTexture9->GetSurfaceLevel(Level, &pSurface9);
        if (SUCCEEDED(hr) && pSurface9)
        {
            *ppSurfaceLevel = new CDirect3DSurface8(pSurface9);
            pSurface9->Release();
        }
        return hr;
    }

    STDMETHOD(LockRect)(DWORD Level, void* pLockedRect, void* pRect, DWORD Flags)
    {
        return m_pTexture9->LockRect(Level, (D3DLOCKED_RECT*)pLockedRect, (RECT*)pRect, Flags);
    }

    STDMETHOD(UnlockRect)(DWORD Level)
    {
        return m_pTexture9->UnlockRect(Level);
    }
};

class CDirect3DSurface8 : public IDirect3DSurface8
{
private:
    IDirect3DSurface9* m_pSurface9;
    LONG m_RefCount;

public:
    CDirect3DSurface8(IDirect3DSurface9* pSurface9) : m_pSurface9(pSurface9), m_RefCount(1)
    {
        if (m_pSurface9) m_pSurface9->AddRef();
    }

    virtual ~CDirect3DSurface8()
    {
        if (m_pSurface9) m_pSurface9->Release();
    }

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
    {
        if (ppvObj == nullptr) return E_INVALIDARG;
        if (riid == IID_IUnknown || riid == __uuidof(IDirect3DSurface8))
        {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)(THIS) { return InterlockedIncrement(&m_RefCount); }
    STDMETHOD_(ULONG, Release)(THIS)
    {
        ULONG RefCount = InterlockedDecrement(&m_RefCount);
        if (RefCount == 0) delete this;
        return RefCount;
    }

    STDMETHOD(GetDevice)(void** ppDevice) { return E_NOTIMPL; }
    STDMETHOD_(DWORD, SetPriority)(DWORD Priority) { return m_pSurface9->SetPriority(Priority); }
    STDMETHOD_(DWORD, GetPriority)(void) { return m_pSurface9->GetPriority(); }
    STDMETHOD_(void, PreLoad)(void) { m_pSurface9->PreLoad(); }
    STDMETHOD_(DWORD, GetType)(void) { return D3DRTYPE_SURFACE; }
    STDMETHOD(GetDesc)(void* pDesc)
    {
        D3DSURFACE_DESC desc;
        return m_pSurface9->GetDesc(&desc);
    }

    STDMETHOD(LockRect)(void* pLockedRect, void* pRect, DWORD Flags)
    {
        return m_pSurface9->LockRect((D3DLOCKED_RECT*)pLockedRect, (RECT*)pRect, Flags);
    }

    STDMETHOD(UnlockRect)(void)
    {
        return m_pSurface9->UnlockRect();
    }
};

MIDL_INTERFACE("A0E3E7D8-8C9A-4D7B-8B2E-5F7E2C3B4D6A")
IDirect3DDevice8 : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE TestCooperativeLevel(void) = 0;
    virtual UINT STDMETHODCALLTYPE GetAvailableTextureMem(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE ResourceManagerDiscardBytes(DWORD Bytes) = 0;
    virtual void* STDMETHODCALLTYPE GetDirect3D(void) = 0;
    virtual void STDMETHODCALLTYPE GetDeviceCaps(void* pCaps) = 0;
    virtual void STDMETHODCALLTYPE GetDisplayMode(void* pMode) = 0;
    virtual void STDMETHODCALLTYPE GetCreationParameters(void* pParameters) = 0;
    virtual void STDMETHODCALLTYPE CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, DWORD Format, DWORD Pool, void** ppTexture) = 0;
    virtual void STDMETHODCALLTYPE CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, DWORD Pool, void** ppVertexBuffer) = 0;
    virtual void STDMETHODCALLTYPE CreateIndexBuffer(UINT Length, DWORD Usage, DWORD Format, DWORD Pool, void** ppIndexBuffer) = 0;
    virtual void STDMETHODCALLTYPE SetRenderTarget(void* pRenderTarget, void* pNewZStencil) = 0;
    virtual void STDMETHODCALLTYPE GetRenderTarget(void** ppRenderTarget) = 0;
    virtual void STDMETHODCALLTYPE GetDepthStencilSurface(void** ppZStencilSurface) = 0;
    virtual HRESULT STDMETHODCALLTYPE BeginScene(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE EndScene(void) = 0;
    virtual void STDMETHODCALLTYPE Clear(DWORD Count, void* pRects, DWORD Flags, DWORD Color, float Z, DWORD Stencil) = 0;
    virtual void STDMETHODCALLTYPE SetViewport(void* pViewport) = 0;
    virtual void STDMETHODCALLTYPE GetViewport(void* pViewport) = 0;
    virtual void STDMETHODCALLTYPE SetMaterial(void* pMaterial) = 0;
    virtual void STDMETHODCALLTYPE GetMaterial(void* pMaterial) = 0;
    virtual void STDMETHODCALLTYPE SetLight(DWORD Index, void* pLight) = 0;
    virtual void STDMETHODCALLTYPE LightEnable(DWORD Index, BOOL Enable) = 0;
    virtual void STDMETHODCALLTYPE GetLightEnable(DWORD Index, BOOL* pEnable) = 0;
    virtual void STDMETHODCALLTYPE SetRenderState(DWORD State, DWORD Value) = 0;
    virtual void STDMETHODCALLTYPE GetRenderState(DWORD State, DWORD* pValue) = 0;
    virtual void STDMETHODCALLTYPE SetTexture(DWORD Stage, void* pTexture) = 0;
    virtual void* STDMETHODCALLTYPE GetTexture(DWORD Stage) = 0;
    virtual void STDMETHODCALLTYPE GetTextureStageState(DWORD Stage, DWORD Type, DWORD* pValue) = 0;
    virtual void STDMETHODCALLTYPE SetTextureStageState(DWORD Stage, DWORD Type, DWORD Value) = 0;
    virtual void STDMETHODCALLTYPE SetScissorRect(void* pRect) = 0;
    virtual void STDMETHODCALLTYPE GetScissorRect(void* pRect) = 0;
    virtual void STDMETHODCALLTYPE SetSoftwareVertexProcessing(BOOL Software) = 0;
    virtual BOOL STDMETHODCALLTYPE GetSoftwareVertexProcessing(void) = 0;
    virtual void STDMETHODCALLTYPE SetNPatchMode(float Segments) = 0;
    virtual float STDMETHODCALLTYPE GetNPatchMode(void) = 0;
    virtual void STDMETHODCALLTYPE DrawPrimitive(DWORD PrimitiveType, DWORD StartVertex, DWORD PrimitiveCount) = 0;
    virtual void STDMETHODCALLTYPE DrawIndexedPrimitive(DWORD PrimitiveType, DWORD MinIndex, DWORD NumVertices, DWORD StartIndex, DWORD PrimitiveCount) = 0;
    virtual void STDMETHODCALLTYPE DrawPrimitiveUP(DWORD PrimitiveType, DWORD PrimitiveCount, void* pVertexStreamZeroData, DWORD VertexStreamZeroStride) = 0;
    virtual void STDMETHODCALLTYPE DrawIndexedPrimitiveUP(DWORD PrimitiveType, DWORD MinVertexIndex, DWORD NumVertexIndices, DWORD PrimitiveCount, void* pIndexData, DWORD IndexDataFormat, void* pVertexStreamZeroData, DWORD VertexStreamZeroStride) = 0;
    virtual void STDMETHODCALLTYPE ProcessVertices(DWORD SrcStartIndex, DWORD DestIndex, DWORD VertexCount, void* pDestBuffer, DWORD Flags) = 0;
    virtual void STDMETHODCALLTYPE SetVertexShader(DWORD Handle) = 0;
    virtual DWORD STDMETHODCALLTYPE GetVertexShader(void) = 0;
    virtual void STDMETHODCALLTYPE SetVertexShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) = 0;
    virtual void STDMETHODCALLTYPE GetVertexShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) = 0;
    virtual void STDMETHODCALLTYPE DeleteVertexShader(DWORD Handle) = 0;
    virtual void STDMETHODCALLTYPE SetPixelShader(DWORD Handle) = 0;
    virtual DWORD STDMETHODCALLTYPE GetPixelShader(void) = 0;
    virtual void STDMETHODCALLTYPE SetPixelShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) = 0;
    virtual void STDMETHODCALLTYPE GetPixelShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) = 0;
    virtual void STDMETHODCALLTYPE DeletePixelShader(DWORD Handle) = 0;
    virtual void STDMETHODCALLTYPE GetGammaRamp(void* pRamp) = 0;
    virtual void STDMETHODCALLTYPE SetGammaRamp(DWORD Flags, void* pRamp) = 0;
    virtual void STDMETHODCALLTYPE UpdateTexture(void* pSourceTexture, void* pDestinationTexture) = 0;
    virtual void STDMETHODCALLTYPE GetFrontBuffer(void* pDestSurface) = 0;
    virtual void STDMETHODCALLTYPE GetBackBuffer(DWORD BackBuffer, DWORD Type, void** ppBackBuffer) = 0;
};

class CDirect3DDevice8 : public IDirect3DDevice8
{
private:
    IDirect3DDevice9* m_pDevice9;
    CDirect3D8* m_pD3D8;
    LONG m_RefCount;

public:
    CDirect3DDevice8(IDirect3DDevice9* pDevice9, CDirect3D8* pD3D8)
        : m_pDevice9(pDevice9), m_pD3D8(pD3D8), m_RefCount(1)
    {
        if (m_pD3D8) m_pD3D8->AddRef();
        if (m_pDevice9) m_pDevice9->AddRef();
    }

    virtual ~CDirect3DDevice8()
    {
        if (m_pDevice9) m_pDevice9->Release();
        if (m_pD3D8) m_pD3D8->Release();
    }

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
    {
        if (ppvObj == nullptr) return E_INVALIDARG;
        if (riid == IID_IUnknown || riid == IID_IDirect3DDevice8)
        {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)(THIS) { return InterlockedIncrement(&m_RefCount); }
    STDMETHOD_(ULONG, Release)(THIS)
    {
        ULONG RefCount = InterlockedDecrement(&m_RefCount);
        if (RefCount == 0) delete this;
        return RefCount;
    }

    STDMETHOD(TestCooperativeLevel)(void) { return m_pDevice9->TestCooperativeLevel(); }
    STDMETHOD_(UINT, GetAvailableTextureMem)(void) { return m_pDevice9->GetAvailableTextureMem(); }
    STDMETHOD(ResourceManagerDiscardBytes)(DWORD Bytes) { return m_pDevice9->EvictManagedResources(); }
    STDMETHOD_(void*, GetDirect3D)(void) { return m_pD3D8; }
    STDMETHOD(GetDeviceCaps)(void* pCaps) { return m_pDevice9->GetDeviceCaps((D3DCAPS9*)pCaps); }
    STDMETHOD(GetDisplayMode)(void* pMode) { return m_pDevice9->GetDisplayMode((D3DDISPLAYMODE*)pMode); }
    STDMETHOD(GetCreationParameters)(void* pParameters) { return m_pDevice9->GetCreationParameters((D3DDEVICE_CREATION_PARAMETERS*)pParameters); }

    STDMETHOD(CreateTexture)(UINT Width, UINT Height, UINT Levels, DWORD Usage, DWORD Format, DWORD Pool, void** ppTexture)
    {
        IDirect3DTexture9* pTex9 = nullptr;
        DWORD d3d9Usage = Usage;
        D3DPOOL d3d9Pool = (D3DPOOL)Pool;

        if (Config::g_Config.AnisotropicFiltering > 1 && d3d9Usage == 0)
            d3d9Usage = D3DUSAGE_DYNAMIC;

        if (Pool == D3DPOOL_DEFAULT && Usage == 0)
            d3d9Pool = D3DPOOL_MANAGED;

        UINT levels = (Levels == 0) ? 1 : Levels;

        HRESULT hr = m_pDevice9->CreateTexture(Width, Height, levels, d3d9Usage, (D3DFORMAT)Format, d3d9Pool, &pTex9, nullptr);
        if (SUCCEEDED(hr) && pTex9 && ppTexture)
        {
            *ppTexture = new CDirect3DTexture8(pTex9);
            pTex9->Release();
        }
        return hr;
    }

    STDMETHOD(CreateVertexBuffer)(UINT Length, DWORD Usage, DWORD FVF, DWORD Pool, void** ppVertexBuffer)
    {
        IDirect3DVertexBuffer9* pVB9 = nullptr;
        D3DPOOL d3d9Pool = (D3DPOOL)Pool;
        if (Pool == D3DPOOL_DEFAULT && Usage == 0)
            d3d9Pool = D3DPOOL_MANAGED;

        return m_pDevice9->CreateVertexBuffer(Length, Usage & ~D3DUSAGE_WRITEONLY, FVF, d3d9Pool, &pVB9, nullptr);
    }

    STDMETHOD(CreateIndexBuffer)(UINT Length, DWORD Usage, DWORD Format, DWORD Pool, void** ppIndexBuffer)
    {
        D3DPOOL d3d9Pool = (D3DPOOL)Pool;
        if (Pool == D3DPOOL_DEFAULT && Usage == 0)
            d3d9Pool = D3DPOOL_MANAGED;

        D3DFORMAT d3d9Format = (Format == 21) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
        return m_pDevice9->CreateIndexBuffer(Length, Usage & ~D3DUSAGE_WRITEONLY, d3d9Format, d3d9Pool, (IDirect3DIndexBuffer9**)ppVertexBuffer, nullptr);
    }

    STDMETHOD(SetRenderTarget)(void* pRenderTarget, void* pNewZStencil)
    {
        return m_pDevice9->SetRenderTarget((IDirect3DSurface9*)pRenderTarget, (IDirect3DSurface9*)pNewZStencil);
    }

    STDMETHOD(GetRenderTarget)(void** ppRenderTarget)
    {
        IDirect3DSurface9* pRT9 = nullptr;
        HRESULT hr = m_pDevice9->GetRenderTarget(&pRT9);
        if (SUCCEEDED(hr) && pRT9)
        {
            *ppRenderTarget = new CDirect3DSurface8(pRT9);
            pRT9->Release();
        }
        return hr;
    }

    STDMETHOD(GetDepthStencilSurface)(void** ppZStencilSurface)
    {
        IDirect3DSurface9* pZS9 = nullptr;
        HRESULT hr = m_pDevice9->GetDepthStencilSurface(&pZS9);
        if (SUCCEEDED(hr) && pZS9)
        {
            *ppZStencilSurface = new CDirect3DSurface8(pZS9);
            pZS9->Release();
        }
        return hr;
    }

    STDMETHOD(BeginScene)(void) { return m_pDevice9->BeginScene(); }
    STDMETHOD(EndScene)(void) { return m_pDevice9->EndScene(); }
    STDMETHOD(Clear)(DWORD Count, void* pRects, DWORD Flags, DWORD Color, float Z, DWORD Stencil)
    {
        return m_pDevice9->Clear(Count, (D3DRECT*)pRects, Flags, Color, Z, Stencil);
    }

    STDMETHOD(SetViewport)(void* pViewport)
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

    STDMETHOD(GetViewport)(void* pViewport)
    {
        D3DVIEWPORT9 vp;
        HRESULT hr = m_pDevice9->GetViewport(&vp);
        if (SUCCEEDED(hr) && pViewport)
        {
            ((D3DVIEWPORT8*)pViewport)->X = vp.X;
            ((D3DVIEWPORT8*)pViewport)->Y = vp.Y;
            ((D3DVIEWPORT8*)pViewport)->Width = vp.Width;
            ((D3DVIEWPORT8*)pViewport)->Height = vp.Height;
            ((D3DVIEWPORT8*)pViewport)->MinZ = vp.MinZ;
            ((D3DVIEWPORT8*)pViewport)->MaxZ = vp.MaxZ;
        }
        return hr;
    }

    STDMETHOD(SetMaterial)(void* pMaterial)
    {
        D3DMATERIAL9 mat9;
        D3DMATERIAL8* pMat8 = (D3DMATERIAL8*)pMaterial;
        mat9.Diffuse = pMat8->diffuse;
        mat9.Ambient = pMat8->ambient;
        mat9.Specular = pMat8->specular;
        mat9.Emissive = pMat8->emissive;
        mat9.Power = pMat8->Power;
        return m_pDevice9->SetMaterial(&mat9);
    }

    STDMETHOD(GetMaterial)(void* pMaterial)
    {
        D3DMATERIAL9 mat9;
        HRESULT hr = m_pDevice9->GetMaterial(&mat9);
        if (SUCCEEDED(hr) && pMaterial)
        {
            ((D3DMATERIAL8*)pMaterial)->diffuse = mat9.Diffuse;
            ((D3DMATERIAL8*)pMaterial)->ambient = mat9.Ambient;
            ((D3DMATERIAL8*)pMaterial)->specular = mat9.Specular;
            ((D3DMATERIAL8*)pMaterial)->emissive = mat9.Emissive;
            ((D3DMATERIAL8*)pMaterial)->Power = mat9.Power;
        }
        return hr;
    }

    STDMETHOD(SetLight)(DWORD Index, void* pLight)
    {
        D3DLIGHT9 light9;
        D3DLIGHT8* pLight8 = (D3DLIGHT8*)pLight;
        light9.Type = (D3DLIGHTTYPE)pLight8->Type;
        light9.Diffuse = pLight8->diffuse;
        light9.Specular = pLight8->specular;
        light9.Ambient = pLight8->ambient;
        light9.Position = pLight8->Position;
        light9.Direction = pLight8->Direction;
        light9.Range = pLight8->Range;
        light9.Falloff = pLight8->Falloff;
        light9.Attenuation0 = pLight8->Attenuation0;
        light9.Attenuation1 = pLight8->Attenuation1;
        light9.Attenuation2 = pLight8->Attenuation2;
        light9.Theta = pLight8->Theta;
        light9.Phi = pLight8->Phi;
        return m_pDevice9->SetLight(Index, &light9);
    }

    STDMETHOD(LightEnable)(DWORD Index, BOOL Enable) { return m_pDevice9->LightEnable(Index, Enable); }
    STDMETHOD(GetLightEnable)(DWORD Index, BOOL* pEnable) { return m_pDevice9->LightEnable(Index, pEnable); }
    STDMETHOD(SetRenderState)(DWORD State, DWORD Value) { return m_pDevice9->SetRenderState((D3DRENDERSTATETYPE)State, Value); }
    STDMETHOD(GetRenderState)(DWORD State, DWORD* pValue) { return m_pDevice9->GetRenderState((D3DRENDERSTATETYPE)State, pValue); }

    STDMETHOD(SetTexture)(DWORD Stage, void* pTexture)
    {
        if (Config::g_Config.AnisotropicFiltering > 1 && pTexture != nullptr)
        {
            m_pDevice9->SetSamplerState(Stage, D3DSAMP_MAXANISOTROPY, Config::g_Config.AnisotropicFiltering);
            m_pDevice9->SetSamplerState(Stage, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
            m_pDevice9->SetSamplerState(Stage, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
        }
        return m_pDevice9->SetTexture(Stage, (IDirect3DBaseTexture9*)pTexture);
    }

    STDMETHOD_(void*, GetTexture)(DWORD Stage)
    {
        IDirect3DBaseTexture9* pTex9 = nullptr;
        m_pDevice9->GetTexture(Stage, &pTex9);
        if (pTex9 && pTex9->GetType() == D3DRTYPE_TEXTURE)
        {
            void* pResult = new CDirect3DTexture8((IDirect3DTexture9*)pTex9);
            pTex9->Release();
            return pResult;
        }
        if (pTex9) pTex9->Release();
        return nullptr;
    }

    STDMETHOD(GetTextureStageState)(DWORD Stage, DWORD Type, DWORD* pValue) { return m_pDevice9->GetTextureStageState(Stage, (D3DTEXTURESTAGESTATETYPE)Type, pValue); }
    STDMETHOD(SetTextureStageState)(DWORD Stage, DWORD Type, DWORD Value) { return m_pDevice9->SetTextureStageState(Stage, (D3DTEXTURESTAGESTATETYPE)Type, Value); }
    STDMETHOD(SetScissorRect)(void* pRect) { return m_pDevice9->SetScissorRect((RECT*)pRect); }
    STDMETHOD(GetScissorRect)(void* pRect) { return m_pDevice9->GetScissorRect((RECT*)pRect); }
    STDMETHOD(SetSoftwareVertexProcessing)(BOOL Software) { return m_pDevice9->SetSoftwareVertexProcessing(Software); }
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(void) { return m_pDevice9->GetSoftwareVertexProcessing(); }
    STDMETHOD(SetNPatchMode)(float Segments) { return m_pDevice9->SetNPatchMode(Segments); }
    STDMETHOD_(float, GetNPatchMode)(void) { return m_pDevice9->GetNPatchMode(); }
    STDMETHOD(DrawPrimitive)(DWORD PrimitiveType, DWORD StartVertex, DWORD PrimitiveCount) { return m_pDevice9->DrawPrimitive((D3DPRIMITIVETYPE)PrimitiveType, StartVertex, PrimitiveCount); }
    STDMETHOD(DrawIndexedPrimitive)(DWORD PrimitiveType, DWORD MinIndex, DWORD NumVertices, DWORD StartIndex, DWORD PrimitiveCount) { return m_pDevice9->DrawIndexedPrimitive((D3DPRIMITIVETYPE)PrimitiveType, MinIndex, NumVertices, StartIndex, PrimitiveCount); }
    STDMETHOD(DrawPrimitiveUP)(DWORD PrimitiveType, DWORD PrimitiveCount, void* pVertexStreamZeroData, DWORD VertexStreamZeroStride) { return m_pDevice9->DrawPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
    STDMETHOD(DrawIndexedPrimitiveUP)(DWORD PrimitiveType, DWORD MinVertexIndex, DWORD NumVertexIndices, DWORD PrimitiveCount, void* pIndexData, DWORD IndexDataFormat, void* pVertexStreamZeroData, DWORD VertexStreamZeroStride) { return m_pDevice9->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, (D3DFORMAT)IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
    STDMETHOD(SetVertexShader)(DWORD Handle) { return m_pDevice9->SetVertexShader(Handle); }
    STDMETHOD_(DWORD, GetVertexShader)(void) { return m_pDevice9->GetVertexShader(); }
    STDMETHOD(SetVertexShaderConstant)(DWORD Register, void* pConstantData, DWORD ConstantCount) { return m_pDevice9->SetVertexShaderConstant(Register, pConstantData, ConstantCount); }
    STDMETHOD(GetVertexShaderConstant)(DWORD Register, void* pConstantData, DWORD ConstantCount) { return m_pDevice9->GetVertexShaderConstant(Register, pConstantData, ConstantCount); }
    STDMETHOD(DeleteVertexShader)(DWORD Handle) { return m_pDevice9->DeleteVertexShader(Handle); }
    STDMETHOD(SetPixelShader)(DWORD Handle) { return m_pDevice9->SetPixelShader(Handle); }
    STDMETHOD_(DWORD, GetPixelShader)(void) { return m_pDevice9->GetPixelShader(); }
    STDMETHOD(SetPixelShaderConstant)(DWORD Register, void* pConstantData, DWORD ConstantCount) { return m_pDevice9->SetPixelShaderConstant(Register, pConstantData, ConstantCount); }
    STDMETHOD(GetPixelShaderConstant)(DWORD Register, void* pConstantData, DWORD ConstantCount) { return m_pDevice9->GetPixelShaderConstant(Register, pConstantData, ConstantCount); }
    STDMETHOD(DeletePixelShader)(DWORD Handle) { return m_pDevice9->DeletePixelShader(Handle); }
    STDMETHOD(GetGammaRamp)(void* pRamp) { m_pDevice9->GetGammaRamp((D3DGAMMARAMP*)pRamp); return D3D_OK; }
    STDMETHOD(SetGammaRamp)(DWORD Flags, void* pRamp) { m_pDevice9->SetGammaRamp(Flags, (D3DGAMMARAMP*)pRamp); return D3D_OK; }
    STDMETHOD(UpdateTexture)(void* pSourceTexture, void* pDestinationTexture) { return m_pDevice9->UpdateTexture((IDirect3DBaseTexture9*)pSourceTexture, (IDirect3DBaseTexture9*)pDestinationTexture); }
    STDMETHOD(GetFrontBuffer)(void* pDestSurface) { return m_pDevice9->GetFrontBufferData(0, (IDirect3DSurface9*)pDestSurface); }
    STDMETHOD(GetBackBuffer)(DWORD BackBuffer, DWORD Type, void** ppBackBuffer)
    {
        IDirect3DSurface9* pBB9 = nullptr;
        D3DBACKBUFFER_TYPE type9 = (Type == 0) ? D3DBACKBUFFER_TYPE_MONO : D3DBACKBUFFER_TYPE_UNKNOWN;
        HRESULT hr = m_pDevice9->GetBackBuffer(0, BackBuffer, type9, &pBB9);
        if (SUCCEEDED(hr) && pBB9)
        {
            *ppBackBuffer = new CDirect3DSurface8(pBB9);
            pBB9->Release();
        }
        return hr;
    }
};

class CDirect3D8 : public IUnknown
{
private:
    IDirect3D9* m_pD3D9;

public:
    CDirect3D8(IDirect3D9* pD3D9) : m_pD3D9(pD3D9)
    {
        if (m_pD3D9) m_pD3D9->AddRef();
    }

    virtual ~CDirect3D8()
    {
        if (m_pD3D9) m_pD3D9->Release();
    }

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
    {
        if (ppvObj == nullptr) return E_INVALIDARG;
        if (riid == IID_IUnknown || riid == IID_IDirect3D8)
        {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)(THIS) { return InterlockedIncrement(&g_RefCount); }
    STDMETHOD_(ULONG, Release)(THIS)
    {
        ULONG RefCount = InterlockedDecrement(&g_RefCount);
        if (RefCount == 0) delete this;
        return RefCount;
    }

    STDMETHOD(RegisterSoftwareDevice)(void* pInitializeFunction) { return m_pD3D9->RegisterSoftwareDevice(pInitializeFunction); }
    STDMETHOD_(UINT, GetAdapterCount)(void) { return m_pD3D9->GetAdapterCount(); }
    STDMETHOD(GetAdapterIdentifier)(UINT Adapter, DWORD Flags, void* pIdentifier) { return m_pD3D9->GetAdapterIdentifier(Adapter, Flags, (D3DADAPTER_IDENTIFIER9*)pIdentifier); }
    STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter) { return m_pD3D9->GetAdapterModeCount(Adapter, D3DFMT_UNKNOWN); }
    STDMETHOD(EnumAdapterModes)(UINT Adapter, UINT Mode, void* pMode) { return m_pD3D9->EnumAdapterModes(Adapter, D3DFMT_UNKNOWN, Mode, (D3DDISPLAYMODE*)pMode); }
    STDMETHOD(GetAdapterDisplayMode)(UINT Adapter, void* pMode) { return m_pD3D9->GetAdapterDisplayMode(Adapter, (D3DDISPLAYMODE*)pMode); }
    STDMETHOD(CheckDeviceType)(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) { return m_pD3D9->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed); }
    STDMETHOD(CheckDeviceFormat)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) { return m_pD3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat); }
    STDMETHOD(CheckDeviceMultiSampleType)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType) { return m_pD3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, nullptr); }
    STDMETHOD(CheckDepthStencilMatch)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) { return m_pD3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }
    STDMETHOD(GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, void* pCaps) { return m_pD3D9->GetDeviceCaps(Adapter, DeviceType, (D3DCAPS9*)pCaps); }
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter) { return m_pD3D9->GetAdapterMonitor(Adapter); }

    STDMETHOD(CreateDevice)(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, void* pPresentationParameters, void** ppReturnedDeviceInterface)
    {
        if (ppReturnedDeviceInterface == nullptr) return D3DERR_INVALIDCALL;

        D3DPRESENT_PARAMETERS pp9 = {0};
        if (pPresentationParameters)
        {
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
        }

        pp9.FullScreen_PresentationInterval = Config::g_Config.EnableVSync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

        IDirect3DDevice9* pDevice9 = nullptr;
        HRESULT hr = m_pD3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, &pp9, &pDevice9);
        if (SUCCEEDED(hr) && pDevice9)
        {
            *ppReturnedDeviceInterface = new CDirect3DDevice8(pDevice9, this);
            pDevice9->Release();
        }
        return hr;
    }
};

extern "C" __declspec(dllexport) IDirect3D8* __cdecl CreateD3D8Wrapper(IDirect3D9* pD3D9)
{
    if (pD3D9 == nullptr) return nullptr;
    return new CDirect3D8(pD3D9);
}
