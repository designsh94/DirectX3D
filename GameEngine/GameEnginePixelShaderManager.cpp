#include "PreCompile.h"
#include "GameEnginePixelShaderManager.h"
#include "GameEnginePixelShader.h"
#include "GameEngineBase/GameEngineFile.h"

GameEnginePixelShaderManager* GameEnginePixelShaderManager::Inst = new GameEnginePixelShaderManager();

GameEnginePixelShaderManager::GameEnginePixelShaderManager()
{
}

GameEnginePixelShaderManager::~GameEnginePixelShaderManager()
{
	for (const std::pair<std::string, GameEnginePixelShader*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEnginePixelShaderManager::GameEnginePixelShaderManager(GameEnginePixelShaderManager&& _other) noexcept
{
}

GameEnginePixelShader* GameEnginePixelShaderManager::Load(const std::string& _Path, const std::string& _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	std::string FileName = GameEnginePath::GetFileName(_Path);

	return Load(FileName, _Path, _EntryPoint);
}

GameEnginePixelShader* GameEnginePixelShaderManager::Load(const std::string& _Name, const std::string& _Path, const std::string& _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	GameEnginePixelShader* FindRes = Find(_Name);
	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}

	GameEnginePixelShader* NewRes = new GameEnginePixelShader();
	NewRes->SetName(_Name);
	if (false == NewRes->Load(_Path, _EntryPoint, _VersionHigh, _VersionLow))
	{
		delete NewRes;
		return nullptr;
	}

	ResourcesMap.insert(std::map<std::string, GameEnginePixelShader*>::value_type(_Name, NewRes));

	return NewRes;
}

GameEnginePixelShader* GameEnginePixelShaderManager::Create(const std::string& _Name, const std::string& _ShaderCode, const std::string& _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	GameEnginePixelShader* FindRes = Find(_Name);
	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}

	GameEnginePixelShader* NewRes = new GameEnginePixelShader();
	NewRes->SetName(_Name);
	if (false == NewRes->Create(_ShaderCode, _EntryPoint, _VersionHigh, _VersionLow))
	{
		delete NewRes;
		return nullptr;
	}

	ResourcesMap.insert(std::map<std::string, GameEnginePixelShader*>::value_type(_Name, NewRes));

	return NewRes;
}

GameEnginePixelShader* GameEnginePixelShaderManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEnginePixelShader*>::iterator FindIter = ResourcesMap.find(_Name);
	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}
