#include <vector>
#include <random>
#include "Vane.h"
#include <DirectXMath.h>
#include "Res/shader/Shaders.h"


bool Vane::Background::Init()
{
    long hr = S_OK;

    hr = renderer.g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&m_pBackBuffer));
    if (FAILED(hr))
    {
        Errors::Set("Failed to get backbuffer");
        return false;
    }

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = renderer.g_backBufferDesc.Width;
    texDesc.Height = renderer.g_backBufferDesc.Height;
    texDesc.MipLevels = renderer.g_backBufferDesc.MipLevels;
    texDesc.ArraySize = 1;
    texDesc.Format = renderer.g_backBufferDesc.Format;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    hr = renderer.g_pd3dDevice->CreateTexture2D(&texDesc, nullptr, &m_pOffscreenTexture);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create offscreen texture");
        return false;
    }

    hr = renderer.g_pd3dDevice->CreateShaderResourceView(m_pOffscreenTexture, nullptr,
        &m_pOffscreenSRV);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create shader resource view");
        return false;
    }

    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    hr = renderer.g_pd3dDevice->CreateTexture2D(&texDesc, nullptr, &m_p1PassTexture);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create Shader Blur texture");
        return false;
    }

    hr = renderer.g_pd3dDevice->CreateShaderResourceView(m_p1PassTexture, nullptr,
        &m_p1PassSRV);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create shader resource view 2");
        return false;
    }

    hr = renderer.g_pd3dDevice->CreateRenderTargetView(m_p1PassTexture, nullptr, &m_p1PassRTV);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create vertical blur RTV");
        return false;
    }

    hr = renderer.g_pd3dDevice->CreateVertexShader(Shaders::_acVertexShader, sizeof(Shaders::_acVertexShader) - 1,
        nullptr, &m_pQuadVS);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create vertex shader");
        return false;
    }

    // Define the input layout for the quad vertices.
    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(XyVec4, x),
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(XyVec4, z),
          D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    hr = renderer.g_pd3dDevice->CreateInputLayout(layoutDesc, 2,
        Shaders::_acVertexShader,
        sizeof(Shaders::_acVertexShader) - 1,
        &m_pInputLayout);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create input layout");
        return false;
    }

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = renderer.g_pd3dDevice->CreateBlendState(&blendDesc, &m_pBlendState);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create blend state.");
        return false;
    }

    // --- Pixel Shader (Blur) ---
    hr = renderer.g_pd3dDevice->CreatePixelShader(Shaders::_acBlurHorizontaletc, sizeof(Shaders::_acBlurHorizontaletc) - 1,
        nullptr, &m_pBlurPS);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create pixel shader.");
        return false;
    }


    // --- Pixel Shader 2 (Blur Vertical) ---
    hr = renderer.g_pd3dDevice->CreatePixelShader(Shaders::_acBlurVertical, sizeof(Shaders::_acBlurVertical) - 1,
        nullptr, &m_pBlurPSV);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create pixel shader 2.");
        return false;
    }

    // Create constant buffer for blur parameters.
    if (!ConstantBuffer.Init(renderer.g_pd3dDevice))
    {
        Errors::Set("Failed to create constant buffer.");
        return false;
    }


    // Vertex Buffer
    // x and y are pos, z and w are uv
    XyVec4 vertices[] = {
        { -1.0f, -1.0f, 0.0f, 1.0f },
        { -1.0f,  1.0f, 0.0f, 0.0f },
        { 1.0f, -1.0f, 1.0f, 1.0f },
        { 1.0f,  1.0f, 1.0f, 0.0f },
    };

    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = sizeof(vertices);
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    hr = renderer.g_pd3dDevice->CreateBuffer(&vbDesc, &initData, &m_pQuadVB);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create vertex buffer.");
        return false;
    }

    // Index Buffer
    unsigned int indices[] = {
        0, 1, 2, // First triangle
        2, 1, 3  // Second triangle
    };

    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.ByteWidth = sizeof(indices);
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    initData = {};
    initData.pSysMem = indices;

    hr = renderer.g_pd3dDevice->CreateBuffer(&ibDesc, &initData, &m_pQuadIB);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create vertex buffer.");
        return false;
    }

    // Sample State
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = renderer.g_pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create sampler state.");
        return false;
    }

    return true;
}

