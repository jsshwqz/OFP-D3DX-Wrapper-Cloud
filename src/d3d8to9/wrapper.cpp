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

interface IDirect3DTexture8 : public IUnknown
{
public:
    virtual void* STDMETHODCALLTYPE GetDevice(void) = 0;
    virtual DWORD STDMETHODCALLTYPE SetPriority(DWORD Priority) = 0;
    virtual DWORD STDMETHODCALLTYPE GetPriority(void) = 0;
    virtual void STDMETHODCALLTYPE PreLoad(void) = 0;
    virtual DWORD STDMETHODCALLTYPE GetType(void) = 0;
    virtual DWORD STDMETHODCALLTYPE GetLevelCount(void) = 0;
    virtual void STDMETHODCALLTYPE GetLevelDesc(DWORD Level, void* pDesc) = 0;
    virtual void STDMETHODCALLTYPE GetSurfaceLevel(DWORD Level, void** ppSurfaceLevel) = 0;
    virtual void STDMETHODCALLTYPE LockRect(DWORD Level, void* pLockedRect, void* pRect, DWORD Flags) = 0;
    virtual void STDMETHODCALLTYPE UnlockRect(DWORD Level) = 0;
};

interface IDirect3DSurface8 : public IUnknown
{
public:
    virtual void* STDMETHODCALLTYPE GetDevice(void) = 0;
    virtual DWORD STDMETHODCALLTYPE SetPriority(DWORD Priority) = 0;
    virtual DWORD STDMETHODCALLTYPE GetPriority(void) = 0;
    virtual void STDMETHODCALLTYPE PreLoad(void) = 0;
    virtual DWORD STDMETHODCALLTYPE GetType(void) = 0;
    virtual void STDMETHODCALLTYPE GetDesc(void* pDesc) = 0;
    virtual void STDMETHODCALLTYPE LockRect(void* pLockedRect, void* pRect, DWORD Flags) = 0;
    virtual void STDMETHODCALLTYPE UnlockRect(void) = 0;
};

class CDirect3DTexture8 : public IDirect3DTexture8
{
private:
    IDirect3DTexture9* m_pTexture9;
    LONG m_RefCount;

public:
    CDirect3DTexture8(IDirect3DTexture9* pTexture9) : m_pTexture9(pTexture9), m_RefCount(1)
    {
        if (m_pTexture9) m_pTexture9->AddRef();
    }

    virtual ~CDirect3DTexture8()
    {
        if (m_pTexture9) m_pTexture9->Release();
    }

    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
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

    STDMETHOD_(ULONG, AddRef)(void) { return InterlockedIncrement(&m_RefCount); }
    STDMETHOD_(ULONG, Release)(void)
    {
        ULONG RefCount = InterlockedDecrement(&m_RefCount);
        if (RefCount == 0) delete this;
        return RefCount;
    }

    STDMETHOD_(void*, GetDevice)(void) { return nullptr; }
    STDMETHOD_(DWORD, SetPriority)(DWORD Priority) { return m_pTexture9->SetPriority(Priority); }
    STDMETHOD_(DWORD, GetPriority)(void) { return m_pTexture9->GetPriority(); }
    STDMETHOD_(void, PreLoad)(void) { m_pTexture9->PreLoad(); }
    STDMETHOD_(DWORD, GetType)(void) { return D3DRTYPE_TEXTURE; }
    STDMETHOD_(DWORD, GetLevelCount)(void) { return m_pTexture9->GetLevelCount(); }
    STDMETHOD_(void, GetLevelDesc)(DWORD Level, void* pDesc)
    {
        D3DSURFACE_DESC desc;
        m_pTexture9->GetLevelDesc(Level, &desc);
    }

    STDMETHOD_(void, GetSurfaceLevel)(DWORD Level, void** ppSurfaceLevel)
    {
        IDirect3DSurface9* pSurface9 = nullptr;
        m_pTexture9->GetSurfaceLevel(Level, &pSurface9);
        if (pSurface9)
        {
            *ppSurfaceLevel = new CDirect3DSurface8(pSurface9);
            pSurface9->Release();
        }
    }

    STDMETHOD_(void, LockRect)(DWORD Level, void* pLockedRect, void* pRect, DWORD Flags)
    {
        m_pTexture9->LockRect(Level, (D3DLOCKED_RECT*)pLockedRect, (RECT*)pRect, Flags);
    }

