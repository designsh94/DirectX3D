#include "PreCompile.h"
#include "GameEngineDepthBuffer.h"

GameEngineDepthBuffer::GameEngineDepthBuffer() :
	DepthTexture_(nullptr)
{
}

GameEngineDepthBuffer::~GameEngineDepthBuffer() 
{
	if (nullptr != DepthTexture_)
	{
		delete DepthTexture_;
		DepthTexture_ = nullptr;
	}
}

void GameEngineDepthBuffer::Create(float4 _Scale)
{
	DepthTexture_ = new GameEngineTexture();

	D3D11_TEXTURE2D_DESC Info = {0};

	D3D11_TEXTURE2D_DESC TextureInfo = { 0, };
	TextureInfo.ArraySize = 1;
	TextureInfo.Width = _Scale.uix();
	TextureInfo.Height = _Scale.uiy();
	TextureInfo.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureInfo.SampleDesc.Count = 1;
	TextureInfo.SampleDesc.Quality = 0;
	TextureInfo.MipLevels = 1;
	TextureInfo.Usage = D3D11_USAGE_DEFAULT;
	TextureInfo.CPUAccessFlags = 0;
	TextureInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

	DepthTexture_->Create(TextureInfo);
}
