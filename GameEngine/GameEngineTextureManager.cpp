#include "PreCompile.h"
#include "GameEngineTextureManager.h"
#include "GameEngineTexture.h"

GameEngineTextureManager* GameEngineTextureManager::Inst = new GameEngineTextureManager();

std::mutex GameEngineTextureManager::ManagerLock;

GameEngineTextureManager::GameEngineTextureManager()
{
}

GameEngineTextureManager::~GameEngineTextureManager()
{
	for (const std::pair<std::string, GameEngineTexture*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineTextureManager::GameEngineTextureManager(GameEngineTextureManager&& _other) noexcept
{
}

GameEngineTexture* GameEngineTextureManager::Create(const std::string& _Name, ID3D11Texture2D* _Texture2D)
{
	std::string UpName = GameEngineString::toupper(_Name);

	GameEngineTexture* FindRes = Find(UpName);
	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(UpName + " Is Overlap Create");
	}

	GameEngineTexture* NewRes = new GameEngineTexture();
	NewRes->SetName(UpName);
	NewRes->Create(_Texture2D);
	
	{
		std::lock_guard Lock(ManagerLock);
		ResourcesMap.insert(std::map<std::string, GameEngineTexture*>::value_type(UpName, NewRes));
	}
	return NewRes;
}

GameEngineTexture* GameEngineTextureManager::Load(const std::string& _Path)
{
	return Load(GameEngineString::toupper(GameEnginePath::GetFileName(_Path)), _Path);
}

GameEngineTexture* GameEngineTextureManager::Load(const std::string& _Name, const std::string& _Path)
{
	GameEngineTexture* FindRes = Find(_Name);
	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineTexture* NewRes = new GameEngineTexture();
	NewRes->SetName(_Name);
	NewRes->Load(_Path);
	{
		std::lock_guard Lock(ManagerLock);
		ResourcesMap.insert(std::map<std::string, GameEngineTexture*>::value_type(_Name, NewRes));
	}
	return NewRes;
}

GameEngineTexture* GameEngineTextureManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineTexture*>::iterator FindIter;
	{
		std::lock_guard Lock(ManagerLock);

		FindIter = ResourcesMap.find(GameEngineString::toupper(_Name));
	}

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}