    STDMETHOD_(void, UnlockRect)(DWORD Level)
    {
        m_pTexture9->UnlockRect(Level);
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

    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
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

    STDMETHOD_(ULONG, AddRef)(void) { return InterlockedIncrement(&m_RefCount); }
    STDMETHOD_(ULONG, Release)(void)
    {
        ULONG RefCount = InterlockedDecrement(&m_RefCount);
        if (RefCount == 0) delete this;
        return RefCount;
    }

    STDMETHOD_(void*, GetDevice)(void) { return nullptr; }
    STDMETHOD_(DWORD, SetPriority)(DWORD Priority) { return m_pSurface9->SetPriority(Priority); }
    STDMETHOD_(DWORD, GetPriority)(void) { return m_pSurface9->GetPriority(); }
    STDMETHOD_(void, PreLoad)(void) { m_pSurface9->PreLoad(); }
    STDMETHOD_(DWORD, GetType)(void) { return D3DRTYPE_SURFACE; }
    STDMETHOD_(void, GetDesc)(void* pDesc)
    {
        D3DSURFACE_DESC desc;
        m_pSurface9->GetDesc(&desc);
    }

    STDMETHOD_(void, LockRect)(void* pLockedRect, void* pRect, DWORD Flags)
    {
        m_pSurface9->LockRect((D3DLOCKED_RECT*)pLockedRect, (RECT*)pRect, Flags);
    }

    STDMETHOD_(void, UnlockRect)(void)
    {
        m_pSurface9->UnlockRect();
    }
};

interface IDirect3DDevice8 : public IUnknown
{
public:
    virtual void STDMETHODCALLTYPE TestCooperativeLevel(void) = 0;
    virtual void STDMETHODCALLTYPE GetAvailableTextureMem(void) = 0;
    virtual void STDMETHODCALLTYPE ResourceManagerDiscardBytes(DWORD Bytes) = 0;
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
    virtual void STDMETHODCALLTYPE BeginScene(void) = 0;
    virtual void STDMETHODCALLTYPE EndScene(void) = 0;
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
    void* m_pD3D8;
    LONG m_RefCount;

public:
    CDirect3DDevice8(IDirect3DDevice9* pDevice9, void* pD3D8) : m_pDevice9(pDevice9), m_pD3D8(pD3D8), m_RefCount(1)
    {
        if (m_pDevice9) m_pDevice9->AddRef();
    }

    virtual ~CDirect3DDevice8()
    {
        if (m_pDevice9) m_pDevice9->Release();
    }

    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
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

    STDMETHOD_(ULONG, AddRef)(void) { return InterlockedIncrement(&m_RefCount); }
    STDMETHOD_(ULONG, Release)(void)
    {
        ULONG RefCount = InterlockedDecrement(&m_RefCount);
        if (RefCount == 0) delete this;
        return RefCount;
    }

    STDMETHOD_(void, TestCooperativeLevel)(void) { m_pDevice9->TestCooperativeLevel(); }
    STDMETHOD_(void, GetAvailableTextureMem)(void) { m_pDevice9->GetAvailableTextureMem(); }
    STDMETHOD_(void, ResourceManagerDiscardBytes)(DWORD Bytes) { m_pDevice9->EvictManagedResources(); }
    STDMETHOD_(void*, GetDirect3D)(void) { return m_pD3D8; }
    STDMETHOD_(void, GetDeviceCaps)(void* pCaps) { m_pDevice9->GetDeviceCaps((D3DCAPS9*)pCaps); }
    STDMETHOD_(void, GetDisplayMode)(void* pMode) { m_pDevice9->GetDisplayMode((D3DDISPLAYMODE*)pMode); }
    STDMETHOD_(void, GetCreationParameters)(void* pParameters) { m_pDevice9->GetCreationParameters((D3DDEVICE_CREATION_PARAMETERS*)pParameters); }

