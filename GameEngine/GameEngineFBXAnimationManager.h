#pragma once
#include "GameEngineDevice.h"
#include "GameEngineFBXAnimation.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineFBXAnimation;
class GameEngineFBXAnimationManager
{
	friend class GameEngineFBXWindow;

private:
	static GameEngineFBXAnimationManager* Inst;

public:
	static GameEngineFBXAnimationManager& GetInst()
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
	std::map<std::string, GameEngineFBXAnimation*> ResourcesMap;

private:
	GameEngineFBXAnimationManager();
	~GameEngineFBXAnimationManager();

protected:
	GameEngineFBXAnimationManager(const GameEngineFBXAnimationManager& _other) = delete;
	GameEngineFBXAnimationManager(GameEngineFBXAnimationManager&& _other) noexcept;

private:
	GameEngineFBXAnimationManager& operator=(const GameEngineFBXAnimationManager& _other) = delete;
	GameEngineFBXAnimationManager& operator=(const GameEngineFBXAnimationManager&& _other) = delete;

public:
	GameEngineFBXAnimation* Load(const std::string& _Path);
	GameEngineFBXAnimation* Load(const std::string& _Name, const std::string& _Path);
	GameEngineFBXAnimation* Find(const std::string& _Name);

public:
	void DeletePath(const std::string& _Path);
	void Delete(const std::string& _Name);

public:
};




