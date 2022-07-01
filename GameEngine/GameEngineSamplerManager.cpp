#include "PreCompile.h"
#include "GameEngineSamplerManager.h"
#include "GameEngineSampler.h"

GameEngineSamplerManager* GameEngineSamplerManager::Inst = new GameEngineSamplerManager();

GameEngineSamplerManager::GameEngineSamplerManager()
{
}

GameEngineSamplerManager::~GameEngineSamplerManager()
{
	for (const std::pair<std::string, GameEngineSampler*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineSamplerManager::GameEngineSamplerManager(GameEngineSamplerManager&& _other) noexcept
{
}

GameEngineSampler* GameEngineSamplerManager::Create(const std::string& _Name, const D3D11_SAMPLER_DESC& _Info)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineSampler* FindRes = Find(UpperName);
	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}

	GameEngineSampler* NewRes = new GameEngineSampler();
	NewRes->SetName(UpperName);
	NewRes->Create(_Info);

	ResourcesMap.insert(std::map<std::string, GameEngineSampler*>::value_type(UpperName, NewRes));

	return NewRes;
}

GameEngineSampler* GameEngineSamplerManager::Find(const std::string& _Name)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	std::map<std::string, GameEngineSampler*>::iterator FindIter = ResourcesMap.find(UpperName);
	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}

GameEngineSampler* GameEngineSamplerManager::CreateAndFind(const std::string& _Name, const D3D11_SAMPLER_DESC& _Info)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineSampler* FindRes = Find(UpperName);
	if (nullptr != FindRes)
	{
		return FindRes;
	}

	GameEngineSampler* NewRes = new GameEngineSampler();
	NewRes->SetName(UpperName);
	NewRes->Create(_Info);

	ResourcesMap.insert(std::map<std::string, GameEngineSampler*>::value_type(UpperName, NewRes));

	return NewRes;
}