void Vane::Background::Render()
{
    D3D11_VIEWPORT viewport = {};
    viewport.Width = renderer.data.display_size.x;
    viewport.Height = renderer.data.display_size.y;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    renderer.g_pd3dDeviceContext->RSSetViewports(1, &viewport);

    if (renderer.g_backBufferDesc.SampleDesc.Count > 1)
        renderer.g_pd3dDeviceContext->ResolveSubresource(m_pOffscreenTexture, 0, m_pBackBuffer, 0, renderer.g_backBufferDesc.Format);
    else
        renderer.g_pd3dDeviceContext->CopyResource(m_pOffscreenTexture, m_pBackBuffer);

    // --- First Pass: Vertical Blur ---
    renderer.g_pd3dDeviceContext->OMSetRenderTargets(1, &m_p1PassRTV, nullptr);
    renderer.g_pd3dDeviceContext->RSSetViewports(1, &viewport);

    float clearColor[4] = { 0, 0, 0, 0 };
    renderer.g_pd3dDeviceContext->ClearRenderTargetView(m_p1PassRTV, clearColor);

    uint32_t stride = sizeof(XyVec4);
    uint32_t offset = 0;
    renderer.g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pQuadVB, &stride, &offset);
    renderer.g_pd3dDeviceContext->IASetIndexBuffer(m_pQuadIB, DXGI_FORMAT_R32_UINT, 0);
    renderer.g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer.g_pd3dDeviceContext->IASetInputLayout(m_pInputLayout);

    renderer.g_pd3dDeviceContext->VSSetShader(m_pQuadVS, nullptr, 0);
    renderer.g_pd3dDeviceContext->PSSetShader(m_pBlurPSV, nullptr, 0);

    renderer.g_pd3dDeviceContext->PSSetShaderResources(0, 1, &m_pOffscreenSRV);
    renderer.g_pd3dDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

    ConstantBuffer->iResolution = renderer.data.display_size;
    ConstantBuffer->Animation = animation;
    ConstantBuffer->iTime += renderer.data.delta_time;
    ConstantBuffer->MenuPos = XyVec4(x, y, w, h);
    ConstantBuffer->Rounding = Style::Rounding;
    ConstantBuffer->MenuBackgroundColor = Style::Background;
    // Blur
    ConstantBuffer->samples = (int)round(BlurRadius * 2.f);
    ConstantBuffer->LOD = HighQuality ? 0 : 2;
    ConstantBuffer->sLOD = (1 << ConstantBuffer->LOD);
    ConstantBuffer->sigma = (float)ConstantBuffer->samples * 0.25f;
    //static const float gaussianNormalization2D = 1.0 / (6.28318530718 * sigma * sigma);
    ConstantBuffer->gaussianNormalization1D = 1.0f / (sqrt(6.28318530718f) * ConstantBuffer->sigma * ConstantBuffer->sigma);

    ConstantBuffer.Update(renderer.g_pd3dDeviceContext);
    renderer.g_pd3dDeviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);

    float blendFactor[4] = { 0, 0, 0, 0 };
    uint32_t sampleMask = 0xffffffff;
    renderer.g_pd3dDeviceContext->OMSetBlendState(m_pBlendState, blendFactor, sampleMask);

    renderer.g_pd3dDeviceContext->DrawIndexed(6, 0, 0);

    // --- First Pass: Horizontal Blur and Menu Background ---
    renderer.g_pd3dDeviceContext->OMSetRenderTargets(1, &renderer.g_pd3dRenderTargetView, nullptr);
    renderer.g_pd3dDeviceContext->RSSetViewports(1, &viewport);

    renderer.g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pQuadVB, &stride, &offset);
    renderer.g_pd3dDeviceContext->IASetIndexBuffer(m_pQuadIB, DXGI_FORMAT_R32_UINT, 0);
    renderer.g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer.g_pd3dDeviceContext->IASetInputLayout(m_pInputLayout);

    renderer.g_pd3dDeviceContext->VSSetShader(m_pQuadVS, nullptr, 0);
    renderer.g_pd3dDeviceContext->PSSetShader(m_pBlurPS, nullptr, 0);

    renderer.g_pd3dDeviceContext->PSSetShaderResources(0, 1, &m_p1PassSRV);
    renderer.g_pd3dDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

    renderer.g_pd3dDeviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
    renderer.g_pd3dDeviceContext->OMSetBlendState(m_pBlendState, blendFactor, sampleMask);

    renderer.g_pd3dDeviceContext->DrawIndexed(6, 0, 0);
}

void Vane::Background::Destroy()
{
    if (m_pOffscreenTexture) m_pOffscreenTexture->Release();
    if (m_pOffscreenSRV) m_pOffscreenSRV->Release();
    if (m_pBackBuffer) m_pBackBuffer->Release();
    if (m_pQuadVS) m_pQuadVS->Release();
    if (m_pBlurPS) m_pBlurPS->Release();
    if (m_pInputLayout) m_pInputLayout->Release();
    if (m_pQuadVB) m_pQuadVB->Release();
    if (m_pQuadIB) m_pQuadIB->Release();
    if (m_pSamplerState) m_pSamplerState->Release();
    if (m_pBlendState) m_pBlendState->Release();

    ConstantBuffer.Destroy();
}