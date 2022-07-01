#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include <GameEngine/GameEngineDevice.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineDepthStencil : public GameEngineObjectNameBase
{
public:
	friend class GameEngineDepthStencilManager;

private:
	D3D11_DEPTH_STENCIL_DESC Info_;
	ID3D11DepthStencilState* State_;

public:
	GameEngineDepthStencil();
	~GameEngineDepthStencil();

protected:
	GameEngineDepthStencil(const GameEngineDepthStencil& _Other) = delete;
	GameEngineDepthStencil(GameEngineDepthStencil&& _Other) noexcept = delete;

private:
	GameEngineDepthStencil& operator=(const GameEngineDepthStencil& _Other) = delete;
	GameEngineDepthStencil& operator=(GameEngineDepthStencil&& _Other) noexcept = delete;

private:
	void Create(const D3D11_DEPTH_STENCIL_DESC& _Info);

public:
	void ReCreate(const D3D11_DEPTH_STENCIL_DESC& _Info);
	void Setting();
	void Reset();

public:
};

