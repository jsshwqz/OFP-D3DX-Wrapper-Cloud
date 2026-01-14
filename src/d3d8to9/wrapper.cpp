#include "config.hpp"
#include "d3d8to9-wrapper.hpp"
#include <stdio.h>
#include <string.h>

namespace D3D8To9
{
    static IDirect3D9* g_pD3D9 = nullptr;
    static UINT g_SDKVersion = 0;
    static LONG g_RefCount = 0;

    class CDirect3D8 : public IDirect3D8
    {
    private:
        IDirect3D9* m_pD3D9;
        UINT m_SDKVersion;

    public:
        CDirect3D8(IDirect3D9* pD3D9, UINT SDKVersion) : m_pD3D9(pD3D9), m_SDKVersion(SDKVersion)
        {
            if (m_pD3D9)
                m_pD3D9->AddRef();
        }

        virtual ~CDirect3D8() {}

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
            {
                delete this;
            }
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
                pCaps->VolumeTextureCaps = caps9.VolumeTextureCaps;
                pCaps->CubeTextureCaps = caps9.CubeTextureCaps;
                pCaps->TextureAddressCaps = caps9.TextureAddressCaps;
                pCaps->VolumeTextureAddressCaps = caps9.VolumeTextureAddressCaps;
                pCaps->LineCaps = caps9.LineCaps;
                pCaps->MaxTextureWidth = caps9.MaxTextureWidth;
                pCaps->MaxTextureHeight = caps9.MaxTextureHeight;
                pCaps->MaxVolumeExtent = caps9.MaxVolumeExtent;
                pCaps->MaxTextureRepeat = caps9.MaxTextureRepeat;
                pCaps->MaxTextureAspectRatio = caps9.MaxTextureAspectRatio;
                pCaps->MaxAnisotropy = caps9.MaxAnisotropy;
                pCaps->MaxVertexW = caps9.MaxVertexW;
                pCaps->GuardBandLeft = caps9.GuardBandLeft;
                pCaps->GuardBandRight = caps9.GuardBandRight;
                pCaps->GuardBandTop = caps9.GuardBandTop;
                pCaps->GuardBandBottom = caps9.GuardBandBottom;
                pCaps->ExtentsAdjust = caps9.ExtentsAdjust;
                pCaps->StencilCaps = caps9.StencilCaps;
                pCaps->FVFCaps = caps9.FVFCaps;
                pCaps->TextureOpCaps = caps9.TextureOpCaps;
                pCaps->MaxTextureBlendStages = caps9.MaxTextureBlendStages;
                pCaps->MaxSimultaneousTextures = caps9.MaxSimultaneousTextures;
                pCaps->VertexProcessingCaps = caps9.VertexProcessingCaps;
                pCaps->MaxActiveLights = caps9.MaxActiveLights;
                pCaps->MaxUserClipPlanes = caps9.MaxUserClipPlanes;
                pCaps->MaxVertexBlendMatrices = caps9.MaxVertexBlendMatrices;
                pCaps->MaxVertexBlendMatrixIndex = caps9.MaxVertexBlendMatrixIndex;
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

