#pragma once
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineBlend : public GameEngineObjectNameBase
{
public:
	friend class GameEngineBlendManager;

private:
	ID3D11BlendState* State_;
	D3D11_BLEND_DESC Info_;
	float4 Factor_;
	unsigned int Mask_;

public:
	GameEngineBlend();
	~GameEngineBlend();

protected:
	GameEngineBlend(const GameEngineBlend& _other) = delete;
	GameEngineBlend(GameEngineBlend&& _other) noexcept = delete;

private:
	GameEngineBlend& operator=(const GameEngineBlend& _other) = delete;
	GameEngineBlend& operator=(const GameEngineBlend&& _other) = delete;

public:
	inline ID3D11BlendState** GetSamplerState()
	{
		return &State_;
	}

private:
	void Create(const D3D11_BLEND_DESC& _Info, float4 _Factor, unsigned int _Mask);
	void ReCreate(const D3D11_BLEND_DESC& _Info, float4 _Factor, unsigned int _Mask);

public:
	void Setting();
	void Reset();
};

