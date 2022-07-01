#include "PreCompile.h"
#include "GameEngineFBXAnimationManager.h"
#include "GameEngineFBXAnimation.h"

GameEngineFBXAnimationManager* GameEngineFBXAnimationManager::Inst = new GameEngineFBXAnimationManager();

GameEngineFBXAnimationManager::GameEngineFBXAnimationManager()
{
}

GameEngineFBXAnimationManager::~GameEngineFBXAnimationManager()
{
	for (const std::pair<std::string, GameEngineFBXAnimation*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineFBXAnimationManager::GameEngineFBXAnimationManager(GameEngineFBXAnimationManager&& _other) noexcept
{
}

GameEngineFBXAnimation* GameEngineFBXAnimationManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineFBXAnimation* GameEngineFBXAnimationManager::Load(const std::string& _Name, const std::string& _Path)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineFBXAnimation* FindRes = Find(UpperName);
	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineFBXAnimation* NewRes = new GameEngineFBXAnimation();
	NewRes->SetName(UpperName);
	NewRes->Load(_Path);

	ResourcesMap.insert(std::map<std::string, GameEngineFBXAnimation*>::value_type(UpperName, NewRes));

	return NewRes;
}

GameEngineFBXAnimation* GameEngineFBXAnimationManager::Find(const std::string& _Name)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	std::map<std::string, GameEngineFBXAnimation*>::iterator FindIter = ResourcesMap.find(UpperName);
	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}

void GameEngineFBXAnimationManager::DeletePath(const std::string& _Path)
{
	return Delete(GameEnginePath::GetFileName(_Path));
}

void GameEngineFBXAnimationManager::Delete(const std::string& _Name)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineFBXAnimation* FindRes = Find(UpperName);
	if (nullptr == FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + "는 존재하지 않는 애니메이션FBX 입니다 삭제할수 없습니다.");
		return;
	}

	delete FindRes;

	ResourcesMap.erase(UpperName);
	return;
}