        STDMETHOD(CreateDevice)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface)
        {
            if (ppReturnedDeviceInterface == nullptr)
                return D3DERR_INVALIDCALL;

            D3DPRESENT_PARAMETERS pp9;
            memset(&pp9, 0, sizeof(pp9));
            pp9.BackBufferWidth = pPresentationParameters->BackBufferWidth;
            pp9.BackBufferHeight = pPresentationParameters->BackBufferHeight;
            pp9.BackBufferFormat = pPresentationParameters->BackBufferFormat;
            pp9.BackBufferCount = pPresentationParameters->BackBufferCount;
            pp9.MultiSampleType = pPresentationParameters->MultiSampleType;
            pp9.SwapEffect = pPresentationParameters->SwapEffect;
            pp9.hDeviceWindow = pPresentationParameters->hDeviceWindow;
            pp9.Windowed = pPresentationParameters->Windowed;
            pp9.EnableAutoDepthStencil = pPresentationParameters->EnableAutoDepthStencil;
            pp9.AutoDepthStencilFormat = pPresentationParameters->AutoDepthStencilFormat;
            pp9.Flags = pPresentationParameters->Flags;
            pp9.FullScreen_RefreshRateInHz = 0;
            pp9.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

            if (Config::g_Config.EnableVSync && Config::g_Config.ForceVSyncMode)
            {
                pp9.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
            }
            else if (!Config::g_Config.EnableVSync)
            {
                pp9.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
            }

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
            {
                delete this;
            }
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
                pCaps->Caps4 = caps9.Caps4;
                pCaps->PresentationIntervals = caps9.PresentationIntervals;
                pCaps->CursorCaps = caps9.CursorCaps;
                pCaps->TextureCaps = caps9.TextureCaps;
                pCaps->TextureFilterCaps = caps9.TextureFilterCaps;
                pCaps->VolumeTextureCaps = caps9.VolumeTextureCaps;
                pCaps->CubeTextureCaps = caps9.CubeTextureCaps;
                pCaps->TextureAddressCaps = caps9.TextureAddressCaps;
                pCaps->VolumeTextureAddressCaps = caps9.VolumeTextureAddressCaps;
                pCaps->LineCaps = caps9.LineCaps;
                pCaps->MaxTextureWidth = caps9.MaxTextureWidth;
                pCaps->MaxTextureHeight = caps9.MaxTextureHeight;
                pCaps->MaxVolumeExtent = caps9.MaxVolumeExtent;
                pCaps->MaxTextureRepeat = caps9.MaxTextureRepeat;
                pCaps->MaxTextureAspectRatio = caps9.MaxTextureAspectRatio;
                pCaps->MaxAnisotropy = caps9.MaxAnisotropy;
                pCaps->MaxVertexW = caps9.MaxVertexW;
                pCaps->GuardBandLeft = caps9.GuardBandLeft;
                pCaps->GuardBandRight = caps9.GuardBandRight;
                pCaps->GuardBandTop = caps9.GuardBandTop;
                pCaps->GuardBandBottom = caps9.GuardBandBottom;
                pCaps->ExtentsAdjust = caps9.ExtentsAdjust;
                pCaps->StencilCaps = caps9.StencilCaps;
                pCaps->FVFCaps = caps9.FVFCaps;
                pCaps->TextureOpCaps = caps9.TextureOpCaps;
                pCaps->MaxTextureBlendStages = caps9.MaxTextureBlendStages;
                pCaps->MaxSimultaneousTextures = caps9.MaxSimultaneousTextures;
                pCaps->VertexProcessingCaps = caps9.VertexProcessingCaps;
                pCaps->MaxActiveLights = caps9.MaxActiveLights;
                pCaps->MaxUserClipPlanes = caps9.MaxUserClipPlanes;
                pCaps->MaxVertexBlendMatrices = caps9.MaxVertexBlendMatrices;
                pCaps->MaxVertexBlendMatrixIndex = caps9.MaxVertexBlendMatrixIndex;
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

        STDMETHOD(GetDisplayMode)(THIS_ D3DDISPLAYMODE* pMode)
        {
            return m_pDevice9->GetDisplayMode(pMode);
        }

        STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS* pParameters)
        {
            return m_pDevice9->GetCreationParameters(pParameters);
        }