    STDMETHOD_(void, CreateTexture)(UINT Width, UINT Height, UINT Levels, DWORD Usage, DWORD Format, DWORD Pool, void** ppTexture)
    {
        IDirect3DTexture9* pTex9 = nullptr;
        DWORD d3d9Usage = Usage;
        D3DPOOL d3d9Pool = (D3DPOOL)Pool;

        if (Config::g_Config.AnisotropicFiltering > 1 && d3d9Usage == 0)
            d3d9Usage = D3DUSAGE_DYNAMIC;

        if (Pool == D3DPOOL_DEFAULT && Usage == 0)
            d3d9Pool = D3DPOOL_MANAGED;

        UINT levels = (Levels == 0) ? 1 : Levels;

        m_pDevice9->CreateTexture(Width, Height, levels, d3d9Usage, (D3DFORMAT)Format, d3d9Pool, &pTex9, nullptr);
        if (pTex9 && ppTexture)
        {
            *ppTexture = new CDirect3DTexture8(pTex9);
            pTex9->Release();
        }
    }

    STDMETHOD_(void, CreateVertexBuffer)(UINT Length, DWORD Usage, DWORD FVF, DWORD Pool, void** ppVertexBuffer)
    {
        D3DPOOL d3d9Pool = (D3DPOOL)Pool;
        if (Pool == D3DPOOL_DEFAULT && Usage == 0)
            d3d9Pool = D3DPOOL_MANAGED;
        m_pDevice9->CreateVertexBuffer(Length, Usage & ~D3DUSAGE_WRITEONLY, FVF, d3d9Pool, (IDirect3DVertexBuffer9**)ppVertexBuffer, nullptr);
    }

    STDMETHOD_(void, CreateIndexBuffer)(UINT Length, DWORD Usage, DWORD Format, DWORD Pool, void** ppIndexBuffer)
    {
        D3DPOOL d3d9Pool = (D3DPOOL)Pool;
        if (Pool == D3DPOOL_DEFAULT && Usage == 0)
            d3d9Pool = D3DPOOL_MANAGED;
        D3DFORMAT d3d9Format = (Format == 21) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
        m_pDevice9->CreateIndexBuffer(Length, Usage & ~D3DUSAGE_WRITEONLY, d3d9Format, d3d9Pool, (IDirect3DIndexBuffer9**)ppIndexBuffer, nullptr);
    }

    STDMETHOD_(void, SetRenderTarget)(void* pRenderTarget, void* pNewZStencil)
    {
        m_pDevice9->SetRenderTarget((IDirect3DSurface9*)pRenderTarget, (IDirect3DSurface9*)pNewZStencil);
    }

    STDMETHOD_(void, GetRenderTarget)(void** ppRenderTarget)
    {
        IDirect3DSurface9* pRT9 = nullptr;
        m_pDevice9->GetRenderTarget(&pRT9);
        if (pRT9)
        {
            *ppRenderTarget = new CDirect3DSurface8(pRT9);
            pRT9->Release();
        }
    }

    STDMETHOD_(void, GetDepthStencilSurface)(void** ppZStencilSurface)
    {
        IDirect3DSurface9* pZS9 = nullptr;
        m_pDevice9->GetDepthStencilSurface(&pZS9);
        if (pZS9)
        {
            *ppZStencilSurface = new CDirect3DSurface8(pZS9);
            pZS9->Release();
        }
    }

    STDMETHOD_(void, BeginScene)(void) { m_pDevice9->BeginScene(); }
    STDMETHOD_(void, EndScene)(void) { m_pDevice9->EndScene(); }
    STDMETHOD_(void, Clear)(DWORD Count, void* pRects, DWORD Flags, DWORD Color, float Z, DWORD Stencil)
    {
        m_pDevice9->Clear(Count, (D3DRECT*)pRects, Flags, Color, Z, Stencil);
    }

    STDMETHOD_(void, SetViewport)(void* pViewport)
    {
        D3DVIEWPORT9 vp;
        vp.X = ((D3DVIEWPORT8*)pViewport)->X;
        vp.Y = ((D3DVIEWPORT8*)pViewport)->Y;
        vp.Width = ((D3DVIEWPORT8*)pViewport)->Width;
        vp.Height = ((D3DVIEWPORT8*)pViewport)->Height;
        vp.MinZ = ((D3DVIEWPORT8*)pViewport)->MinZ;
        vp.MaxZ = ((D3DVIEWPORT8*)pViewport)->MaxZ;
        m_pDevice9->SetViewport(&vp);
    }

    STDMETHOD_(void, GetViewport)(void* pViewport)
    {
        D3DVIEWPORT9 vp;
        m_pDevice9->GetViewport(&vp);
        ((D3DVIEWPORT8*)pViewport)->X = vp.X;
        ((D3DVIEWPORT8*)pViewport)->Y = vp.Y;
        ((D3DVIEWPORT8*)pViewport)->Width = vp.Width;
        ((D3DVIEWPORT8*)pViewport)->Height = vp.Height;
        ((D3DVIEWPORT8*)pViewport)->MinZ = vp.MinZ;
        ((D3DVIEWPORT8*)pViewport)->MaxZ = vp.MaxZ;
    }

