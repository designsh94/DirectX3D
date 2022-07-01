#pragma once
#include "GameEngineObjectNameBase.h"

class GameEngineThread : public GameEngineObjectNameBase
{
public:
	static void GameServerThreadFunction(GameEngineThread* _Thread, std::string _Name);

private:
	std::thread Thread;
	std::function<void(GameEngineThread*)> ThreadFunction;

public:
	GameEngineThread();
	~GameEngineThread();

public:
	GameEngineThread(const GameEngineThread& _Other) = delete;
	GameEngineThread(GameEngineThread&& _Other) noexcept = delete;
	GameEngineThread& operator=(const GameEngineThread& _Other) = delete;
	GameEngineThread& operator=(GameEngineThread&& _Other) noexcept = delete;

public:
	void Start(std::string _ThreadName, std::function<void(GameEngineThread*)> _Function)
	{
		ThreadFunction = _Function;
		Thread = std::thread(GameServerThreadFunction, this, _ThreadName);
	}

	void Join()
	{
		Thread.join();
	}
};

