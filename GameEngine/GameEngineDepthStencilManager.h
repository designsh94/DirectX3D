#pragma once
#include "GameEngineDevice.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineDepthStencil;
class GameEngineDepthStencilManager
{
private:
	static GameEngineDepthStencilManager* Inst;

public:
	static GameEngineDepthStencilManager& GetInst()
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
	std::map<std::string, GameEngineDepthStencil*> ResourcesMap;

private:
	GameEngineDepthStencilManager();
	~GameEngineDepthStencilManager();

protected:
	GameEngineDepthStencilManager(const GameEngineDepthStencilManager& _other) = delete;
	GameEngineDepthStencilManager(GameEngineDepthStencilManager&& _other) noexcept;

private:
	GameEngineDepthStencilManager& operator=(const GameEngineDepthStencilManager& _other) = delete;
	GameEngineDepthStencilManager& operator=(const GameEngineDepthStencilManager&& _other) = delete;

public:
	GameEngineDepthStencil* Create(const std::string& _Name, const D3D11_DEPTH_STENCIL_DESC& DepthInfo);
	GameEngineDepthStencil* Load(const std::string& _Path);
	GameEngineDepthStencil* Load(const std::string& _Name, const std::string& _Path);
	GameEngineDepthStencil* Find(const std::string& _Name);

public:
};
