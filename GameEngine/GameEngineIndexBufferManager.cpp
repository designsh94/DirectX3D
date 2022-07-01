#include "PreCompile.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineIndexBufferManager.h"

GameEngineIndexBufferManager* GameEngineIndexBufferManager::Inst = new GameEngineIndexBufferManager();

GameEngineIndexBufferManager::GameEngineIndexBufferManager()
{
}

GameEngineIndexBufferManager::~GameEngineIndexBufferManager()
{
	for (const std::pair<std::string, GameEngineIndexBuffer*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineIndexBufferManager::GameEngineIndexBufferManager(GameEngineIndexBufferManager&& _other) noexcept
{
}

GameEngineIndexBuffer* GameEngineIndexBufferManager::Create(const std::string& _Name, const std::vector<UINT>& _Index, D3D11_USAGE _Usage)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineIndexBuffer* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(UpperName + " Is Overlap Create");
	}

	GameEngineIndexBuffer* NewRes = new GameEngineIndexBuffer();
	NewRes->SetName(UpperName);
	NewRes->Create(_Index, _Usage);

	ResourcesMap.insert(std::map<std::string, GameEngineIndexBuffer*>::value_type(UpperName, NewRes));

	return NewRes;
}

GameEngineIndexBuffer* GameEngineIndexBufferManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineIndexBuffer* GameEngineIndexBufferManager::Load(const std::string& _Name, const std::string& _Path)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineIndexBuffer* FindRes = Find(UpperName);
	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineIndexBuffer* NewRes = new GameEngineIndexBuffer();
	NewRes->SetName(UpperName);

	ResourcesMap.insert(std::map<std::string, GameEngineIndexBuffer*>::value_type(UpperName, NewRes));
	return NewRes;
}

GameEngineIndexBuffer* GameEngineIndexBufferManager::Find(const std::string& _Name)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	std::map<std::string, GameEngineIndexBuffer*>::iterator FindIter = ResourcesMap.find(UpperName);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}
