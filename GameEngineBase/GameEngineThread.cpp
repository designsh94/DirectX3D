#include "PreCompile.h"
#include "GameEngineThread.h"
#include "GameEngineString.h"

void GameEngineThread::GameServerThreadFunction(GameEngineThread* _Thread, std::string _Name)
{
	std::wstring WName;
	GameEngineString::AnsiToUnicode(_Name, WName);
	SetThreadDescription(GetCurrentThread(), WName.c_str());
	_Thread->ThreadFunction(_Thread);
}

GameEngineThread::GameEngineThread() 
{
}

GameEngineThread::~GameEngineThread() 
{
	Join();
}
