#include "PreCompile.h"
#include "GameEngineDepthStencilManager.h"
#include "GameEngineDepthStencil.h"

GameEngineDepthStencilManager* GameEngineDepthStencilManager::Inst = new GameEngineDepthStencilManager();

GameEngineDepthStencilManager::GameEngineDepthStencilManager()
{
}

GameEngineDepthStencilManager::~GameEngineDepthStencilManager()
{
	for (const std::pair<std::string, GameEngineDepthStencil*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineDepthStencilManager::GameEngineDepthStencilManager(GameEngineDepthStencilManager&& _other) noexcept
{
}

GameEngineDepthStencil* GameEngineDepthStencilManager::Create(const std::string& _Name, const D3D11_DEPTH_STENCIL_DESC& DepthInfo)
{
	GameEngineDepthStencil* FindRes = Find(_Name);
	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}

	GameEngineDepthStencil* NewRes = new GameEngineDepthStencil();
	NewRes->SetName(_Name);
	NewRes->Create(DepthInfo);

	ResourcesMap.insert(std::map<std::string, GameEngineDepthStencil*>::value_type(_Name, NewRes));

	return NewRes;
}

GameEngineDepthStencil* GameEngineDepthStencilManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineDepthStencil* GameEngineDepthStencilManager::Load(const std::string& _Name, const std::string& _Path)
{
	GameEngineDepthStencil* FindRes = Find(_Name);
	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineDepthStencil* NewRes = new GameEngineDepthStencil();
	NewRes->SetName(_Name);

	ResourcesMap.insert(std::map<std::string, GameEngineDepthStencil*>::value_type(_Name, NewRes));

	return NewRes;
}

GameEngineDepthStencil* GameEngineDepthStencilManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineDepthStencil*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}
