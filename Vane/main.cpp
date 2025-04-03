#include "Vane/Vane.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <cassert>
#include <iostream>
#include <string>
#include <windows.h>

float width = 0.f;
float height = 0.f;
bool should_exit = false;
bool render_started = false;
int resize = 0;
static XyTextureID wallpaper = 0;

HWND                    g_hWnd = nullptr;
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

void Cleanup();
bool InitD3D();
void AddConfig();


LRESULT CALLBACK WndProc(HWND hWnd, uint32_t msg, uint64_t wParam, int64_t lParam)
{
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    else if (msg == WM_MOUSEMOVE)
    {
        return Vane::open ? S_OK : DefWindowProc(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_SIZE && wParam != SIZE_MINIMIZED && render_started)
    {
        resize = 1;

        while (resize != 2)
            std::this_thread::yield();
        // recreate render target
        ((ID3D11ShaderResourceView*)wallpaper)->Release();
        wallpaper = 0;
        Cleanup();

        if (!InitD3D())
        {
            should_exit = true;
            XY_ASSERT(false && "Failed to resize\n");
            return S_OK;
        }
        
        if (!Vane::Init(g_pSwapChain, XyVec2(800.f, 550.f)))
        {
            should_exit = true;
            XY_ASSERT(false && "Failed to resize\n" && Vane::Errors::Get().c_str());
            return S_OK;
        }

        resize = 0;
    }
    else if (msg == WM_SETCURSOR)
    {
        if (Vane::open && Vane::Cursor::actual.load() != Vane::Cursor::arrow)
        {
            Vane::Cursor::Set();
            return S_OK;
        }
        else
        {
            SetCursor(Vane::Cursor::arrow);
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }

    }

    auto ret = Vane::WndProc(msg, wParam, lParam);
    if (ret.has_value())
        return ret.value();

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

#include "Vane/Res/png/wallpaper_temp.h"

// Render a single frame.
void Render()
{
    while (!should_exit)
    {
        if (resize != 0)
            resize = 2;

        while (resize != 0)
            std::this_thread::yield();

        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        g_pd3dDeviceContext->ClearRenderTargetView(g_pRenderTargetView, clearColor);


        // -- Full Background (Remove this code) --
        {
            Vane::renderer.start_frame(false);

            constexpr float wallpaper_ratio = 1.f;
            float curr_ratio = Vane::renderer.data.display_size.x / Vane::renderer.data.display_size.y;

            XyVec2 uvMin, uvMax;

            if (curr_ratio > wallpaper_ratio)
            {
                float offset = (1.f - (Vane::renderer.data.display_size.y / Vane::renderer.data.display_size.x)) * 0.5f;
                uvMin = XyVec2(0.f, offset);
                uvMax = XyVec2(1.f, 1.f - offset);
            }
            else
            {
                float offset = (1.f - (Vane::renderer.data.display_size.x / Vane::renderer.data.display_size.y)) * 0.5f;
                uvMin = XyVec2(offset, 0.f);
                uvMax = XyVec2(1.f - offset, 1.f);
            }

            if (!wallpaper)
            {
                if (!Vane::Util::LoadIconXy(_acwallpaper_temp, sizeof(_acwallpaper_temp), &wallpaper))
                    XY_ASSERT(false && "Failed to create wallpaper!");
            }

            Vane::renderer.AddImage(wallpaper, XyVec2(0.f, 0.f), Vane::renderer.data.display_size, uvMin, uvMax);
            Vane::renderer.end_frame();
        }

        if (!Vane::Render())
            break;

        g_pSwapChain->Present(1, 0);
    }

    should_exit = true;
}

int WINAPI main()
{
    // Create Window
    WNDCLASSEXW wc = {
         sizeof(wc),
         CS_CLASSDC,
         WndProc,
         0L, 0L,
         GetModuleHandle(nullptr),
         nullptr, nullptr, nullptr, nullptr,
         L"Xyphra_Ext",
         nullptr
    };

    if (!::RegisterClassExW(&wc))
    {
        return -1;
    }

    width = (float)GetSystemMetrics(SM_CXSCREEN) / 1.35f;
    height = (float)GetSystemMetrics(SM_CYSCREEN) / 1.2f;

    g_hWnd = ::CreateWindowExW(
        0,
        wc.lpszClassName, L"Xyphra Ext",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, (int)width, (int)height,
        nullptr, nullptr, wc.hInstance, nullptr
    );

    if (!g_hWnd)
    {
        return -1;
    }

    // Init D3d11
    if (!InitD3D())
        return -1;

    // Add Widgets
    Vane::AddLabel("Other");

    Vane::AddDefaultThemeTab();
    Vane::AddConfigTab("Config");


    // Init Vane
    if (!Vane::Init(g_pSwapChain, XyVec2(800.f, 550.f)))
        return -1;

    // Add Config Modules
    AddConfig();

    // Show Window
    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);

    std::thread render_thread(Render);
    if (!render_thread.joinable())
        return -1;

    render_started = true;

    MSG msg = {};
    while (msg.message != WM_QUIT && !should_exit)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    should_exit = true;
    render_thread.join();

    Vane::Destroy();
    Cleanup();
    return (int)msg.wParam;
}

#define Add Vane::Config::AddModule
#define AddCfg(value) Vane::Config::AddModule<decltype(value)>(&value, #value)
void AddConfig()
{
	/// Menu
	AddCfg(Vane::Style::Background);
	AddCfg(Vane::Style::Text);
	AddCfg(Vane::Style::Accent);
	AddCfg(Vane::Style::Accent2);
	AddCfg(Vane::Style::Icon);
	AddCfg(Vane::Style::Highlight);
	AddCfg(Vane::Style::AnimationSpeed);
	AddCfg(Vane::Style::Rounding);
	AddCfg(Vane::Style::ChildWindowWidth);

	/// Background
	AddCfg(Vane::Background::ConstantBuffer->AnimationColor);
	AddCfg(Vane::Background::ConstantBuffer->TURB_AMP);
	AddCfg(Vane::Background::ConstantBuffer->TURB_EXP);
	AddCfg(Vane::Background::ConstantBuffer->TURB_FREQ);
	AddCfg(Vane::Background::ConstantBuffer->TURB_NUM);
	AddCfg(Vane::Background::ConstantBuffer->TURB_SPEED);
	AddCfg(Vane::Background::ConstantBuffer->BlurEnabled);
	AddCfg(Vane::Background::ConstantBuffer->BlurMenuOnly);
	AddCfg(Vane::Background::ConstantBuffer->NoiseScale);
	AddCfg(Vane::Background::ConstantBuffer->ShadowAlpha);
	AddCfg(Vane::Background::ConstantBuffer->ShadowSize);
}



void Cleanup()
{
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pSwapChain)        g_pSwapChain->Release();
    if (g_pd3dDeviceContext) g_pd3dDeviceContext->Release();
    if (g_pd3dDevice)        g_pd3dDevice->Release();
}

bool InitD3D()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 8;
    sd.SampleDesc.Quality = 2;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    uint32_t createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    long res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
    {
        return  false;
    }

    ID3D11Texture2D* pBackBuffer;
    res = g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (res != S_OK)
    {
        return false;
    }

    res = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    if (res != S_OK)
    {
        return false;
    }

    return true;
}