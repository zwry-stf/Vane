#include "Xyphra.h"
#include <d3dcompiler.h>

#define save_release(v) if (v) v->Release(); v = nullptr

_Xyphra::_Xyphra() : Fonts(this)
{
}

DXGI_FORMAT GetCorrectFormat(DXGI_FORMAT curr)
{
    switch (curr)
    {
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    case DXGI_FORMAT_BC1_UNORM_SRGB:
        return DXGI_FORMAT_BC1_UNORM;
    case DXGI_FORMAT_BC2_UNORM_SRGB:
        return DXGI_FORMAT_BC2_UNORM;
    case DXGI_FORMAT_BC3_UNORM_SRGB:
        return DXGI_FORMAT_BC3_UNORM;
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return DXGI_FORMAT_BC7_UNORM;
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        return DXGI_FORMAT_B8G8R8X8_UNORM;
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    }

    return curr;
}

bool _Xyphra::init(IDXGISwapChain* pSwapChain)
{
    if (!pSwapChain)
        return false;

    g_pSwapChain = pSwapChain;
    g_pSwapChain->AddRef();

    // Device
    if (FAILED(
        pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice)) ||
        !g_pd3dDevice)
        return false;

    // Context
    g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
    if (!g_pd3dDeviceContext)
        return false;

    // Window
    pSwapChain->GetDesc(&g_swapChainDesc);

    g_hWnd = g_swapChainDesc.OutputWindow;
    if (!g_hWnd)
        return false;

    // Create Render Target
    ID3D11Texture2D* pBackBuffer = nullptr;
    if (FAILED(
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        return false;

    pBackBuffer->GetDesc(&g_backBufferDesc);
    data.display_size = XyVec2((float)g_backBufferDesc.Width, (float)g_backBufferDesc.Height);

    D3D11_RENDER_TARGET_VIEW_DESC desc{};
    desc.Format = GetCorrectFormat(g_swapChainDesc.BufferDesc.Format);
    desc.ViewDimension = g_swapChainDesc.SampleDesc.Count > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
    long hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &desc, &g_pd3dRenderTargetView);
    if (FAILED(hr))
    {
        hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pd3dRenderTargetView);
        if (FAILED(hr))
        {
            pBackBuffer->Release();
            return false;
        }
    }
    pBackBuffer->Release();

    if (!create_dx11_resources())
        return false;

    return true;
}

void _Xyphra::destroy()
{
    save_release(g_pd3dRenderTargetView);
    save_release(g_pd3dDevice);
    save_release(g_pd3dDeviceContext);
    save_release(g_pSwapChain);

    dx11.clear();
}

bool _Xyphra::start_frame(bool update_frame_time)
{
    if (!g_pd3dDeviceContext || !g_pd3dRenderTargetView || !g_hWnd || !g_pd3dDevice)
        return false;

    RECT rect = { 0, 0, 0, 0 };
    if (!GetClientRect(g_hWnd, &rect))
        return false;

    data.display_size = XyVec2((float)rect.right, (float)rect.bottom);

    if (update_frame_time)
    {
        auto now = std::chrono::steady_clock::now();
        data.delta_time = std::chrono::duration<float>(now - bg.time).count();
        data.curr_time = GetTickCount64();
        bg.time = now;
    }

    shared_data.ClipRectFullscreen = XyVec4(0.f, 0.f, data.display_size.x, data.display_size.y);

    CmdBuffer.resize(0);
    IdxBuffer.resize(0);
    VtxBuffer.resize(0);
    Flags = shared_data.InitialFlags;
    memset(&_CmdHeader, 0, sizeof(_CmdHeader));
    _VtxCurrentIdx = 0;
    _VtxWritePtr = NULL;
    _IdxWritePtr = NULL;
    _ClipRectStack.resize(0);
    _TextureIdStack.resize(0);
    _Path.resize(0);
    CmdBuffer.push_back(XyCmd());
    _FringeScale = 1.0f;

    ///

    shared_data.SetCircleTessellationMaxError(0.30f);
    PushClipRect(XyVec2(0, 0), data.display_size, false);
    SetCurrentFont(Fonts.Fonts[0]);
    PushTextureID(Fonts.TexID);

    Fonts.Locked = true;

    return true;
}


