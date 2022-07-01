#pragma once
#include "GameEngineDevice.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineSampler;
class GameEngineSamplerManager
{
private:
	static GameEngineSamplerManager* Inst;

public:
	static GameEngineSamplerManager& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:
	std::map<std::string, GameEngineSampler*> ResourcesMap;

private:
	GameEngineSamplerManager();
	~GameEngineSamplerManager();

protected:
	GameEngineSamplerManager(const GameEngineSamplerManager& _other) = delete;
	GameEngineSamplerManager(GameEngineSamplerManager&& _other) noexcept;

private:
	GameEngineSamplerManager& operator=(const GameEngineSamplerManager& _other) = delete;
	GameEngineSamplerManager& operator=(const GameEngineSamplerManager&& _other) = delete;

public:
	GameEngineSampler* Create(const std::string& _Name, const D3D11_SAMPLER_DESC& _Info);
	GameEngineSampler* Find(const std::string& _Name);
	GameEngineSampler* CreateAndFind(const std::string& _Name, const D3D11_SAMPLER_DESC& _Info);

public:
};
