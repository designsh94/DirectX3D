#pragma once
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineSampler : public GameEngineObjectNameBase
{
public:
	friend class GameEngineSamplerManager;

private:
	ID3D11SamplerState* State_;

public:
	D3D11_SAMPLER_DESC Info_;

public:
	GameEngineSampler();
	~GameEngineSampler();

protected:
	GameEngineSampler(const GameEngineSampler& _other) = delete;
	GameEngineSampler(GameEngineSampler&& _other) noexcept = delete;

private:
	GameEngineSampler& operator=(const GameEngineSampler& _other) = delete;
	GameEngineSampler& operator=(const GameEngineSampler&& _other) = delete;

private:
	void Create(const D3D11_SAMPLER_DESC& _Info);

public:
	inline ID3D11SamplerState** GetSamplerState()
	{
		return &State_;
	}

public:
	void ReCreate();
	void ReCreate(const D3D11_SAMPLER_DESC& _Info);

public:
};