void _Xyphra::setup_render_state()
{
    ID3D11DeviceContext* ctx = g_pd3dDeviceContext;

    D3D11_VIEWPORT vp;
    memset(&vp, 0, sizeof(D3D11_VIEWPORT));
    vp.Width = data.display_size.x;
    vp.Height = data.display_size.y;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = vp.TopLeftY = 0;
    ctx->RSSetViewports(1, &vp);

    // Setup shader and vertex buffers
    unsigned int stride = sizeof(XyVertex);
    unsigned int offset = 0;
    ctx->IASetInputLayout(dx11.pInputLayout);
    ctx->IASetVertexBuffers(0, 1, &dx11.pVB, &stride, &offset);
    ctx->IASetIndexBuffer(dx11.pIB, sizeof(XyIndex) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ctx->VSSetShader(dx11.pVertexShader, nullptr, 0);
    ctx->VSSetConstantBuffers(0, 1, &dx11.pVertexConstantBuffer);
    ctx->PSSetShader(dx11.pPixelShader, nullptr, 0);
    ctx->PSSetSamplers(0, 1, &dx11.pFontSampler);
    ctx->GSSetShader(nullptr, nullptr, 0);
    ctx->HSSetShader(nullptr, nullptr, 0); // In theory we should backup and restore this as well.. very infrequently used..
    ctx->DSSetShader(nullptr, nullptr, 0); // In theory we should backup and restore this as well.. very infrequently used..
    ctx->CSSetShader(nullptr, nullptr, 0); // In theory we should backup and restore this as well.. very infrequently used..

    // Setup blend state
    const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
    ctx->OMSetBlendState(dx11.pBlendState, blend_factor, 0xffffffff);
    ctx->OMSetDepthStencilState(dx11.pDepthStencilState, 0);
    ctx->RSSetState(dx11.pRasterizerState);
}


void _Xyphra::end_frame()
{
    int total_vertex = 0;
    int total_index = 0;


    while (CmdBuffer.Size > 0)
    {
        XyCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
        if (curr_cmd->ElemCount != 0)
            break;
        CmdBuffer.pop_back();
    }

    total_vertex += VtxBuffer.Size;
    total_index += IdxBuffer.Size;

    ID3D11DeviceContext* ctx = g_pd3dDeviceContext;

    // Create and grow vertex/index buffers if needed
    if (!dx11.pVB || dx11.VertexBufferSize < total_vertex)
    {
        if (dx11.pVB) { dx11.pVB->Release(); dx11.pVB = nullptr; }
        dx11.VertexBufferSize = total_vertex + 5000;
        D3D11_BUFFER_DESC desc;
        memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = dx11.VertexBufferSize * sizeof(XyVertex);
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        if (g_pd3dDevice->CreateBuffer(&desc, nullptr, &dx11.pVB) < 0)
            return;
    }
    if (!dx11.pIB || dx11.IndexBufferSize < total_index)
    {
        if (dx11.pIB) { dx11.pIB->Release(); dx11.pIB = nullptr; }
        dx11.IndexBufferSize = total_index + 10000;
        D3D11_BUFFER_DESC desc;
        memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = dx11.IndexBufferSize * sizeof(XyIndex);
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        if (g_pd3dDevice->CreateBuffer(&desc, nullptr, &dx11.pIB) < 0)
            return;
    }

    // Upload vertex/index data into a single contiguous GPU buffer
    D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
    if (ctx->Map(dx11.pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)
        return;
    if (ctx->Map(dx11.pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)
        return;
    XyVertex* vtx_dst = (XyVertex*)vtx_resource.pData;
    XyIndex* idx_dst = (XyIndex*)idx_resource.pData;
    memcpy(vtx_dst, VtxBuffer.Data, VtxBuffer.Size * sizeof(XyVertex));
    memcpy(idx_dst, IdxBuffer.Data, IdxBuffer.Size * sizeof(XyIndex));
    vtx_dst += VtxBuffer.Size;
    idx_dst += IdxBuffer.Size;
    ctx->Unmap(dx11.pVB, 0);
    ctx->Unmap(dx11.pIB, 0);

    // Setup orthographic projection matrix into our constant buffer
    // Our visible imgui space lies from XyVec2(0.f, 0.f) (top left) to XyVec2(0.f, 0.f)+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        if (ctx->Map(dx11.pVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource) != S_OK)
            return;
        VERTEX_CONSTANT_BUFFER_DX11* constant_buffer = (VERTEX_CONSTANT_BUFFER_DX11*)mapped_resource.pData;
        float L = XyVec2(0.f, 0.f).x;
        float R = XyVec2(0.f, 0.f).x + data.display_size.x;
        float T = XyVec2(0.f, 0.f).y;
        float B = XyVec2(0.f, 0.f).y + data.display_size.y;
        float mvp[4][4] =
        {
            { 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
            { 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
            { 0.0f,         0.0f,           0.5f,       0.0f },
            { (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
        };
        memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
        ctx->Unmap(dx11.pVertexConstantBuffer, 0);
    }

    // Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
    struct BACKUP_DX11_STATE
    {
        uint32_t                        ScissorRectsCount, ViewportsCount;
        D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
        D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
        ID3D11RasterizerState* RS;
        ID3D11BlendState* BlendState;
        FLOAT                       BlendFactor[4];
        uint32_t                        SampleMask;
        uint32_t                        StencilRef;
        ID3D11DepthStencilState* DepthStencilState;
        ID3D11ShaderResourceView* PSShaderResource;
        ID3D11SamplerState* PSSampler;
        ID3D11PixelShader* PS;
        ID3D11VertexShader* VS;
        ID3D11GeometryShader* GS;
        uint32_t                        PSInstancesCount, VSInstancesCount, GSInstancesCount;
        ID3D11ClassInstance* PSInstances[256], * VSInstances[256], * GSInstances[256];   // 256 is max according to PSSetShader documentation
        D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
        ID3D11Buffer* IndexBuffer, * VertexBuffer, * VSConstantBuffer;
        uint32_t                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
        DXGI_FORMAT                 IndexBufferFormat;
        ID3D11InputLayout* InputLayout;
    };
    BACKUP_DX11_STATE old = {};
    old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    ctx->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
    ctx->RSGetViewports(&old.ViewportsCount, old.Viewports);
    ctx->RSGetState(&old.RS);
    ctx->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
    ctx->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
    ctx->PSGetShaderResources(0, 1, &old.PSShaderResource);
    ctx->PSGetSamplers(0, 1, &old.PSSampler);
    old.PSInstancesCount = old.VSInstancesCount = old.GSInstancesCount = 256;
    ctx->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
    ctx->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
    ctx->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
    ctx->GSGetShader(&old.GS, old.GSInstances, &old.GSInstancesCount);

    ctx->IAGetPrimitiveTopology(&old.PrimitiveTopology);
    ctx->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
    ctx->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
    ctx->IAGetInputLayout(&old.InputLayout);

    // Setup desired DX state
    setup_render_state();

    // Render command lists
    // (Because we merged all buffers into a single one, we maintain our own offset into them)
    int global_idx_offset = 0;
    int global_vtx_offset = 0;
    for (int cmd_i = 0; cmd_i < CmdBuffer.Size; cmd_i++)
    {
        XyCmd* pcmd = &CmdBuffer[cmd_i];

        // Project scissor/clipping rectangles into framebuffer space
        if (pcmd->ClipRect.z <= pcmd->ClipRect.x || pcmd->ClipRect.w <= pcmd->ClipRect.y)
            continue;

        // Apply scissor/clipping rectangle
        const D3D11_RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
        ctx->RSSetScissorRects(1, &r);

        // Bind texture, Draw
        ID3D11ShaderResourceView* texture_srv = (ID3D11ShaderResourceView*)pcmd->GetTexID();
        if (!texture_srv || texture_srv == (ID3D11ShaderResourceView*)0xFFFFFFFFFFFFFFFF)
            texture_srv = dx11.pFontTextureView;
        ctx->PSSetShaderResources(0, 1, &texture_srv);
        ctx->DrawIndexed(pcmd->ElemCount, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset);
    }
    global_idx_offset += IdxBuffer.Size;
    global_vtx_offset += VtxBuffer.Size;

    // Restore modified DX state
    ctx->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
    ctx->RSSetViewports(old.ViewportsCount, old.Viewports);
    ctx->RSSetState(old.RS); if (old.RS) old.RS->Release();
    ctx->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
    ctx->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
    ctx->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
    ctx->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
    ctx->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
    for (uint32_t i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
    ctx->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
    ctx->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
    ctx->GSSetShader(old.GS, old.GSInstances, old.GSInstancesCount); if (old.GS) old.GS->Release();
    for (uint32_t i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
    ctx->IASetPrimitiveTopology(old.PrimitiveTopology);
    ctx->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
    ctx->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
    ctx->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();


    Fonts.Locked = false;
}

bool _Xyphra::create_dx11_resources()
{
    dx11.clear();

    // By using D3DCompile() from <d3dcompiler.h> / d3dcompiler.lib, we introduce a dependency to a given version of d3dcompiler_XX.dll (see D3DCOMPILER_DLL_A)
    // If you would like to use this DX11 sample code but remove this dependency you can:
    //  1) compile once, save the compiled shader blobs into a file or source code and pass them to CreateVertexShader()/CreatePixelShader() [preferred solution]
    //  2) use code to detect any version of the DLL and grab a pointer to D3DCompile from the DLL.
    // See https://github.com/ocornut/imgui/pull/638 for sources and details.


    // Create the vertex shader
    {
        static const char* vertexShader =
            "cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";

        ID3DBlob* vertexShaderBlob;
        if (FAILED(D3DCompile(vertexShader, strlen(vertexShader), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &vertexShaderBlob, nullptr)))
            return false; // NB: Pass ID3DBlob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
        if (g_pd3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &dx11.pVertexShader) != S_OK)
        {
            vertexShaderBlob->Release();
            return false;
        }

        // Create the input layout
        D3D11_INPUT_ELEMENT_DESC local_layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (uint32_t)offsetof(XyVertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (uint32_t)offsetof(XyVertex, uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (uint32_t)offsetof(XyVertex, col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        if (g_pd3dDevice->CreateInputLayout(local_layout, 3, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &dx11.pInputLayout) != S_OK)
        {
            vertexShaderBlob->Release();
            return false;
        }
        vertexShaderBlob->Release();

        // Create the constant buffer
        {
            D3D11_BUFFER_DESC desc;
            desc.ByteWidth = sizeof(VERTEX_CONSTANT_BUFFER_DX11);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            g_pd3dDevice->CreateBuffer(&desc, nullptr, &dx11.pVertexConstantBuffer);
        }
    }

    // Create the pixel shader
    {
        static const char* pixelShader =
            "struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
            return out_col; \
            }";

        ID3DBlob* pixelShaderBlob;
        if (FAILED(D3DCompile(pixelShader, strlen(pixelShader), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0, &pixelShaderBlob, nullptr)))
            return false; // NB: Pass ID3DBlob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
        if (g_pd3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &dx11.pPixelShader) != S_OK)
        {
            pixelShaderBlob->Release();
            return false;
        }
        pixelShaderBlob->Release();
    }

    // Create the blending setup
    {
        D3D11_BLEND_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.AlphaToCoverageEnable = false;
        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        g_pd3dDevice->CreateBlendState(&desc, &dx11.pBlendState);
    }

    // Create the rasterizer state
    {
        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_NONE;
        desc.ScissorEnable = true;
        desc.DepthClipEnable = true;
        g_pd3dDevice->CreateRasterizerState(&desc, &dx11.pRasterizerState);
    }

    // Create depth-stencil State
    {
        D3D11_DEPTH_STENCIL_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.DepthEnable = false;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        desc.StencilEnable = false;
        desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        desc.BackFace = desc.FrontFace;
        g_pd3dDevice->CreateDepthStencilState(&desc, &dx11.pDepthStencilState);
    }

    // Create Font Textures
    // Build texture atlas
    unsigned char* pixels;
    int width, height;
    Fonts.GetTexDataAsRGBA32(&pixels, &width, &height);

    // Upload texture to graphics system
    {
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = nullptr;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = pixels;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
        XY_ASSERT(pTexture != nullptr);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &dx11.pFontTextureView);
        pTexture->Release();
        if (!dx11.pFontTextureView)
            return false;
    }

    // Store our identifier
    Fonts.SetTexID((XyTextureID)dx11.pFontTextureView);

    // Create texture sampler
    // (Bilinear sampling is required by default. Set 'io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines' or 'style.AntiAliasedLinesUseTex = false' to allow point/nearest sampling)
    {
        D3D11_SAMPLER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MipLODBias = 0.f;
        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.MinLOD = 0.f;
        desc.MaxLOD = 0.f;
        long hr = g_pd3dDevice->CreateSamplerState(&desc, &dx11.pFontSampler);
        if (FAILED(hr) || !dx11.pFontSampler)
            return false;
    }

    return true;
}