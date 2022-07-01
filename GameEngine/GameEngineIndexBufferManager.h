#pragma once
#include "GameEngineDevice.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineIndexBuffer;
class GameEngineIndexBufferManager
{
private:
	static GameEngineIndexBufferManager* Inst;

public:
	static GameEngineIndexBufferManager& GetInst()
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
	std::map<std::string, GameEngineIndexBuffer*> ResourcesMap;

private:
	GameEngineIndexBufferManager();
	~GameEngineIndexBufferManager();

protected:
	GameEngineIndexBufferManager(const GameEngineIndexBufferManager& _other) = delete;
	GameEngineIndexBufferManager(GameEngineIndexBufferManager&& _other) noexcept;

private:
	GameEngineIndexBufferManager& operator=(const GameEngineIndexBufferManager& _other) = delete;
	GameEngineIndexBufferManager& operator=(const GameEngineIndexBufferManager&& _other) = delete;

public:
	GameEngineIndexBuffer* Create(const std::string& _Name, const std::vector<unsigned int>& _Index, D3D11_USAGE _Usage);
	GameEngineIndexBuffer* Load(const std::string& _Path);
	GameEngineIndexBuffer* Load(const std::string& _Name, const std::string& _Path);
	GameEngineIndexBuffer* Find(const std::string& _Name);

public:
};




