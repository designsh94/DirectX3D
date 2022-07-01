#pragma once
#include "GameEngineVertexBuffer.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineVertexBuffer;
class GameEngineVertexBufferManager
{
private:
	static GameEngineVertexBufferManager* Inst;

public:
	static GameEngineVertexBufferManager& GetInst()
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
	std::map<std::string, GameEngineVertexBuffer*> ResourcesMap;

private:
	GameEngineVertexBufferManager();
	~GameEngineVertexBufferManager();

protected:
	GameEngineVertexBufferManager(const GameEngineVertexBufferManager& _other) = delete;
	GameEngineVertexBufferManager(GameEngineVertexBufferManager&& _other) noexcept;

private:
	GameEngineVertexBufferManager& operator=(const GameEngineVertexBufferManager& _other) = delete;
	GameEngineVertexBufferManager& operator=(const GameEngineVertexBufferManager&& _other) = delete;

public:
	template<typename VertexType>
	GameEngineVertexBuffer* Create(const std::string& _Name, const std::vector<VertexType>& _Vertex, D3D11_USAGE _Usage)
	{
		GameEngineVertexBuffer* FindRes = Find(_Name);

		if (nullptr != FindRes)
		{
			GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
		}


		GameEngineVertexBuffer* NewRes = new GameEngineVertexBuffer();
		NewRes->SetName(_Name);
		NewRes->Create<VertexType>(_Vertex, _Usage);
		ResourcesMap.insert(std::map<std::string, GameEngineVertexBuffer*>::value_type(_Name, NewRes));
		return NewRes;
	}

public:
	GameEngineVertexBuffer* Load(const std::string& _Path);
	GameEngineVertexBuffer* Load(const std::string& _Name, const std::string& _Path);
	GameEngineVertexBuffer* Find(const std::string& _Name);

public:
};

