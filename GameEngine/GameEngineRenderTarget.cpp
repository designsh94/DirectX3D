#include "PreCompile.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineTextureManager.h"
#include "GameEngineTexture.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineDepthBuffer.h"

GameEngineRenderTarget* GameEngineRenderTarget::LastRenderTarget = nullptr;
GameEngineDepthBuffer* GameEngineRenderTarget::LastDepthBuffer = nullptr;

GameEngineRenderTarget::GameEngineRenderTarget() :
	DepthBuffer_(nullptr)
{
	Pipe_ = GameEngineRenderingPipeLineManager::GetInst().Find("TargetMerge");
	Res_.ShaderResourcesCheck(Pipe_);
}

GameEngineRenderTarget::~GameEngineRenderTarget()
{
	for (size_t i = 0; i < ReleaseTextures_.size(); i++)
	{
		delete ReleaseTextures_[i];
	}
	
	if (nullptr != DepthBuffer_)
	{
		delete DepthBuffer_;
	}
}

GameEngineRenderTarget::GameEngineRenderTarget(GameEngineRenderTarget&& _other) noexcept :
	DepthBuffer_(_other.DepthBuffer_),
	Pipe_(_other.Pipe_)
{
}

void GameEngineRenderTarget::Clear(bool _Death) 
{
	for (size_t i = 0; i < RenderTargetViews_.size(); i++)
	{
		GameEngineDevice::GetContext()->ClearRenderTargetView(RenderTargetViews_[i], ClearColor_[i].Arr1D);
	}

	if (nullptr != DepthBuffer_ && true == _Death)
	{
		GameEngineDevice::GetContext()->ClearDepthStencilView(DepthBuffer_->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void GameEngineRenderTarget::Setting(int _Index)
{
	if (0 >= RenderTargetViews_.size())
	{
		GameEngineDebug::MsgBoxError("Render Target Setting Error Size Zero");
	}

	ID3D11DepthStencilView* View = nullptr;
	if (nullptr != DepthBuffer_)
	{
		View = DepthBuffer_->GetDepthStencilView();
		LastDepthBuffer = DepthBuffer_;
	}

	LastRenderTarget = this;
	if (-1 == _Index)
	{
		GameEngineDevice::GetContext()->OMSetRenderTargets(static_cast<UINT>(RenderTargetViews_.size()), &RenderTargetViews_[0], View);
	}
	else
	{
		GameEngineDevice::GetContext()->OMSetRenderTargets(1, &RenderTargetViews_[_Index], View);
	}
}

void GameEngineRenderTarget::Create(const std::string _TextureName, float4 _ClearColor)
{
	GameEngineTexture* FindTexture = GameEngineTextureManager::GetInst().Find(_TextureName);
	if (nullptr == FindTexture)
	{
		GameEngineDebug::MsgBoxError("FindTexture Is null Create Render Target Error");
	}

	FindTexture->CreateRenderTargetView();
	FindTexture->CreateShaderResourceView();

	Create(FindTexture, _ClearColor);
}

void GameEngineRenderTarget::Create(float4 _Size, float4 _ClearColor)
{
	GameEngineTexture* NewTexture = new GameEngineTexture();

	NewTexture->Create(_Size, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);

	ReleaseTextures_.push_back(NewTexture);

	Create(NewTexture, _ClearColor);
}

void GameEngineRenderTarget::CreateDepthBuffer(float4 _Scale) 
{
	if (nullptr != DepthBuffer_)
	{
		return;
	}

	DepthBuffer_ = new GameEngineDepthBuffer();
	DepthBuffer_->Create(_Scale);
}

void GameEngineRenderTarget::Create(GameEngineTexture* _Texture, float4 _ClearColor)
{
	Textures_.push_back(_Texture);
	RenderTargetViews_.push_back(_Texture->GetRenderTargetView());
	ShaderResourcesViews_.push_back(*_Texture->GetShaderResourcesView());
	ClearColor_.push_back(_ClearColor);
}

void GameEngineRenderTarget::Merge(GameEngineRenderTarget* _Other, int _Index) 
{
	Setting();
	Res_.SettingTexture("Tex", _Other->Textures_[_Index]);
	Res_.Setting();
	Pipe_->Rendering();
	Pipe_->Reset();
	Res_.ReSet();
}

void GameEngineRenderTarget::Copy(GameEngineRenderTarget* _Other) 
{
	Clear();
	Merge(_Other);
}
