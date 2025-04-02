#include "Xyphra.h"


void DX11Data::clear()
{
	if (pFontSampler) { pFontSampler->Release(); pFontSampler = nullptr; }
	if (pFontTextureView) { pFontTextureView->Release(); pFontTextureView = nullptr; }// Vane::renderer.GetIO().Fonts->SetTexID(0); } // We copied data->pFontTextureView to io.Fonts->TexID so let's clear that as well.
	if (pIB) { pIB->Release(); pIB = nullptr; }
	if (pVB) { pVB->Release(); pVB = nullptr; }
	if (pBlendState) { pBlendState->Release(); pBlendState = nullptr; }
	if (pDepthStencilState) { pDepthStencilState->Release(); pDepthStencilState = nullptr; }
	if (pRasterizerState) { pRasterizerState->Release(); pRasterizerState = nullptr; }
	if (pPixelShader) { pPixelShader->Release(); pPixelShader = nullptr; }
	if (pVertexConstantBuffer) { pVertexConstantBuffer->Release(); pVertexConstantBuffer = nullptr; }
	if (pInputLayout) { pInputLayout->Release(); pInputLayout = nullptr; }
	if (pVertexShader) { pVertexShader->Release(); pVertexShader = nullptr; }
}
