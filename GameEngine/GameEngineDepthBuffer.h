#pragma once
#include "GameEngineTexture.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineDepthBuffer : public GameEngineObjectNameBase
{
private:
	GameEngineTexture* DepthTexture_;

public:
	GameEngineDepthBuffer();
	~GameEngineDepthBuffer();

protected:
	GameEngineDepthBuffer(const GameEngineDepthBuffer& _Other) = delete;
	GameEngineDepthBuffer(GameEngineDepthBuffer&& _Other) noexcept = delete;

private:
	GameEngineDepthBuffer& operator=(const GameEngineDepthBuffer& _Other) = delete;
	GameEngineDepthBuffer& operator=(GameEngineDepthBuffer&& _Other) noexcept = delete;

public:
	inline ID3D11DepthStencilView* GetDepthStencilView()
	{
		return DepthTexture_->GetDepthStencilView();
	}

public:
	void Create(float4 _Scale);

public:
};