    STDMETHOD_(void, SetMaterial)(void* pMaterial)
    {
        D3DMATERIAL9 mat9;
        D3DMATERIAL8* pMat8 = (D3DMATERIAL8*)pMaterial;
        mat9.Diffuse = pMat8->diffuse;
        mat9.Ambient = pMat8->ambient;
        mat9.Specular = pMat8->specular;
        mat9.Emissive = pMat8->emissive;
        mat9.Power = pMat8->Power;
        m_pDevice9->SetMaterial(&mat9);
    }

    STDMETHOD_(void, GetMaterial)(void* pMaterial)
    {
        D3DMATERIAL9 mat9;
        m_pDevice9->GetMaterial(&mat9);
        ((D3DMATERIAL8*)pMaterial)->diffuse = mat9.Diffuse;
        ((D3DMATERIAL8*)pMaterial)->ambient = mat9.Ambient;
        ((D3DMATERIAL8*)pMaterial)->specular = mat9.Specular;
        ((D3DMATERIAL8*)pMaterial)->emissive = mat9.Emissive;
        ((D3DMATERIAL8*)pMaterial)->Power = mat9.Power;
    }

    STDMETHOD_(void, SetLight)(DWORD Index, void* pLight)
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
        m_pDevice9->SetLight(Index, &light9);
    }

    STDMETHOD_(void, LightEnable)(DWORD Index, BOOL Enable) { m_pDevice9->LightEnable(Index, Enable); }
    STDMETHOD_(void, GetLightEnable)(DWORD Index, BOOL* pEnable) { m_pDevice9->LightEnable(Index, pEnable); }
    STDMETHOD_(void, SetRenderState)(DWORD State, DWORD Value) { m_pDevice9->SetRenderState((D3DRENDERSTATETYPE)State, Value); }
    STDMETHOD_(void, GetRenderState)(DWORD State, DWORD* pValue) { m_pDevice9->GetRenderState((D3DRENDERSTATETYPE)State, pValue); }

