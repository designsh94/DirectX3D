#pragma once
#include <GameEngine/GameEngineDevice.h>
#include "GameEngineFBXMesh.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineFBXMesh;
class GameEngineFBXMeshManager
{
	friend class GameEngineFBXWindow;

private:
	static GameEngineFBXMeshManager* Inst;

public:
	static GameEngineFBXMeshManager& GetInst()
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
	std::map<std::string, GameEngineFBXMesh*> ResourcesMap;

private:
	GameEngineFBXMeshManager();
	~GameEngineFBXMeshManager();

protected:
	GameEngineFBXMeshManager(const GameEngineFBXMeshManager& _other) = delete;
	GameEngineFBXMeshManager(GameEngineFBXMeshManager&& _other) noexcept;

private:
	GameEngineFBXMeshManager& operator=(const GameEngineFBXMeshManager& _other) = delete;
	GameEngineFBXMeshManager& operator=(const GameEngineFBXMeshManager&& _other) = delete;

public:
	GameEngineFBXMesh* Load(const std::string& _Path);
	GameEngineFBXMesh* Load(const std::string& _Name, const std::string& _Path);
	GameEngineFBXMesh* Find(const std::string& _Name);

public:
	void DeletePath(const std::string& _Path);
	void Delete(const std::string& _Name);

public:
};




