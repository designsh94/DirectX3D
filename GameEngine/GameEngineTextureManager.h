#pragma once
#include "GameEngineDevice.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineTexture;
class GameEngineTextureManager
{
private:
	static GameEngineTextureManager* Inst;
	static std::mutex ManagerLock;

public:
	static GameEngineTextureManager& GetInst()
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
	std::map<std::string, GameEngineTexture*> ResourcesMap;

private:
	GameEngineTextureManager();
	~GameEngineTextureManager();

protected:
	GameEngineTextureManager(const GameEngineTextureManager& _other) = delete;
	GameEngineTextureManager(GameEngineTextureManager&& _other) noexcept;

private:
	GameEngineTextureManager& operator=(const GameEngineTextureManager& _other) = delete;
	GameEngineTextureManager& operator=(const GameEngineTextureManager&& _other) = delete;

public:
	GameEngineTexture* Create(const std::string& _Name, ID3D11Texture2D* _Texture2D);
	GameEngineTexture* Load(const std::string& _Path);
	GameEngineTexture* Load(const std::string& _Name, const std::string& _Path);
	GameEngineTexture* Find(const std::string& _Name);

public:
};