    STDMETHOD_(void, SetTexture)(DWORD Stage, void* pTexture)
    {
        if (Config::g_Config.AnisotropicFiltering > 1 && pTexture != nullptr)
        {
            m_pDevice9->SetSamplerState(Stage, D3DSAMP_MAXANISOTROPY, Config::g_Config.AnisotropicFiltering);
            m_pDevice9->SetSamplerState(Stage, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
            m_pDevice9->SetSamplerState(Stage, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
        }
        m_pDevice9->SetTexture(Stage, (IDirect3DBaseTexture9*)pTexture);
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

    STDMETHOD_(void, GetTextureStageState)(DWORD Stage, DWORD Type, DWORD* pValue) { m_pDevice9->GetTextureStageState(Stage, (D3DTEXTURESTAGESTATETYPE)Type, pValue); }
    STDMETHOD_(void, SetTextureStageState)(DWORD Stage, DWORD Type, DWORD Value) { m_pDevice9->SetTextureStageState(Stage, (D3DTEXTURESTAGESTATETYPE)Type, Value); }
    STDMETHOD_(void, SetScissorRect)(void* pRect) { m_pDevice9->SetScissorRect((RECT*)pRect); }
    STDMETHOD_(void, GetScissorRect)(void* pRect) { m_pDevice9->GetScissorRect((RECT*)pRect); }
    STDMETHOD_(void, SetSoftwareVertexProcessing)(BOOL Software) { m_pDevice9->SetSoftwareVertexProcessing(Software); }
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(void) { return m_pDevice9->GetSoftwareVertexProcessing(); }
    STDMETHOD_(void, SetNPatchMode)(float Segments) { m_pDevice9->SetNPatchMode(Segments); }
    STDMETHOD_(float, GetNPatchMode)(void) { return m_pDevice9->GetNPatchMode(); }
    STDMETHOD_(void, DrawPrimitive)(DWORD PrimitiveType, DWORD StartVertex, DWORD PrimitiveCount) { m_pDevice9->DrawPrimitive((D3DPRIMITIVETYPE)PrimitiveType, StartVertex, PrimitiveCount); }
    STDMETHOD_(void, DrawIndexedPrimitive)(DWORD PrimitiveType, DWORD MinIndex, DWORD NumVertices, DWORD StartIndex, DWORD PrimitiveCount) { m_pDevice9->DrawIndexedPrimitive((D3DPRIMITIVETYPE)PrimitiveType, MinIndex, NumVertices, StartIndex, PrimitiveCount); }
    STDMETHOD_(void, DrawPrimitiveUP)(DWORD PrimitiveType, DWORD PrimitiveCount, void* pVertexStreamZeroData, DWORD VertexStreamZeroStride) { m_pDevice9->DrawPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
    STDMETHOD_(void, DrawIndexedPrimitiveUP)(DWORD PrimitiveType, DWORD MinVertexIndex, DWORD NumVertexIndices, DWORD PrimitiveCount, void* pIndexData, DWORD IndexDataFormat, void* pVertexStreamZeroData, DWORD VertexStreamZeroStride) { m_pDevice9->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, (D3DFORMAT)IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
    STDMETHOD_(void, SetVertexShader)(DWORD Handle) { m_pDevice9->SetVertexShader(Handle); }
    STDMETHOD_(DWORD, GetVertexShader)(void) { return m_pDevice9->GetVertexShader(); }
    STDMETHOD_(void, SetVertexShaderConstant)(DWORD Register, void* pConstantData, DWORD ConstantCount) { m_pDevice9->SetVertexShaderConstant(Register, pConstantData, ConstantCount); }
    STDMETHOD_(void, GetVertexShaderConstant)(DWORD Register, void* pConstantData, DWORD ConstantCount) { m_pDevice9->GetVertexShaderConstant(Register, pConstantData, ConstantCount); }
    STDMETHOD_(void, DeleteVertexShader)(DWORD Handle) { m_pDevice9->DeleteVertexShader(Handle); }
    STDMETHOD_(void, SetPixelShader)(DWORD Handle) { m_pDevice9->SetPixelShader(Handle); }
    STDMETHOD_(DWORD, GetPixelShader)(void) { return m_pDevice9->GetPixelShader(); }
    STDMETHOD_(void, SetPixelShaderConstant)(DWORD Register, void* pConstantData, DWORD ConstantCount) { m_pDevice9->SetPixelShaderConstant(Register, pConstantData, ConstantCount); }
    STDMETHOD_(void, GetPixelShaderConstant)(DWORD Register, void* pConstantData, DWORD ConstantCount) { m_pDevice9->GetPixelShaderConstant(Register, pConstantData, ConstantCount); }
    STDMETHOD_(void, DeletePixelShader)(DWORD Handle) { m_pDevice9->DeletePixelShader(Handle); }
    STDMETHOD_(void, GetGammaRamp)(void* pRamp) { m_pDevice9->GetGammaRamp((D3DGAMMARAMP*)pRamp); }
    STDMETHOD_(void, SetGammaRamp)(DWORD Flags, void* pRamp) { m_pDevice9->SetGammaRamp(Flags, (D3DGAMMARAMP*)pRamp); }
    STDMETHOD_(void, UpdateTexture)(void* pSourceTexture, void* pDestinationTexture) { m_pDevice9->UpdateTexture((IDirect3DBaseTexture9*)pSourceTexture, (IDirect3DBaseTexture9*)pDestinationTexture); }
    STDMETHOD_(void, GetFrontBuffer)(void* pDestSurface) { m_pDevice9->GetFrontBufferData(0, (IDirect3DSurface9*)pDestSurface); }
    STDMETHOD_(void, GetBackBuffer)(DWORD BackBuffer, DWORD Type, void** ppBackBuffer)
    {
        IDirect3DSurface9* pBB9 = nullptr;
        D3DBACKBUFFER_TYPE type9 = (Type == 0) ? D3DBACKBUFFER_TYPE_MONO : D3DBACKBUFFER_TYPE_UNKNOWN;
        m_pDevice9->GetBackBuffer(0, BackBuffer, type9, &pBB9);
        if (pBB9)
        {
            *ppBackBuffer = new CDirect3DSurface8(pBB9);
            pBB9->Release();
        }
    }
};

interface IDirect3D8 : public IUnknown
{
public:
    virtual void STDMETHODCALLTYPE RegisterSoftwareDevice(void* pInitializeFunction) = 0;
    virtual UINT STDMETHODCALLTYPE GetAdapterCount(void) = 0;
    virtual void STDMETHODCALLTYPE GetAdapterIdentifier(UINT Adapter, DWORD Flags, void* pIdentifier) = 0;
    virtual UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT Adapter) = 0;
    virtual void STDMETHODCALLTYPE EnumAdapterModes(UINT Adapter, UINT Mode, void* pMode) = 0;
    virtual void STDMETHODCALLTYPE GetAdapterDisplayMode(UINT Adapter, void* pMode) = 0;
    virtual void STDMETHODCALLTYPE CheckDeviceType(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) = 0;
    virtual void STDMETHODCALLTYPE CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, DWORD RType, D3DFORMAT CheckFormat) = 0;
    virtual void STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, DWORD MultiSampleType) = 0;
    virtual void STDMETHODCALLTYPE CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) = 0;
    virtual void STDMETHODCALLTYPE GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, void* pCaps) = 0;
    virtual void* STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter) = 0;
    virtual void STDMETHODCALLTYPE CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, void* pPresentationParameters, void** ppReturnedDeviceInterface) = 0;
};