        STDMETHOD(CreateTexture)(THIS_ UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture)
        {
            if (ppTexture == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DTexture9* pTex9 = nullptr;
            DWORD d3d9Usage = Usage;
            if (Config::g_Config.AnisotropicFiltering > 1)
            {
                if (d3d9Usage & D3DUSAGE_AUTOGENMIPMAP)
                    d3d9Usage |= D3DUSAGE_DYNAMIC;
            }

            D3DPOOL d3d9Pool = Pool;
            if (Pool == D3DPOOL_DEFAULT && Usage == 0)
            {
                d3d9Pool = D3DPOOL_MANAGED;
            }

            DWORD levels = Levels;
            if (Levels == 0)
            {
                levels = 1;
                UINT w = Width, h = Height;
                while (w > 1 && h > 1)
                {
                    w >>= 1;
                    h >>= 1;
                    levels++;
                }
            }

            HRESULT hr = m_pDevice9->CreateTexture(Width, Height, levels, d3d9Usage, Format, d3d9Pool, &pTex9, nullptr);
            if (SUCCEEDED(hr) && pTex9)
            {
                *ppTexture = new CDirect3DTexture8(pTex9, Width, Height, levels, Format);
                pTex9->Release();
            }
            return hr;
        }

        STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppVolumeTexture)
        {
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppCubeTexture)
        {
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(CreateSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface)
        {
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(CreateRenderTarget)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppRenderTarget)
        {
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppDepthStencil)
        {
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(CreateImageSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface)
        {
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer)
        {
            if (ppVertexBuffer == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DVertexBuffer9* pVB9 = nullptr;
            DWORD d3d9Usage = Usage & ~(D3DUSAGE_WRITEONLY);
            if (d3d9Usage == D3DUSAGE_WRITEONLY)
                d3d9Usage = 0;

            D3DPOOL d3d9Pool = Pool;
            if (Pool == D3DPOOL_DEFAULT && Usage == 0)
            {
                d3d9Pool = D3DPOOL_MANAGED;
            }

            HRESULT hr = m_pDevice9->CreateVertexBuffer(Length, d3d9Usage, FVF, d3d9Pool, &pVB9, nullptr);
            if (SUCCEEDED(hr) && pVB9)
            {
                *ppVertexBuffer = new CDirect3DVertexBuffer8(pVB9, Length, FVF);
                pVB9->Release();
            }
            return hr;
        }

        STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer)
        {
            if (ppIndexBuffer == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DIndexBuffer9* pIB9 = nullptr;
            DWORD d3d9Usage = Usage & ~(D3DUSAGE_WRITEONLY);
            if (d3d9Usage == D3DUSAGE_WRITEONLY)
                d3d9Usage = 0;

            D3DPOOL d3d9Pool = Pool;
            if (Pool == D3DPOOL_DEFAULT && Usage == 0)
            {
                d3d9Pool = D3DPOOL_MANAGED;
            }

            D3DFORMAT d3d9Format = (Format == D3DFMT_INDEX16) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;

            HRESULT hr = m_pDevice9->CreateIndexBuffer(Length, d3d9Usage, d3d9Format, d3d9Pool, &pIB9, nullptr);
            if (SUCCEEDED(hr) && pIB9)
            {
                *ppIndexBuffer = new CDirect3DIndexBuffer8(pIB9, Length, Format);
                pIB9->Release();
            }
            return hr;
        }

        STDMETHOD(SetRenderTarget)(THIS_ IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil)
        {
            IDirect3DSurface9* pRT9 = nullptr;
            IDirect3DSurface9* pZS9 = nullptr;

            if (pRenderTarget)
            {
                CDirect3DSurface8* pRT = static_cast<CDirect3DSurface8*>(pRenderTarget);
                pRT9 = pRT->GetD3D9Surface();
                pRT9->AddRef();
            }

            if (pNewZStencil)
            {
                CDirect3DSurface8* pZS = static_cast<CDirect3DSurface8*>(pNewZStencil);
                pZS9 = pZS->GetD3D9Surface();
                pZS9->AddRef();
            }

            HRESULT hr = m_pDevice9->SetRenderTarget(pRT9, pZS9);

            if (pRT9) pRT9->Release();
            if (pZS9) pZS9->Release();

            return hr;
        }

        STDMETHOD(GetRenderTarget)(THIS_ IDirect3DSurface8** ppRenderTarget)
        {
            if (ppRenderTarget == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DSurface9* pRT9 = nullptr;
            HRESULT hr = m_pDevice9->GetRenderTarget(&pRT9);
            if (SUCCEEDED(hr) && pRT9)
            {
                *ppRenderTarget = new CDirect3DSurface8(pRT9);
                pRT9->Release();
            }
            return hr;
        }

        STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface8** ppZStencilSurface)
        {
            if (ppZStencilSurface == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DSurface9* pZS9 = nullptr;
            HRESULT hr = m_pDevice9->GetDepthStencilSurface(&pZS9);
            if (SUCCEEDED(hr) && pZS9)
            {
                *ppZStencilSurface = new CDirect3DSurface8(pZS9);
                pZS9->Release();
            }
            return hr;
        }

        STDMETHOD(BeginScene)(THIS)
        {
            return m_pDevice9->BeginScene();
        }

        STDMETHOD(EndScene)(THIS)
        {
            return m_pDevice9->EndScene();
        }

        STDMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
        {
            return m_pDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil);
        }

        STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT8* pViewport)
        {
            D3DVIEWPORT9 vp;
            vp.X = pViewport->X;
            vp.Y = pViewport->Y;
            vp.Width = pViewport->Width;
            vp.Height = pViewport->Height;
            vp.MinZ = pViewport->MinZ;
            vp.MaxZ = pViewport->MaxZ;
            return m_pDevice9->SetViewport(&vp);
        }

        STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT8* pViewport)
        {
            D3DVIEWPORT9 vp;
            HRESULT hr = m_pDevice9->GetViewport(&vp);
            if (SUCCEEDED(hr) && pViewport)
            {
                pViewport->X = vp.X;
                pViewport->Y = vp.Y;
                pViewport->Width = vp.Width;
                pViewport->Height = vp.Height;
                pViewport->MinZ = vp.MinZ;
                pViewport->MaxZ = vp.MaxZ;
            }
            return hr;
        }

        STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL8* pMaterial)
        {
            D3DMATERIAL9 mat9;
            mat9.Diffuse.r = pMaterial->diffuse.r;
            mat9.Diffuse.g = pMaterial->diffuse.g;
            mat9.Diffuse.b = pMaterial->diffuse.b;
            mat9.Diffuse.a = pMaterial->diffuse.a;
            mat9.Ambient.r = pMaterial->ambient.r;
            mat9.Ambient.g = pMaterial->ambient.g;
            mat9.Ambient.b = pMaterial->ambient.b;
            mat9.Ambient.a = pMaterial->ambient.a;
            mat9.Specular.r = pMaterial->specular.r;
            mat9.Specular.g = pMaterial->specular.g;
            mat9.Specular.b = pMaterial->specular.b;
            mat9.Specular.a = pMaterial->specular.a;
            mat9.Emissive.r = pMaterial->emissive.r;
            mat9.Emissive.g = pMaterial->emissive.g;
            mat9.Emissive.b = pMaterial->emissive.b;
            mat9.Emissive.a = pMaterial->emissive.a;
            mat9.Power = pMaterial->Power;
            return m_pDevice9->SetMaterial(&mat9);
        }

        STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL8* pMaterial)
        {
            D3DMATERIAL9 mat9;
            HRESULT hr = m_pDevice9->GetMaterial(&mat9);
            if (SUCCEEDED(hr) && pMaterial)
            {
                pMaterial->diffuse.r = mat9.Diffuse.r;
                pMaterial->diffuse.g = mat9.Diffuse.g;
                pMaterial->diffuse.b = mat9.Diffuse.b;
                pMaterial->diffuse.a = mat9.Diffuse.a;
                pMaterial->ambient.r = mat9.Ambient.r;
                pMaterial->ambient.g = mat9.Ambient.g;
                pMaterial->ambient.b = mat9.Ambient.b;
                pMaterial->ambient.a = mat9.Ambient.a;
                pMaterial->specular.r = mat9.Specular.r;
                pMaterial->specular.g = mat9.Specular.g;
                pMaterial->specular.b = mat9.Specular.b;
                pMaterial->specular.a = mat9.Specular.a;
                pMaterial->emissive.r = mat9.Emissive.r;
                pMaterial->emissive.g = mat9.Emissive.g;
                pMaterial->emissive.b = mat9.Emissive.b;
                pMaterial->emissive.a = mat9.Emissive.a;
                pMaterial->Power = mat9.Power;
            }
            return hr;
        }

        STDMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT8* pLight)
        {
            D3DLIGHT9 light9;
            light9.Type = (D3DLIGHTTYPE)pLight->Type;
            light9.Diffuse.r = pLight->diffuse.r;
            light9.Diffuse.g = pLight->diffuse.g;
            light9.Diffuse.b = pLight->diffuse.b;
            light9.Diffuse.a = pLight->diffuse.a;
            light9.Specular.r = pLight->specular.r;
            light9.Specular.g = pLight->specular.g;
            light9.Specular.b = pLight->specular.b;
            light9.Specular.a = pLight->specular.a;
            light9.Ambient.r = pLight->ambient.r;
            light9.Ambient.g = pLight->ambient.g;
            light9.Ambient.b = pLight->ambient.b;
            light9.Ambient.a = pLight->ambient.a;
            light9.Position.x = pLight->Position.x;
            light9.Position.y = pLight->Position.y;
            light9.Position.z = pLight->Position.z;
            light9.Direction.x = pLight->Direction.x;
            light9.Direction.y = pLight->Direction.y;
            light9.Direction.z = pLight->Direction.z;
            light9.Range = pLight->Range;
            light9.Falloff = pLight->Falloff;
            light9.Attenuation0 = pLight->Attenuation0;
            light9.Attenuation1 = pLight->Attenuation1;
            light9.Attenuation2 = pLight->Attenuation2;
            light9.Theta = pLight->Theta;
            light9.Phi = pLight->Phi;
            return m_pDevice9->SetLight(Index, &light9);
        }

        STDMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable)
        {
            return m_pDevice9->LightEnable(Index, Enable);
        }

        STDMETHOD(GetLightEnable)(THIS_ DWORD Index, BOOL* pEnable)
        {
            return m_pDevice9->LightEnable(Index, pEnable);
        }

        STDMETHOD(SetClipPlane)(THIS_ DWORD Index, CONST float* pPlane)
        {
            return m_pDevice9->SetClipPlane(Index, pPlane);
        }

        STDMETHOD(GetClipPlane)(THIS_ DWORD Index, float* pPlane)
        {
            return m_pDevice9->GetClipPlane(Index, pPlane);
        }

        STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD Value)
        {
            return m_pDevice9->SetRenderState((D3DRENDERSTATETYPE)State, Value);
        }

        STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD* pValue)
        {
            return m_pDevice9->GetRenderState((D3DRENDERSTATETYPE)State, pValue);
        }

        STDMETHOD(SetTexture)(THIS_ DWORD Stage, IDirect3DTexture8* pTexture)
        {
            if (pTexture == nullptr)
            {
                return m_pDevice9->SetTexture(Stage, nullptr);
            }

            CDirect3DTexture8* pTex8 = static_cast<CDirect3DTexture8*>(pTexture);
            IDirect3DTexture9* pTex9 = pTex8->GetD3D9Texture();
            if (pTex9)
            {
                pTex9->AddRef();
                m_pDevice9->SetTexture(Stage, pTex9);
                pTex9->Release();
                return D3D_OK;
            }
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(GetTexture)(THIS_ DWORD Stage, IDirect3DTexture8** ppTexture)
        {
            if (ppTexture == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DBaseTexture9* pTex9 = nullptr;
            HRESULT hr = m_pDevice9->GetTexture(Stage, &pTex9);
            if (SUCCEEDED(hr) && pTex9 && pTex9->GetType() == D3DRTYPE_TEXTURE)
            {
                *ppTexture = new CDirect3DTexture8(static_cast<IDirect3DTexture9*>(pTex9), 0, 0, 1, D3DFMT_UNKNOWN);
                pTex9->Release();
            }
            else
            {
                *ppTexture = nullptr;
            }
            if (pTex9) pTex9->Release();
            return D3D_OK;
        }

        STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
        {
            return m_pDevice9->GetTextureStageState(Stage, (D3DTEXTURESTAGESTATETYPE)Type, pValue);
        }

        STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
        {
            return m_pDevice9->SetTextureStageState(Stage, (D3DTEXTURESTAGESTATETYPE)Type, Value);
        }

        STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect)
        {
            return m_pDevice9->SetScissorRect(pRect);
        }

        STDMETHOD(GetScissorRect)(THIS_ RECT* pRect)
        {
            return m_pDevice9->GetScissorRect(pRect);
        }

        STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL Software)
        {
            return m_pDevice9->SetSoftwareVertexProcessing(Software);
        }

        STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS)
        {
            return m_pDevice9->GetSoftwareVertexProcessing();
        }

        STDMETHOD(SetNPatchMode)(THIS_ float Segments)
        {
            return m_pDevice9->SetNPatchMode(Segments);
        }

        STDMETHOD_(float, GetNPatchMode)(THIS)
        {
            return m_pDevice9->GetNPatchMode();
        }

        STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
        {
            return m_pDevice9->DrawPrimitive((D3DPRIMITIVETYPE)PrimitiveType, StartVertex, PrimitiveCount);
        }

        STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
        {
            return m_pDevice9->DrawIndexedPrimitive((D3DPRIMITIVETYPE)PrimitiveType, MinIndex, NumVertices, StartIndex, PrimitiveCount);
        }

        STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
        {
            return m_pDevice9->DrawPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
        }

        STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
        {
            return m_pDevice9->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
        }

        STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags)
        {
            if (pDestBuffer == nullptr)
                return D3DERR_INVALIDCALL;

            CDirect3DVertexBuffer8* pVB8 = static_cast<CDirect3DVertexBuffer8*>(pDestBuffer);
            IDirect3DVertexBuffer9* pVB9 = pVB8->GetD3D9Buffer();
            if (pVB9)
            {
                pVB9->AddRef();
                HRESULT hr = m_pDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pVB9, nullptr, Flags);
                pVB9->Release();
                return hr;
            }
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(SetVertexShader)(THIS_ DWORD Handle)
        {
            return m_pDevice9->SetVertexShader(Handle);
        }

        STDMETHOD_(DWORD, GetVertexShader)(THIS)
        {
            return m_pDevice9->GetVertexShader();
        }

        STDMETHOD(SetVertexShaderConstant)(THIS_ DWORD Register, CONST void* pConstantData, DWORD ConstantCount)
        {
            return m_pDevice9->SetVertexShaderConstant(Register, pConstantData, ConstantCount);
        }

        STDMETHOD(GetVertexShaderConstant)(THIS_ DWORD Register, void* pConstantData, DWORD ConstantCount)
        {
            return m_pDevice9->GetVertexShaderConstant(Register, pConstantData, ConstantCount);
        }

        STDMETHOD(DeleteVertexShader)(THIS_ DWORD Handle)
        {
            return m_pDevice9->DeleteVertexShader(Handle);
        }

        STDMETHOD(SetPixelShader)(THIS_ DWORD Handle)
        {
            return m_pDevice9->SetPixelShader(Handle);
        }

        STDMETHOD_(DWORD, GetPixelShader)(THIS)
        {
            return m_pDevice9->GetPixelShader();
        }

        STDMETHOD(SetPixelShaderConstant)(THIS_ DWORD Register, CONST void* pConstantData, DWORD ConstantCount)
        {
            return m_pDevice9->SetPixelShaderConstant(Register, pConstantData, ConstantCount);
        }

        STDMETHOD(GetPixelShaderConstant)(THIS_ DWORD Register, void* pConstantData, DWORD ConstantCount)
        {
            return m_pDevice9->GetPixelShaderConstant(Register, pConstantData, ConstantCount);
        }

        STDMETHOD(DeletePixelShader)(THIS_ DWORD Handle)
        {
            return m_pDevice9->DeletePixelShader(Handle);
        }

        STDMETHOD(GetGammaRamp)(THIS_ D3DGAMMARAMP* pRamp)
        {
            return m_pDevice9->GetGammaRamp(pRamp);
        }

        STDMETHOD(SetGammaRamp)(THIS_ DWORD Flags, CONST D3DGAMMARAMP* pRamp)
        {
            return m_pDevice9->SetGammaRamp(Flags, pRamp);
        }

        STDMETHOD(UpdateTexture)(THIS_ IDirect3DTexture8* pSourceTexture, IDirect3DTexture8* pDestinationTexture)
        {
            if (pSourceTexture == nullptr || pDestinationTexture == nullptr)
                return D3DERR_INVALIDCALL;

            CDirect3DTexture8* pSrc8 = static_cast<CDirect3DTexture8*>(pSourceTexture);
            CDirect3DTexture8* pDst8 = static_cast<CDirect3DTexture8*>(pDestinationTexture);

            IDirect3DTexture9* pSrc9 = pSrc8->GetD3D9Texture();
            IDirect3DTexture9* pDst9 = pDst8->GetD3D9Texture();

            if (pSrc9 && pDst9)
            {
                pSrc9->AddRef();
                pDst9->AddRef();
                HRESULT hr = m_pDevice9->UpdateTexture(pSrc9, pDst9);
                pSrc9->Release();
                pDst9->Release();
                return hr;
            }
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(GetFrontBuffer)(THIS_ IDirect3DSurface8* pDestSurface)
        {
            if (pDestSurface == nullptr)
                return D3DERR_INVALIDCALL;

            CDirect3DSurface8* pSurf8 = static_cast<CDirect3DSurface8*>(pDestSurface);
            IDirect3DSurface9* pSurf9 = pSurf8->GetD3D9Surface();

            if (pSurf9)
            {
                pSurf9->AddRef();
                IDirect3DSurface9* pFront9 = nullptr;
                m_pDevice9->GetFrontBufferData(0, &pFront9);
                m_pDevice9->StretchRect(pFront9, nullptr, pSurf9, nullptr, D3DTEXF_NONE);
                pFront9->Release();
                pSurf9->Release();
                return D3D_OK;
            }
            return D3DERR_INVALIDCALL;
        }

        STDMETHOD(GetBackBuffer)(THIS_ UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer)
        {
            if (ppBackBuffer == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DSurface9* pBB9 = nullptr;
            D3DBACKBUFFER_TYPE type9 = (Type == D3DBACKBUFFER_TYPE_MONO) ? D3DBACKBUFFER_TYPE_MONO : D3DBACKBUFFER_TYPE_UNKNOWN;
            HRESULT hr = m_pDevice9->GetBackBuffer(0, BackBuffer, type9, &pBB9);
            if (SUCCEEDED(hr) && pBB9)
            {
                *ppBackBuffer = new CDirect3DSurface8(pBB9);
                pBB9->Release();
            }
            return hr;
        }
    };

    class CDirect3DTexture8 : public IDirect3DTexture8
    {
    private:
        IDirect3DTexture9* m_pTexture9;
        UINT m_Width;
        UINT m_Height;
        UINT m_Levels;
        D3DFORMAT m_Format;
        LONG m_RefCount;

    public:
        CDirect3DTexture8(IDirect3DTexture9* pTexture9, UINT Width, UINT Height, UINT Levels, D3DFORMAT Format)
            : m_pTexture9(pTexture9), m_Width(Width), m_Height(Height), m_Levels(Levels), m_Format(Format), m_RefCount(1)
        {
            if (m_pTexture9)
            {
                m_pTexture9->AddRef();
                if (Width == 0)
                {
                    D3DSURFACE_DESC desc;
                    if (SUCCEEDED(m_pTexture9->GetLevelDesc(0, &desc)))
                    {
                        m_Width = desc.Width;
                        m_Height = desc.Height;
                        m_Format = desc.Format;
                    }
                }
            }
        }

        virtual ~CDirect3DTexture8()
        {
            if (m_pTexture9)
                m_pTexture9->Release();
        }

        IDirect3DTexture9* GetD3D9Texture() const { return m_pTexture9; }

        STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
        {
            if (ppvObj == nullptr)
                return E_INVALIDARG;

            if (riid == IID_IUnknown || riid == __uuidof(IDirect3DTexture8))
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

        STDMETHOD(GetDevice)(THIS_ IDirect3DDevice8** ppDevice)
        {
            if (ppDevice == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DDevice9* pDevice9 = nullptr;
            HRESULT hr = m_pTexture9->GetDevice(&pDevice9);
            if (SUCCEEDED(hr) && pDevice9)
            {
                return E_NOTIMPL;
            }
            return hr;
        }

        STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
        {
            return m_pTexture9->SetPrivateData(refguid, pData, SizeOfData, Flags);
        }

        STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
        {
            return m_pTexture9->GetPrivateData(refguid, pData, pSizeOfData);
        }

        STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid)
        {
            return m_pTexture9->FreePrivateData(refguid);
        }

        STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD Priority)
        {
            return m_pTexture9->SetPriority(Priority);
        }

        STDMETHOD_(DWORD, GetPriority)(THIS)
        {
            return m_pTexture9->GetPriority();
        }

        STDMETHOD_(void, PreLoad)(THIS)
        {
            m_pTexture9->PreLoad();
        }

        STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS)
        {
            return D3DRTYPE_TEXTURE;
        }

        STDMETHOD_(DWORD, GetLevelCount)(THIS)
        {
            return m_pTexture9->GetLevelCount();
        }

        STDMETHOD_(DWORD, GetLevelDesc)(THIS_ UINT Level, D3DSURFACE_DESC* pDesc)
        {
            D3DSURFACE_DESC desc9;
            HRESULT hr = m_pTexture9->GetLevelDesc(Level, &desc9);
            if (SUCCEEDED(hr) && pDesc)
            {
                pDesc->Format = desc9.Format;
                pDesc->Type = desc9.Type;
                pDesc->Usage = desc9.Usage;
                pDesc->Pool = desc9.Pool;
                pDesc->Width = desc9.Width;
                pDesc->Height = desc9.Height;
                pDesc->MultiSampleType = (D3DMULTISAMPLE_TYPE)desc9.MultiSampleType;
            }
            return hr;
        }

        STDMETHOD(GetSurfaceLevel)(THIS_ UINT Level, IDirect3DSurface8** ppSurfaceLevel)
        {
            if (ppSurfaceLevel == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DSurface9* pSurf9 = nullptr;
            HRESULT hr = m_pTexture9->GetSurfaceLevel(Level, &pSurf9);
            if (SUCCEEDED(hr) && pSurf9)
            {
                *ppSurfaceLevel = new CDirect3DSurface8(pSurf9);
                pSurf9->Release();
            }
            return hr;
        }

        STDMETHOD(LockRect)(THIS_ UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
        {
            return m_pTexture9->LockRect(Level, pLockedRect, pRect, Flags);
        }

        STDMETHOD(UnlockRect)(THIS_ UINT Level)
        {
            return m_pTexture9->UnlockRect(Level);
        }

        STDMETHOD(D3D8LoadTexture)(THIS_ IDirect3DTexture8* pSourceTexture, DWORD Filter)
        {
            return D3DERR_INVALIDCALL;
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
            if (m_pSurface9)
                m_pSurface9->AddRef();
        }

        virtual ~CDirect3DSurface8()
        {
            if (m_pSurface9)
                m_pSurface9->Release();
        }

        IDirect3DSurface9* GetD3D9Surface() const { return m_pSurface9; }

        STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
        {
            if (ppvObj == nullptr)
                return E_INVALIDARG;

            if (riid == IID_IUnknown || riid == __uuidof(IDirect3DSurface8))
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

        STDMETHOD(GetDevice)(THIS_ IDirect3DDevice8** ppDevice)
        {
            if (ppDevice == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DDevice9* pDevice9 = nullptr;
            return m_pSurface9->GetDevice(&pDevice9);
        }

        STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
        {
            return m_pSurface9->SetPrivateData(refguid, pData, SizeOfData, Flags);
        }

        STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
        {
            return m_pSurface9->GetPrivateData(refguid, pData, pSizeOfData);
        }

        STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid)
        {
            return m_pSurface9->FreePrivateData(refguid);
        }

        STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD Priority)
        {
            return m_pSurface9->SetPriority(Priority);
        }

        STDMETHOD_(DWORD, GetPriority)(THIS)
        {
            return m_pSurface9->GetPriority();
        }

        STDMETHOD_(void, PreLoad)(THIS)
        {
            m_pSurface9->PreLoad();
        }

        STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS)
        {
            return D3DRTYPE_SURFACE;
        }

        STDMETHOD(GetContainer)(THIS_ REFIID riid, void** ppContainer)
        {
            return m_pSurface9->GetContainer(riid, ppContainer);
        }

        STDMETHOD(GetDesc)(THIS_ D3DSURFACE_DESC* pDesc)
        {
            D3DSURFACE_DESC desc9;
            HRESULT hr = m_pSurface9->GetDesc(&desc9);
            if (SUCCEEDED(hr) && pDesc)
            {
                pDesc->Format = desc9.Format;
                pDesc->Type = desc9.Type;
                pDesc->Usage = desc9.Usage;
                pDesc->Pool = desc9.Pool;
                pDesc->Width = desc9.Width;
                pDesc->Height = desc9.Height;
                pDesc->MultiSampleType = (D3DMULTISAMPLE_TYPE)desc9.MultiSampleType;
            }
            return hr;
        }

        STDMETHOD(LockRect)(THIS_ D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
        {
            return m_pSurface9->LockRect(pLockedRect, pRect, Flags);
        }

        STDMETHOD(UnlockRect)(THIS)
        {
            return m_pSurface9->UnlockRect();
        }
    };

    class CDirect3DVertexBuffer8 : public IDirect3DVertexBuffer8
    {
    private:
        IDirect3DVertexBuffer9* m_pBuffer9;
        UINT m_Size;
        DWORD m_FVF;
        LONG m_RefCount;

    public:
        CDirect3DVertexBuffer8(IDirect3DVertexBuffer9* pBuffer9, UINT Size, DWORD FVF)
            : m_pBuffer9(pBuffer9), m_Size(Size), m_FVF(FVF), m_RefCount(1)
        {
            if (m_pBuffer9)
                m_pBuffer9->AddRef();
        }

        virtual ~CDirect3DVertexBuffer8()
        {
            if (m_pBuffer9)
                m_pBuffer9->Release();
        }

        IDirect3DVertexBuffer9* GetD3D9Buffer() const { return m_pBuffer9; }

        STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
        {
            if (ppvObj == nullptr)
                return E_INVALIDARG;

            if (riid == IID_IUnknown || riid == __uuidof(IDirect3DVertexBuffer8))
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

        STDMETHOD(GetDevice)(THIS_ IDirect3DDevice8** ppDevice)
        {
            if (ppDevice == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DDevice9* pDevice9 = nullptr;
            return m_pBuffer9->GetDevice(&pDevice9);
        }

        STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
        {
            return m_pBuffer9->SetPrivateData(refguid, pData, SizeOfData, Flags);
        }

        STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
        {
            return m_pBuffer9->GetPrivateData(refguid, pData, pSizeOfData);
        }

        STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid)
        {
            return m_pBuffer9->FreePrivateData(refguid);
        }

        STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD Priority)
        {
            return m_pBuffer9->SetPriority(Priority);
        }

        STDMETHOD_(DWORD, GetPriority)(THIS)
        {
            return m_pBuffer9->GetPriority();
        }

        STDMETHOD_(void, PreLoad)(THIS)
        {
            m_pBuffer9->PreLoad();
        }

        STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS)
        {
            return D3DRTYPE_VERTEXBUFFER;
        }

        STDMETHOD_(DWORD, GetLevelCount)(THIS)
        {
            return 1;
        }

        STDMETHOD(Lock)(THIS_ DWORD OffsetToLock, DWORD SizeToLock, BYTE** ppbData, DWORD Flags)
        {
            return m_pBuffer9->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
        }

        STDMETHOD(Unlock)(THIS)
        {
            return m_pBuffer9->Unlock();
        }

        STDMETHOD(GetDesc)(THIS_ D3DVERTEXBUFFER_DESC* pDesc)
        {
            if (pDesc == nullptr)
                return D3DERR_INVALIDCALL;

            pDesc->Format = D3DFMT_VERTEXDATA;
            pDesc->Type = D3DRTYPE_VERTEXBUFFER;
            pDesc->Size = m_Size;
            pDesc->Usage = 0;
            pDesc->Pool = D3DPOOL_MANAGED;
            pDesc->FVF = m_FVF;
            return D3D_OK;
        }
    };

    class CDirect3DIndexBuffer8 : public IDirect3DIndexBuffer8
    {
    private:
        IDirect3DIndexBuffer9* m_pBuffer9;
        UINT m_Size;
        D3DFORMAT m_Format;
        LONG m_RefCount;

    public:
        CDirect3DIndexBuffer8(IDirect3DIndexBuffer9* pBuffer9, UINT Size, D3DFORMAT Format)
            : m_pBuffer9(pBuffer9), m_Size(Size), m_Format(Format), m_RefCount(1)
        {
            if (m_pBuffer9)
                m_pBuffer9->AddRef();
        }

        virtual ~CDirect3DIndexBuffer8()
        {
            if (m_pBuffer9)
                m_pBuffer9->Release();
        }

        IDirect3DIndexBuffer9* GetD3D9Buffer() const { return m_pBuffer9; }

        STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
        {
            if (ppvObj == nullptr)
                return E_INVALIDARG;

            if (riid == IID_IUnknown || riid == __uuidof(IDirect3DIndexBuffer8))
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

        STDMETHOD(GetDevice)(THIS_ IDirect3DDevice8** ppDevice)
        {
            if (ppDevice == nullptr)
                return D3DERR_INVALIDCALL;

            IDirect3DDevice9* pDevice9 = nullptr;
            return m_pBuffer9->GetDevice(&pDevice9);
        }

        STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
        {
            return m_pBuffer9->SetPrivateData(refguid, pData, SizeOfData, Flags);
        }

        STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
        {
            return m_pBuffer9->GetPrivateData(refguid, pData, pSizeOfData);
        }

        STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid)
        {
            return m_pBuffer9->FreePrivateData(refguid);
        }

        STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD Priority)
        {
            return m_pBuffer9->SetPriority(Priority);
        }

        STDMETHOD_(DWORD, GetPriority)(THIS)
        {
            return m_pBuffer9->GetPriority();
        }

        STDMETHOD_(void, PreLoad)(THIS)
        {
            m_pBuffer9->PreLoad();
        }

        STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS)
        {
            return D3DRTYPE_INDEXBUFFER;
        }

        STDMETHOD(Lock)(THIS_ DWORD OffsetToLock, DWORD SizeToLock, BYTE** ppbData, DWORD Flags)
        {
            return m_pBuffer9->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
        }

        STDMETHOD(Unlock)(THIS)
        {
            return m_pBuffer9->Unlock();
        }

        STDMETHOD(GetDesc)(THIS_ D3DINDEXBUFFER_DESC* pDesc)
        {
            if (pDesc == nullptr)
                return D3DERR_INVALIDCALL;

            pDesc->Format = m_Format;
            pDesc->Type = D3DRTYPE_INDEXBUFFER;
            pDesc->Size = m_Size;
            pDesc->Usage = 0;
            pDesc->Pool = D3DPOOL_MANAGED;
            return D3D_OK;
        }
    };

    IDirect3D8* CreateD3D8Wrapper(IDirect3D9* pD3D9, UINT SDKVersion)
    {
        if (pD3D9 == nullptr)
            return nullptr;

        g_pD3D9 = pD3D9;
        g_SDKVersion = SDKVersion;

        return new CDirect3D8(pD3D9, SDKVersion);
    }
}
