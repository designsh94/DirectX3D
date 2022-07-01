#pragma once
#include "GameEngineDevice.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineFolderTexture;
class GameEngineFolderTextureManager
{
private:
	static GameEngineFolderTextureManager* Inst;

public:
	static GameEngineFolderTextureManager& GetInst()
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
	std::map<std::string, GameEngineFolderTexture*> ResourcesMap;

private:
	GameEngineFolderTextureManager();
	~GameEngineFolderTextureManager();

protected:
	GameEngineFolderTextureManager(const GameEngineFolderTextureManager& _other) = delete;
	GameEngineFolderTextureManager(GameEngineFolderTextureManager&& _other) noexcept;

private:
	GameEngineFolderTextureManager& operator=(const GameEngineFolderTextureManager& _other) = delete;
	GameEngineFolderTextureManager& operator=(const GameEngineFolderTextureManager&& _other) = delete;

public:
	GameEngineFolderTexture* Load(const std::string& _Path);
	GameEngineFolderTexture* Load(const std::string& _Name, const std::string& _Path);
	GameEngineFolderTexture* Find(const std::string& _Name);

public:
};