class CDirect3D8 : public IDirect3D8
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

    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
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

    STDMETHOD_(ULONG, AddRef)(void) { return InterlockedIncrement(&g_RefCount); }
    STDMETHOD_(ULONG, Release)(void)
    {
        ULONG RefCount = InterlockedDecrement(&g_RefCount);
        if (RefCount == 0) delete this;
        return RefCount;
    }

    STDMETHOD_(void, RegisterSoftwareDevice)(void* pInitializeFunction) { m_pD3D9->RegisterSoftwareDevice(pInitializeFunction); }
    STDMETHOD_(UINT, GetAdapterCount)(void) { return m_pD3D9->GetAdapterCount(); }
    STDMETHOD_(void, GetAdapterIdentifier)(UINT Adapter, DWORD Flags, void* pIdentifier) { m_pD3D9->GetAdapterIdentifier(Adapter, Flags, (D3DADAPTER_IDENTIFIER9*)pIdentifier); }
    STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter) { return m_pD3D9->GetAdapterModeCount(Adapter, D3DFMT_UNKNOWN); }
    STDMETHOD_(void, EnumAdapterModes)(UINT Adapter, UINT Mode, void* pMode) { m_pD3D9->EnumAdapterModes(Adapter, D3DFMT_UNKNOWN, Mode, (D3DDISPLAYMODE*)pMode); }
    STDMETHOD_(void, GetAdapterDisplayMode)(UINT Adapter, void* pMode) { m_pD3D9->GetAdapterDisplayMode(Adapter, (D3DDISPLAYMODE*)pMode); }
    STDMETHOD_(void, CheckDeviceType)(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) { m_pD3D9->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed); }
    STDMETHOD_(void, CheckDeviceFormat)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, DWORD RType, D3DFORMAT CheckFormat) { m_pD3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, (D3DRESOURCETYPE)RType, CheckFormat); }
    STDMETHOD_(void, CheckDeviceMultiSampleType)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, DWORD MultiSampleType) { m_pD3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, (D3DMULTISAMPLE_TYPE)MultiSampleType, nullptr); }
    STDMETHOD_(void, CheckDepthStencilMatch)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) { m_pD3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }
    STDMETHOD_(void, GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, void* pCaps) { m_pD3D9->GetDeviceCaps(Adapter, DeviceType, (D3DCAPS9*)pCaps); }
    STDMETHOD_(void*, GetAdapterMonitor)(UINT Adapter) { return (void*)m_pD3D9->GetAdapterMonitor(Adapter); }

    STDMETHOD_(void, CreateDevice)(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, void* pPresentationParameters, void** ppReturnedDeviceInterface)
    {
        if (ppReturnedDeviceInterface == nullptr) return;

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
        m_pD3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, &pp9, &pDevice9);
        if (pDevice9)
        {
            *ppReturnedDeviceInterface = new CDirect3DDevice8(pDevice9, this);
            pDevice9->Release();
        }
    }
};

extern "C" __declspec(dllexport) void* __cdecl CreateD3D8Wrapper(void* pD3D9)
{
    if (pD3D9 == nullptr) return nullptr;
    return new CDirect3D8((IDirect3D9*)pD3D9);
}
