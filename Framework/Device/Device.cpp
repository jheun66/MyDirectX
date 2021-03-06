#include "Framework.h"
#include "Device.h"

// 스태틱 변수 초기화
Device* Device::instance = nullptr;

Device::Device()
{
    CreateDevcieAndSwapChain();
    CreateBackBuffer();
}

Device::~Device()
{
    renderTargetView->Release();
    swapChain->Release();
    deviceContext->Release();
    device->Release();
}

void Device::CreateDevcieAndSwapChain()
{

    UINT width = WIN_WIDTH;
    UINT height = WIN_HEIGHT;

    // 스왑체인과 관련되 정보를 담고 있는 구조체
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1; // 백버퍼 개수
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // unorm 양수인 0 ~ 1 까지 값
    // RefreshRate : 디스플레이 모드 갱신율(주사율 : Numerator / Denominator) (fps랑은 다름)
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;

    // 다중 표본화(안씀)
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;

    // 창모드
    sd.Windowed = true;

    vector<IDXGIAdapter1*> adpaters = EnumerateAdpater();

    UINT maxVideoMemory = 0;
    UINT selectedAdapterIndex = 0;
    string gpuName;
    wstring gpuDesc;
    for (int i = 0; i < adpaters.size(); i++)
    {
        DXGI_ADAPTER_DESC1 desc;
        adpaters[i]->GetDesc1(&desc);
        if (desc.DedicatedVideoMemory > maxVideoMemory)
        {
            maxVideoMemory = desc.DedicatedVideoMemory;
            selectedAdapterIndex = i;
            gpuDesc = desc.Description;
        }
    }

    // 그래픽 카드 이름 확인
    gpuName.assign(gpuDesc.begin(),gpuDesc.end());

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        adpaters[selectedAdapterIndex],
        D3D_DRIVER_TYPE_UNKNOWN,
        0,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &swapChain,
        &device,
        nullptr,
        &deviceContext
    );
    assert(SUCCEEDED(hr));

}

void Device::CreateBackBuffer()
{
    ID3D11Texture2D* backBuffer;

    HRESULT hr;

    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    assert(SUCCEEDED(hr));

    hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    assert(SUCCEEDED(hr));

    // back버퍼는 사용해제
    backBuffer->Release();

    ID3D11Texture2D* depthBuffer;

    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = WIN_WIDTH;
    depthDesc.Height = WIN_HEIGHT;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
    assert(SUCCEEDED(hr));

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = depthDesc.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    hr = device->CreateDepthStencilView(depthBuffer, &descDSV, &depthStencilView);
    assert(SUCCEEDED(hr));

    depthBuffer->Release();

    SetRenderTarget();

}

void Device::SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
    if (rtv == nullptr)
    {
        rtv = renderTargetView;
    }
    if (dsv == nullptr)
    {
        dsv = depthStencilView;
    }

    deviceContext->OMSetRenderTargets(1, &rtv, dsv);
}

void Device::Clear(XMFLOAT4 color, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
    if (rtv == nullptr)
        rtv = renderTargetView;
    if (dsv == nullptr)
        dsv = depthStencilView;


    deviceContext->ClearRenderTargetView(rtv, (float*)&color);
    deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::Present()
{
    swapChain->Present(0, 0);
}

vector<IDXGIAdapter1*> Device::EnumerateAdpater()
{
    IDXGIAdapter1* pAdapter;
    vector<IDXGIAdapter1*> vAdapters;
    IDXGIFactory1* pFactory = NULL;


    // Create a DXGIFactory object.
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
    {
        return vAdapters;
    }


    for (UINT i = 0;
        pFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
        ++i)
    {
        vAdapters.push_back(pAdapter);
    }


    if (pFactory)
    {
        pFactory->Release();
    }

    return vAdapters;
}
